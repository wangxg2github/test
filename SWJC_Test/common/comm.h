#ifndef COMM_H
#define COMM_H
#include <QDebug>
#include <QCryptographicHash>



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





#define printLog(logLevel, fmt, args...) \
{\
    MyHelper::applicationLog(__FILE__, __LINE__, logLevel, fmt, ##args); \
    }

enum
{
    LOG_ERROR = 0x11,
    LOG_DEBUG = 0x22,
    LOG_WARN  = 0x33,
    LOG_INFO  = 0x44
};


class MyHelper : public QObject
{
public:

    /**
     * @brief getMd5
     * @param data
     * @return
     */
    static QString getMd5(const QString data)
    {
        QString pwd = data + "1qazCDE#";
        QString md5;
        QByteArray bb;
        bb = QCryptographicHash::hash ( pwd.toLatin1(), QCryptographicHash::Md5 );
        return md5.append(bb.toHex());
    }

    /*
    /*******************************************************************************************
     *  函数名称: getWinCom
     * 函数功能：获取串口键值或者键名
     * 输入参数：
     *      index : 索引号
     *      isGetKeyValue:
     *          true  -> 获取键值
     *          false -> 获取键名
     * 输出参数：
     *      outKey : 返回获取到的键名或者键值
     * 返回值：
     *      0 ： 成功
     *      -1： 失败
     *******************************************************************************************/
//    static int mainWidget::getWinCom(bool isGetKeyValue, QStringList &outKeyList)
//    {
//        isStart = true;
//        QString path = "HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM";
//        QSettings *settings = new QSettings(path, QSettings::NativeFormat);
//        QStringList key = settings->allKeys();

//        /* 取串口名 */
//        for (int index = 0; index < key.size(); ++index)
//        {
//            int ret = 0;
//            QString t_outKeyName;//键名
//            QString t_outKeyValue;//键值

//            HKEY t_hKey;
//            //LPCWSTR m_subkey;
//            if(0 != ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &t_hKey))
//            {
//                //无法打开注册表时返回error
//                QString error="Can`t open regedit!";
//                return -1;
//            }


//            wchar_t t_keyName[256] = {0}; //键名数组
//            DWORD t_keyNameSize = sizeof(t_keyName);

//            char t_keyValue[256] = {0};  //键值数组
//            DWORD t_keyValueSize = sizeof(t_keyValue);
//            DWORD t_type;

//            //列举键名和值
//            if(0 == ::RegEnumValue(t_hKey, index, t_keyName, &t_keyNameSize, 0, &t_type, (BYTE*)t_keyValue, &t_keyValueSize))
//            {
//                //读取键名
//                if (false == isGetKeyValue)
//                {
//                    for(unsigned int i = 0; i < t_keyNameSize; i++)
//                    {
//                        t_outKeyName.append(t_keyName[i]);
//                    }

//                    outKeyList << t_outKeyName;
//                }


//                //读取键值
//                if (true == isGetKeyValue)
//                {
//                    for(unsigned int j = 0; j < t_keyValueSize; j++)
//                    {
//                        if(0x00 != t_keyValue[j])
//                        {
//                            t_outKeyValue.append(t_keyValue[j]);
//                        }
//                    }

//                    outKeyList << t_outKeyValue;
//                }
//            }
//            else
//            {
//                ret = -1;
//                outKeyList = "NOKEY";
//            }
//        }

//        ::RegCloseKey(t_hKey);
//        return ret;
//    }

//*/

    /**
     * @brief applicationLog
     * @param file
     * @param line
     * @param logLevel
     * @param fmt
     */
    static void applicationLog(const char *file, const unsigned int line, unsigned int logLevel, const char *fmt, ...)
    {
        va_list args;
        char printBuf[512] = {0};
        va_start(args, fmt);
        int bufLen = vsprintf(printBuf, fmt, args);
        va_end(args);

        if(bufLen <= 0 || bufLen >= 512)
        {
            return;
        }

        char printLogBuf[1024] = {0};
        sprintf(printLogBuf, "[%s:%u] %s", file, line, printBuf);

        switch (logLevel)
        {
        case LOG_ERROR:
            qCritical(printLogBuf);
            break;
        case LOG_WARN:
            qWarning(printLogBuf);
            break;
        case LOG_INFO:
            qInfo(printLogBuf);
            break;
        case LOG_DEBUG:
            qDebug(printLogBuf);
            break;
        default:
            break;
        }
        return;
    }
};
#endif // COMM_H
