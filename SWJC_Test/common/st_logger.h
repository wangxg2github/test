#ifndef ST_LOGGER_H
#define ST_LOGGER_H

#include <QObject>
#include <QDateTime>
#include <QDir>
#include <QByteArray>
#include <QMutex>
#include <QCoreApplication>
#include "common/comm.h"

class st_logger : public QObject
{
    Q_OBJECT

public:
    explicit st_logger(QObject *parent = 0);

    /**
     * @brief MessageOutput
     * @param type
     * @param context
     * @param msg
     */
    void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    /**
     * @brief 每个日志文件最大大小
     * @return
     */
    int maxFileSize() const;

    /**
     * @brief 当前日志输出级别
     * @return 当前日志级别
     */
    int logLevel() const;

public slots:
    /**
     * @brief 设置日志文件的大小
     * @param nSize：文件大小值
     */
    void setMaxFileSize(int nSize);

    /**
     * @brief 设置日志级别
     * @param lv：日志级别
     */
    void setLogLevel(int lv);


private:
    /**
     * @brief 创建一个新的日志文件
     * @param app
     * @return：true，创建成功；false，创建失败
     */
    bool CreateNewLogFile(QCoreApplication * app);

    QFile*      m_pLogFile;
    bool        m_bUseLogFile;
    QString     m_currLogFileName;
    int         m_nLogLevel;
    int         m_nMaxFileSize;
    QMutex      m_mutextLogger;
};

#endif // ST_LOGGER_H
