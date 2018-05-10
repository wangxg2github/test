#include "loginDlg.h"



LoginDlg::LoginDlg(QWidget *parent) : QDialog(parent)
{
    m_mysqlDB = NULL;
    m_usernameIsChange = true;
    setWindowTitle("登录");

    labelUserName = new QLabel("用户名");
    labelPassWD = new QLabel("密码");

    m_lineEditUserName = new QLineEdit;

    m_lineEditPassWD = new QLineEdit;
    m_lineEditPassWD->setEchoMode(QLineEdit::Password);

    //提示语
    m_lineEditUserName->setPlaceholderText("用户名");
    m_lineEditPassWD->setPlaceholderText("密码");


    m_lineEditUserName->setText("admin");
    m_lineEditPassWD->setText("adminPassward");

    pushBtnLogin = new QPushButton("登陆");
    pushBtnCance = new QPushButton("取消");


    QGridLayout *layout1 = new QGridLayout(this);//重点提示，构造函数需要写this
    layout1->addWidget(labelUserName, 0, 0);
    layout1->addWidget(m_lineEditUserName, 0, 1);

    layout1->addWidget(labelPassWD, 1, 0);
    layout1->addWidget(m_lineEditPassWD, 1, 1);


    layout1->addWidget(pushBtnLogin, 2, 0);
    layout1->addWidget(pushBtnCance, 2, 1);

    layout1->setColumnStretch(0, 1);//设置0列
    layout1->setColumnStretch(1, 1);//设置1列

    layout1->setMargin(20);//设置layout边距
    layout1->setSpacing(15);//设置layout当中控件和控件之间的间距.
    layout1->setSizeConstraint(QLayout::SetFixedSize);//设置对话框大小不可改变



    //设置背景图片
//    this->setAutoFillBackground(true);
//    QPalette palette;
//    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/icon/image/2.jpg")));
//    setPalette(palette);

    connect(pushBtnLogin, &QPushButton::clicked, this, &LoginDlg::on_pushBtn_login);
    connect(pushBtnCance, &QPushButton::clicked, this, &LoginDlg::on_pushBtn_cance);

}

void LoginDlg::setMysqlDB(mymysql* mysqlDB)
{
    if(NULL == mysqlDB)
    {
        printLog(LOG_ERROR, "mysql db is null!");
    }
    m_mysqlDB = mysqlDB;
    return;
}


QString LoginDlg::getUserName()
{
    return m_username;
}

void LoginDlg::on_pushBtn_login()
{
    QString username = m_lineEditUserName->text();
    QString password = m_lineEditPassWD->text();

    if (m_username.isEmpty() || m_username != username)
    {
        QList<QList<QVariant>> data;
        QString SQL = QString("select * from sys_user_info where username='%1';").arg(username);
        if (-1 == m_mysqlDB->sql_open(SQL.toUtf8().data(), data))
        {
            printLog(LOG_ERROR, "query mysql failed! SQL:%s", SQL.toUtf8().data());
            return;
        }

        if (data.size() == 0)
        {
            m_lineEditPassWD->clear();
            //提示语
            m_lineEditUserName->setPlaceholderText("用户名");
            m_lineEditPassWD->setPlaceholderText("密码");
            return;
        }

        m_userData.clear();
        m_userData = data.at(0);
        m_username = m_userData.at(1).toString();
        m_passwd = MyHelper::getMd5(m_userData.at(2).toString());

        printLog(LOG_INFO, "username:%s, password:%s.", m_username.toUtf8().data(), m_passwd.toUtf8().data());
    }

    QString passwordMD5 = MyHelper::getMd5(password);
    if (passwordMD5 != m_passwd)
    {
        //lineEditUserName->clear();
        m_lineEditPassWD->clear();
        //提示语
        m_lineEditUserName->setPlaceholderText("用户名");
        m_lineEditPassWD->setPlaceholderText("密码");

        printLog(LOG_INFO, "username:%s, password:%s.", username.toUtf8().data(), passwordMD5.toUtf8().data());
        return;
    }
    accept();
    return;
}

void LoginDlg::on_pushBtn_cance()
{
    close();
}
