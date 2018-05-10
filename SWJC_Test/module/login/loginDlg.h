#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QGridLayout>
#include <QDebug>

#include "common/domXml.h"
#include "common/mymysql.h"
#include "common/comm.h"



class LoginDlg : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDlg(QWidget *parent = 0);

    void setMysqlDB(mymysql* mysqlDB);
    QString getUserName();


public slots:
    void on_pushBtn_login();
    void on_pushBtn_cance();

private:

private:
    QLabel* labelUserName;
    QLabel* labelPassWD;

    QLineEdit* m_lineEditUserName;
    QLineEdit* m_lineEditPassWD;

    QPushButton* pushBtnLogin;
    QPushButton* pushBtnCance;

    bool m_usernameIsChange;
    QString m_username;
    QString m_passwd;

    mymysql* m_mysqlDB;
    QList<QVariant> m_userData;
};

#endif // LOGINDLG_H
