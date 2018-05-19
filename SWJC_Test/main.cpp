#include <QApplication>
#include "mainWidget.h"

#include "common/comm.h"
#include "common/st_logger.h"
#include "common/mymysql.h"
#include "module/login/loginDlg.h"

st_logger g_logger;
void stMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    g_logger.MessageOutput(type, context, msg);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //注册日志接口函数
    qInstallMessageHandler(stMessageOutput);

    g_logger.setLogLevel(LOG_INFO);

    //连接数据库
    mymysql mysqlDB;
    if (-1 == mysqlDB.sql_connect("127.0.0.1", "root", "", "mydata"))
    {
        printLog(LOG_ERROR,"connect mysql db failed!");
    }

    for(int i = 0 ; i < 1000 ; ++i)
        printLog(LOG_INFO, "connect mysql db successed.");


    //login system
    LoginDlg loginSys;
    loginSys.setMysqlDB(&mysqlDB);
    if(loginSys.exec() == QDialog::Accepted)
    {
        mainWidget w;
        w.setMySqlDB(&mysqlDB);
        w.show();

        return a.exec();
    }
    return -1;
}
