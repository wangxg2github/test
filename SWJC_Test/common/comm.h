#ifndef COMM_H
#define COMM_H
#include <QDebug>
#include <QSettings>
#include <QCryptographicHash>
#include "myHelper.h"

#define cout qDebug() << __FILE__ << ":" << __LINE__ << ">>>"

#define printLog(logLevel, fmt, args...) \
{\
    MyHelper::applicationLog(__FILE__, __LINE__, logLevel, fmt, ##args); \
}



#define RET_OK              -1
#define RET_ERR_U8          0xFF
#define RET_ERR_U16         0xFFFF
#define RET_ERR_U32         0xFFFFFFFF
#define RET_ERR_U64         0xFFFFFFFFFFFFFFFF




struct serialPortParam
{
    QString strCom;
    int bundRate;
    int dataBits;
    int stopBits;
    int checkOut;
};




enum SIGNAL_FOR_SERIALPORT
{
    SIGNAL_COM_OPEN_OK,
    SIGNAL_COM_OPEN_ERROR,
    SIGNAL_AT_CONNECT_OK,
    SIGNAL_AT_CONNECT_ERROR,
    SIGNAL_UPDATE_DATA,
    SIGNAL_AT_BUFF
};


#endif // COMM_H
