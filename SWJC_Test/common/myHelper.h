#ifndef MYHELPER_H
#define MYHELPER_H

enum LOG_LEVEL
{
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOGlEVEL_BUFF
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

    /**
     * @brief applicationLog
     * @param file
     * @param line
     * @param logLevel
     * @param fmt
     */
    static void applicationLog(const char *file, const unsigned int line, unsigned int logLevel, const char* fmt, ...)
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

#endif // COMMFUNC_H
