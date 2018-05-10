#ifndef COMOBJECT_H
#define COMOBJECT_H

#include <QTimer>
#include <QObject>
#include "common/comm.h"
#include "common/mymysql.h"
#include "module/serialPort/qextserialbase.h"
#include "module/serialPort/win_qextserialport.h"


#define MYCOM_EVENTDRIVEN




class comObject : public QObject
{
    Q_OBJECT

    struct SmsInfo
    {
        QString strPhoneNum;
        QString strTime;
        QString strSms;
    };

    struct SiteDataInfo
    {
        QString strSiteNum;
        QString strTime;
        int iDepth;
        int iTemperature;
        int iBattery;
    };

public:
    comObject(serialPortParam param);

    void init();

signals:

public slots:
    void on_slot_readMycom();

public:
    int sendAtCmd(int index);
    void parseDataFromSerialPort();
    void parseSmsData();
    void insertDataToMysql();
private:
    serialPortParam m_serialPortParam;
    mymysql *m_mysqlDB;

    Win_QextSerialPort *myCom;
    QTimer *m_timerCom;

    QString m_qStrRecvData;
    SmsInfo m_smsInfo;
    SiteDataInfo m_siteData;
};

#endif // COMOBJECT_H
