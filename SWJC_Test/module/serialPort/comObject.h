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

        void clear()
        {
            strPhoneNum.clear();
            strTime.clear();
            strSms.clear();
        }
    };

    struct SiteDataInfo
    {
        QString strSiteNum;
        QString strTime;
        float iDepth;
        float iTemperature;
        float iBattery;

        void clear()
        {
            strTime.clear();
            iDepth = 0;
            iTemperature = 0;
        }
    };

public:
    comObject(serialPortParam param);
    ~comObject();

    void init();

signals:
    void signalString(int index);

public slots:
    void on_slot_readMycom();

public:
    int sendAtCmd(int index, int smsCount = 0);
    void parseDataFromSerialPort();
    void parseSmsCountFromSerialPort();
    void parseAllSmsFromSerialPort();
    void parseSmsData();
    void insertDataToMysql();
private:
    serialPortParam m_serialPortParam;
    mymysql *m_mysqlDB;

    Win_QextSerialPort *myCom;
    QTimer *m_timerCom;

    QString m_qStrRecvData;
    QString m_qStrRecvDataAll;
    SmsInfo m_smsInfo;
    SiteDataInfo m_siteData;

    QStringList m_toDeleteIndexList;

    QTimer *m_timerPolling;
};

#endif // COMOBJECT_H
