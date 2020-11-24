#ifndef WORDS_H
#define WORDS_H

#include <QWidget>
#include <QList>
#include <QCheckBox>
#include <QLabel>

namespace Ui {
class Words;
}

class Words : public QWidget
{
    Q_OBJECT

public:
    explicit Words(QWidget *parent = nullptr);
    ~Words();

    QList<QCheckBox*> cbs;
    QList<QLabel*>    lbs;

    void setName(const QString&);

    void setWidget(QWidget*, int, int);
    void bindLastElement();
    QString getLblName();
    int count;

private slots:
    void cbStateChanged(int);
    void on_mainCB_clicked();

private:
    Ui::Words *ui;

};

#endif // WORDS_H














