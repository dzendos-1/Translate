#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/qt-logo.ico"));

    isLoadFinished = false;

    prevClipboardText = "";

    ui->webEngineView->load(QUrl("https://www.translate.ru/"));

    setJQueryScript(JQueryGetTranslation,    ":/JQuery/JQueryGetTranslation.js");
    setJQueryScript(JQuerySearchTranslation, ":/JQuery/JQuerySearchTranslation.js");
    setJQueryScript(JQueryCheck,             ":/JQuery/JQueryCheck.js");

    connect(ui->webEngineView, SIGNAL(loadFinished(bool)), SLOT(sLoadFinished(bool)));

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), SLOT(sTimerAlarm()));

    scrollLayout = ui->gridLayout_4;
    scrollLayout->setAlignment(Qt::AlignTop);

    ui->horizontalLayout->setAlignment(Qt::AlignTop);

    btnAdd = new QPushButton("Add Word");
    btnAdd->setMaximumSize(75, 23);

    connect(QApplication::clipboard(), SIGNAL(dataChanged()), SLOT(sDataChanged()));
    ui->stackedWidget->setCurrentIndex(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sLoadFinished(bool)
{
    isLoadFinished = true;
    ui->statusBar->showMessage("Load finished!", 3000);
}

void MainWindow::sTimerAlarm()
{
    timer->stop();
    ui->webEngineView->page()->runJavaScript(JQueryCheck, [this](const QVariant &v) { ui->statusBar->showMessage(v.toString()); if (v.toBool() == true) translationFound(); else  timer->start(10);  } );
}

void MainWindow::on_actionbrowser_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actiontable_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionSelection_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::setJQueryScript(QString &script, const QString &scriptName) {
    QFile file(scriptName);
    file.open(QIODevice::ReadOnly);
    script = file.readAll();
    file.close();
}

void MainWindow::on_pbTranslate_clicked()
{
    if (!isLoadFinished) {
        QMessageBox::information(0, "Oops..", "The page hasn't loaded yet.");
        return;
    }

    ui->webEngineView->page()->runJavaScript(JQuerySearchTranslation.arg(ui->leWord->text()));

    qDeleteAll(trs);
        trs.clear();

    while (QLayoutItem* item = scrollLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }

    timer->start(10);
}

void MainWindow::translationFound()
{
    ui->webEngineView->page()->runJavaScript(JQueryGetTranslation, [this] (const QVariant &v) { getWordTranslation(v); } );
}

void MainWindow::getWordTranslation(const QVariant &v)
{
    QList<QVariant> list  = v.toList();

    for (int i = 0; i < list.size(); i++) {
        QList<QVariant> words = list[i].toList();

        trs.push_back(new Words);
        trs.back()->setName(words[0].toString());

        bool isInTable = false;
        if (dict.contains(words[0].toString()))
            isInTable = true;

        QString translation = "";
        for (int j = 1; j < words.size(); j++) {
            QCheckBox * curCb  = new QCheckBox("");
            curCb->setMaximumSize(13, 13);

            QLabel    *curWord = new QLabel(words[j].toString());
            curWord->setMaximumHeight(16);

            trs.back()->cbs.push_back(curCb);
            trs.back()->bindLastElement();
            trs.back()->lbs.push_back(curWord);

            if (isInTable && dict[words[0].toString()].contains(words[j].toString()))
                curCb->setCheckState(Qt::Checked);

            trs.back()->setWidget(curCb, j, 0);
            trs.back()->setWidget(curWord, j, 1);

            translation += words[j].toString();
            if (j != words.size() - 1)
                translation += ", ";
        }
        scrollLayout->addWidget(trs.back());
    }

    ui->statusBar->showMessage("Translation is found!", 3000);
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_AddWord_clicked()
{
    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *item;

    model->setHorizontalHeaderLabels({"Word", "Translation"});

    QMap<QString, QList<QString>> curDict;

    for (int i = 0; i < trs.size(); i++) {
        if (trs[i]->count == 0) {
            curDict[trs[i]->getLblName()];
            continue;
        }

        for (int j = 0; j < trs[i]->cbs.size(); j++)
            if (trs[i]->cbs[j]->isChecked())
                curDict[trs[i]->getLblName()].append(trs[i]->lbs[j]->text());
    }

    foreach (const QString &key, curDict.keys()) {
        if (curDict[key].size() == 0) {
            dict.remove(key);
            continue;
        }

        dict[key] = curDict[key];
    }

    int cnt = 0;
    foreach (const QString &key, dict.keys()) {
        item = new QStandardItem(key);
        model->setItem(cnt, 0, item);

        QString translation = "";
        foreach (const QString &w, dict[key]) {
            if (translation != "")
                translation += ", ";
            translation += w;
        }

        item = new QStandardItem(translation);
        model->setItem(cnt, 1, item);

        cnt++;
    }

    ui->tableView->setModel(model);

    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();

    ui->statusBar->showMessage("succeeded!", 3000);
}

void MainWindow::sDataChanged()
{
    if (prevClipboardText == QApplication::clipboard()->text()) {
        ui->leWord->setText(prevClipboardText);
        showMinimized();
        ShowWindow((HWND)MainWindow::winId(), SW_RESTORE);
        SetForegroundWindow((HWND)MainWindow::winId());
        on_pbTranslate_clicked();
    }

    prevClipboardText = QApplication::clipboard()->text();
}

void MainWindow::on_actionCopy_data_triggered()
{
    if(ui->tableView->model() == nullptr) {
        ui->statusBar->showMessage("Dictionary is empty!", 3000);
        return;
    }

    QModelIndex    index;
    QString        str;

    const QItemSelection ranges = ui->tableView->selectionModel()->selection();


    for (int i = 0; i <= ui->tableView->model()->rowCount(); i++) {
        for (int j = 0; j <= ui->tableView->model()->columnCount(); j++) {
            index = ui->tableView->model()->index(i, j, QModelIndex());
            QString t = ui->tableView->model()->data(index).toString();
            str += t;
            str += "\t";
        }
        str += "\n";
    }

    ui->statusBar->showMessage("Copied successfully!", 3000);
    QApplication::clipboard()->setText(str);
}
