#ifndef LOGINMYSQLDLG_H
#define LOGINMYSQLDLG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class LoginMySqlDlg : public QDialog
{
    Q_OBJECT
public:
    explicit LoginMySqlDlg(QWidget *parent = 0);

signals:

public slots:

private:
    QLabel *label0, *label1, *label2, *label3;
    QLineEdit *lineEditUserID, *lineEditPasswd, *lineEditDBName, *lineEditHostIP;
    QPushButton *loginBtn, *logoutBtn;

    //slots:
    void on_loginBtnClick();
    void on_logoutBtnClick();
    void loginBtnOnclick();
    void logoutBtnOnclick();

public:
    QString userid;
    QString passwd;
    QString dbname;
    QString hostip;
    bool islogin;

};

#endif // LOGINDLG_H
