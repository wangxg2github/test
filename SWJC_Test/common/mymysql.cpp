#include <string.h>
#include "mymysql.h"
#include <QMessageBox>


//所有有关mysql API函数调用，都是在mymysql这个类里面实现的

//建这个类的目的是实现封装，封装所有和mysql相关函数，所有和mysql相关的函数，都放到mymysql这个类里面
mymysql::mymysql()
{
    mysql_init(&mysql);
    connection = NULL;
    isLoginMySQL = false;
    memset(buf, 0, sizeof(buf));
}


mymysql::~mymysql()
{
    if (connection)
    {
        mysql_close(connection);
        connection = NULL;
        isLoginMySQL = false;
    }
}



int mymysql::sql_connect(const char *Hostname, const char *User, const char *Password, const char *DBName)
{
    connection = mysql_real_connect(&mysql, Hostname, User, Password, DBName, 0, 0, 0);
    if (connection == NULL)
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mysql_error(&mysql));
        //QMessageBox::information(0, "", mysql_error(&mysql));
        return -1;
    }
    else
    {
        isLoginMySQL = true;
        mysql_query(connection, "SET NAMES UTF8;");
        //mysql_query(connection, QString("USE %1;").arg(*DBName));
        return 0;
    }

}


const char *mymysql::geterror()
{
    return buf;
}


void mymysql::sql_disconnet()
{
    if (connection)
    {
        mysql_close(connection);
        connection = NULL;
        isLoginMySQL = false;
    }
}


int mymysql::sql_exec(const char *SQL)
{
    if (NULL == SQL)
        return -1;
    if (mysql_query(connection, SQL) != 0)
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mysql_error(&mysql));
        return -1;
    }
    return 0;
}


int mymysql::sql_open(const char *SQL, QStandardItemModel **p)
{
    if (mysql_query(connection, SQL) != 0)
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mysql_error(&mysql));
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(connection);
    if (result == NULL)//没有查询结果
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mysql_error(&mysql));
        return -1;
    }

    int rowcount = mysql_affected_rows(connection);//这个函数返回SQL语句执行后又多少行
    int fieldcount = mysql_field_count(connection);//这个函数返回SQL语句执行后有多少列

    *p = new QStandardItemModel(rowcount, fieldcount);//根据SQL语句返回的行列总数，动态的建一个modul出来.
    MYSQL_FIELD *field;

    int i = 0;
    int j = 0;
    for(i = 0;i<fieldcount;i++)
    {
        field = mysql_fetch_field(result);
        (*p)->setHeaderData(i, Qt::Horizontal, field->name);
    }


    for(i = 0;i<rowcount;i++)//循环遍历每一行
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        for(j = 0;j<fieldcount;j++)//循环遍历一行当中所有的列
        {
            (*p)->setData((*p)->index(i, j, QModelIndex()), row[j]);
        }
    }


    mysql_free_result(result);//释放通过mysql_store_result函数分配的内存空间
    return 0;
}


int mymysql::sql_open(const char *SQL, QList<QList<QVariant>> &data)
{
    if (!isLoginMySQL)
    {
        return -1;
    }
    if (mysql_query(connection, SQL) != 0)
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mysql_error(&mysql));
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(connection);
    if (result == NULL)//没有查询结果
    {
        memset(buf, 0, sizeof(buf));
        strcpy(buf, mysql_error(&mysql));
        return -1;
    }

    int rowcount = mysql_affected_rows(connection);//这个函数返回SQL语句执行后又多少行
    int fieldcount = mysql_field_count(connection);//这个函数返回SQL语句执行后有多少列
    MYSQL_FIELD *field;

    int i = 0;
    int j = 0;
    for(i = 0; i < rowcount; i++)//循环遍历每一行
    {
        MYSQL_ROW row = mysql_fetch_row(result);
        QList<QVariant> rowList;
        for(j = 0;j<fieldcount;j++)//循环遍历一行当中所有的列
        {
            //(*p)->setData((*p)->index(i, j, QModelIndex()), row[j]);

            rowList.push_back(row[j]);
        }
        data.push_back(rowList);
    }


    mysql_free_result(result);//释放通过mysql_store_result函数分配的内存空间
    return 0;
}
