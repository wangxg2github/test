#ifndef MYMYSQL_H
#define MYMYSQL_H


//windows下使用mysql.h的方法
//路径中使用/符号
#include <windows.h>
#include <D:/Software/mysql/include/mysql.h>

//linux下使用mysql.h的方法
//#include <mysql/mysql.h>

/*
int listen_st;//linux下的定义socket描述符的写法
SOCKET listen_st;//windows下的定义socket描述符的写法
*/

#include <QTableView>
#include <QStandardItemModel>

class mymysql
{
public:
    mymysql();
    ~mymysql();

    const char *geterror();

    //第一个参数代表server IP地址，第二个参数代表用户名，第三个密码，第四个使用数据库名
    int sql_connect(const char *Hostname, const char *User, const char *Password, const char *DBName);
    void sql_disconnet();
    int sql_exec(const char *SQL);
    int sql_open(const char *SQL, QStandardItemModel **p);
    int sql_open(const char *SQL, QList<QList<QVariant>> &data);


private:
    MYSQL *connection;
    MYSQL mysql;
    char buf[1024];
public:
    bool isLoginMySQL;
};

#endif // MYMYSQL_H
