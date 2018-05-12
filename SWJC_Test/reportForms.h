#ifndef REPORTFORMS_H
#define REPORTFORMS_H

#include <QDialog>
#include <QDate>
#include <QFileDialog>
#include "common/comm.h"
#include "common/mymysql.h"
#include "common/qword.h"

namespace Ui {
class reportForms;
}

class reportForms : public QDialog
{
    Q_OBJECT

public:
    explicit reportForms(mymysql *mysqlDB);
    ~reportForms();

private slots:
    void on_pushBtn_confirm_clicked();

    void on_pushBtn_cancel_clicked();

    void on_pushBtn_selectPath_clicked();

private:
    Ui::reportForms *ui;

    mymysql *m_mysqlDB;
    QList<QList<QVariant>> m_datas;
    QList<QList<QVariant>> m_maxDatas;
    QList<QList<QVariant>> m_minDatas;
    QList<QList<QVariant>> m_avgDatas;

    void printReportForms();
};

#endif // REPORTFORMS_H
