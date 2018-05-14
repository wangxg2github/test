#ifndef REPORTFORMS_H
#define REPORTFORMS_H

#include <QDialog>
#include <QDate>
#include <QFileDialog>
#include <QLineSeries>
#include <QChart>
#include <QValueAxis>
#include <QChartView>

#include "common/comm.h"
#include "common/mymysql.h"
#include "common/qword.h"
#include "common/qcustomplot.h"

using namespace QtCharts;


class printFormsThread : public QThread
{
    Q_OBJECT

public:
    printFormsThread(QString strHoleNumber,
                     QString strDate,
                     QString filePathOfWord,
                     QString filenameOfDeepness,
                     QString filenameOfTemperature,
                     QString strMaxDeepness,
                     QString strMaxTemperature,
                     QString strMinDeepnees,
                     QString strMinTemperature,
                     QString strAvgDeepnees,
                     QString strAvgTemperature,
                     QString strMinBattery,
                     QString strGeological,
                     QString strHoleElevation,
                     QList<QList<QVariant> > &datas,
                     QObject *parent = 0);

protected:
    virtual void run();

signals:
    void resultReady(int value);
private:
    printFormsDemo();

    QString m_strHoleNumber;
    QString m_strDate;

    QString m_filePathOfWord;
    QString m_filenameOfDeepness;
    QString m_filenameOfTemperature;

    QList<QList<QVariant>> m_datas;
    QString m_strMaxDeepness;
    QString m_strMaxTemperature;
    QString m_strMinDeepnees;
    QString m_strMinTemperature;
    QString m_strAvgDeepnees;
    QString m_strAvgTemperature;
    QString m_strMinBattery;
    QString m_strGeological;
    QString m_strHoleElevation;

};



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
    QString filenameOfDeepness;
    QString filenameOfTemperature;

    QList<QList<QVariant>> m_datas;
    QString strMaxDeepness;
    QString strMaxTemperature;
    QString strMinDeepnees;
    QString strMinTemperature;
    QString strAvgDeepnees;
    QString strAvgTemperature;
    QString strMinBattery;
    QString strGeological;
    QString strHoleElevation;

    QChartView *m_chartView;

    void customPlotInit();
    void printReportForms();
    void saveImageOfDeepness();
    void saveImageOfTemperature();

    QVector<double> m_reportTime;
    QVector<double> m_deepness;
    QVector<double> m_temperature;
};

#endif // REPORTFORMS_H
