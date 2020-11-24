#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mywords.h"

#include "QStandardItemModel"
#include "QStandardItem"

#include <QTimer>
#include <QFile>
#include <QMessageBox>
#include <QMap>
#include <QList>
#include <QString>
#include <QPushButton>
#include <QClipboard>
#include <qt_windows.h>
#include <QShortcut>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionbrowser_triggered();
    void on_actiontable_triggered();
    void on_actionSelection_triggered();

    void sLoadFinished(bool);
    void sTimerAlarm();

    void on_pbTranslate_clicked();

    void on_AddWord_clicked();
    void sDataChanged();

    void on_actionCopy_data_triggered();

private:
    Ui::MainWindow *ui;

    HWND hwnd;

    bool    isLoadFinished;
    QString JQueryGetTranslation;
    QString JQuerySearchTranslation;
    QString JQueryCheck;

    QString prevClipboardText;

    QTimer  *timer;

    QLayout *scrollLayout;
    QList<Words*> trs;

    QMap<QString, QList<QString>> dict;

    QPushButton *btnAdd;

    QShortcut *keyCtrlC;

    void setJQueryScript(QString&, const QString&);
    void translationFound();
    void getWordTranslation(const QVariant&);
};
#endif // MAINWINDOW_H
