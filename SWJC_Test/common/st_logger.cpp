#include "st_logger.h"
#include <QTextStream>

st_logger::st_logger(QObject *parent) :
    QObject(parent)
{
    m_pLogFile          = NULL;
    m_bUseLogFile       = true;
    m_nLogLevel         = LOGLEVEL_DEBUG;
    m_nMaxFileSize      = 50*1024*1024; //50M
}


void st_logger::setMaxFileSize(int nSize)
{
    if (nSize >=1024*1024 && nSize <=1024*1024*1024)
        m_nMaxFileSize = nSize;
}


int st_logger::maxFileSize() const
{
    return m_nMaxFileSize;
}


int st_logger::logLevel() const
{
    return m_nLogLevel;
}


void st_logger::setLogLevel(int lv)
{
    if (lv >=LOGlEVEL_ERROR && lv <= LOGLEVEL_DEBUG)
    {
        m_nLogLevel = lv;
    }
}


bool st_logger::CreateNewLogFile(QCoreApplication * app)
{
    bool res = false;
    QDateTime dtmCur = QDateTime::currentDateTime();
    m_currLogFileName = app->applicationDirPath() + "/log/";// + dtmCur.toString("yyyy_MM") + "/";

    QDir dir;
    dir.mkpath(m_currLogFileName);
    m_currLogFileName += dtmCur.toString("yyyyMMddhhmmss_");
    m_currLogFileName += /*app->applicationName() + */QString("(%1).log").arg(app->applicationPid());

    if (NULL != m_pLogFile)
    {
        if (true == m_pLogFile->isOpen())
        {
            m_pLogFile->close();
        }
        m_pLogFile->deleteLater();
        m_pLogFile = NULL;
    }

    m_pLogFile = new QFile(m_currLogFileName, this);
    if (NULL != m_pLogFile)
    {
        if (false == m_pLogFile->open(QIODevice::WriteOnly))
        {
            m_pLogFile->deleteLater();
            m_pLogFile = NULL;
        }
        else
        {
            res = true;
        }
    }
    return res;
}


void st_logger::MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)
    //3:debug 2:info 1:warning 0:error
    m_mutextLogger.lock();
    switch (type)
    {
    case QtDebugMsg:
        if (m_nLogLevel < 3)
        {
            m_mutextLogger.unlock();
            return;
        }
        break;
    case QtInfoMsg:
        if (m_nLogLevel < 2)
        {
            m_mutextLogger.unlock();
            return;
        }
        break;
    case QtWarningMsg:
        if (m_nLogLevel < 1)
        {
            m_mutextLogger.unlock();
            return;
        }
        break;
    case QtCriticalMsg:
    case QtFatalMsg:
        if (m_nLogLevel < 0)
        {
            m_mutextLogger.unlock();
            return;
        }
        break;

    default:
        return;
    }

    if (NULL == m_pLogFile)
    {
        if (m_bUseLogFile)
        {
            m_bUseLogFile = CreateNewLogFile(QCoreApplication::instance());
        }

        if (NULL == m_pLogFile)
        {
            m_mutextLogger.unlock();
            return;
        }
    }


    QString strMessageLevel;
    switch (type)
    {
    case QtDebugMsg:
        strMessageLevel = QString("DEBUG");
        break;
    case QtInfoMsg:
        strMessageLevel = QString("INFO ");
        break;
    case QtWarningMsg:
        strMessageLevel = QString("WARN ");
        break;
    case QtCriticalMsg:
    case QtFatalMsg:
        strMessageLevel = QString("ERROR");
        break;
    default:
        return;
    }

    // 设置输出信息格式
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString strMessage = QString("%1 [%2] >>> %3")
            .arg(strDateTime)
            .arg(strMessageLevel)
//            .arg(context.file)
//            .arg(context.line)
//            .arg(context.function)
            .arg(msg.toLocal8Bit().constData());

    QTextStream stream(m_pLogFile);
    stream << strMessage << "\r\n";
    stream.flush();
    if (m_pLogFile->pos() >= m_nMaxFileSize)
    {
        m_bUseLogFile = CreateNewLogFile(QCoreApplication::instance());
    }
    m_mutextLogger.unlock();
}

