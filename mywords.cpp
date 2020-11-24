#include "mywords.h"
#include "ui_mywords.h"
#include <QDebug>

Words::Words(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Words)
{
    ui->setupUi(this);
    count = 0;
}

Words::~Words()
{
    delete ui;
}

void Words::cbStateChanged(int state)
{
    switch (state) {
    case 0:
        count--;
        if (count == 0)
            ui->mainCB->setCheckState(Qt::Unchecked);
        break;
    case 2:
        count++;
        if (count == 1)
            ui->mainCB->setCheckState(Qt::Checked);
        break;
    }
}

void Words::setName(const QString &name)
{
    ui->mainLbl->setText(name);
}

void Words::setWidget(QWidget *w, int row, int column)
{
    ui->gridLayout->addWidget(w, row, column);
}

void Words::bindLastElement()
{
    connect(cbs.back(), SIGNAL(stateChanged(int)), SLOT(cbStateChanged(int)));
}

void Words::on_mainCB_clicked()
{
    int state = ui->mainCB->checkState();

    switch (state) {
    case 0:
        for (int i = 0; i < cbs.size(); i++)
            cbs[i]->setCheckState(Qt::Unchecked);
        count = 0;
        break;
    case 2:
        for (int i = 0; i < cbs.size(); i++)
            cbs[i]->setCheckState(Qt::Checked);
        count = cbs.size();
        break;
    }
}

QString Words::getLblName()
{
    return ui->mainLbl->text();
}











