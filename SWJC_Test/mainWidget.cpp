#include "mainWidget.h"
#include "ui_mainWidget.h"

mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    m_mainWidgetUI(new Ui::mainWidget)
{
    m_mainWidgetUI->setupUi(this);

    m_excelExport = NULL;
    m_comThread = NULL;
    m_comObject = NULL;

    QString path = "HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM";
    QSettings *settings = new QSettings(path, QSettings::NativeFormat);
    QStringList key = settings->allKeys();
    QString comValue ;

    /* 取串口名 */
    for (int i = 0; i < key.size(); ++i)
    {
        getWinCom(i, "value", comValue);
        m_mainWidgetUI->comBox_serialPort->addItem(comValue);
    }
    if(0 != key.size())
        m_mainWidgetUI->pushBtn_serialPort->setText("开始通信");

    m_mainWidgetUI->comBox_baudRate->setCurrentIndex(12);
    m_mainWidgetUI->comBox_Data->setCurrentIndex(3);
    m_mainWidgetUI->comBox_stop->setCurrentIndex(0);
    m_mainWidgetUI->comBox_parity->setCurrentIndex(0);

    mainWidgetInit();
    systemTrayInit();
    settingInit();


    //threadStart();

}

mainWidget::~mainWidget()
{
    threadStop();
    threadDestroy();
    delete m_mainWidgetUI;
}

void mainWidget::setMySqlDB(mymysql *mySqlDB)
{
    if (mySqlDB == NULL)
    {
        printLog(LOG_ERROR, "mysql db is null!");
        return;
    }
    m_mysqlDB = mySqlDB;
    return;
}


void mainWidget::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    switch (event->type())
    {
    case QEvent::WindowStateChange:
        if (this->isMinimized() == true)
        {
            this->hide();
            //m_systemTrayIcon->show();
        }
        break;
    default:
        break;
    }
    return;
}

void mainWidget::closeEvent(QCloseEvent *event)
{
    if (this->isVisible() == true)
    {
        this->hide();
        m_systemTrayIcon->show();
        this->m_systemTrayIcon->showMessage(tr("系统状态"),
                                      tr("系统正在后台运行。"));
        event->ignore();
    }
    else
    {
//        QMessageBox::StandardButton exitBtn = QMessageBox::question(this, "提示",
//                                                                    "是否退出",
//                                                                    QMessageBox::Yes | QMessageBox::No);
//        if (exitBtn == QMessageBox::Yes)
//        {
//            threadStop();
//            event->accept();
//        }
//        else
//        {
//            event->ignore();
//        }
        event->accept();
        //threadStop();
//        this->m_systemTrayIcon->showMessage(tr("Server is  going to closed"),
//                                      tr("Waiting for all unfinished progress..."));
    }

}

bool mainWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == (QObject *)m_mainWidgetUI->widgetTitle )
    {
        //双击最大化/恢复窗口
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            if (this->property("canMoveAndMax").toBool())
            {
                this->showMaximized();
                this->setProperty("canMoveAndMax", false);

            }
            else
            {
                this->showNormal();
                this->setProperty("canMoveAndMax", true);
            }
            return true;
        }
    }

    QWidget *w = (QWidget *)this->window();
    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *mouseevent = static_cast<QMouseEvent *>(event);
    if (mouseevent->type() == QEvent::MouseButtonPress)
    {
        if (mouseevent->button() == Qt::LeftButton)
        {
            mousePressed = true;
            mousePoint = mouseevent->globalPos() - w->pos();
            return true;
        }
    }
    else if (mouseevent->type() == QEvent::MouseButtonRelease)
    {
        mousePressed = false;
        return true;
    }
    else if (mouseevent->type() == QEvent::MouseMove)
    {
        if (mousePressed && (mouseevent->buttons() && Qt::LeftButton))
        {
            w->move(mouseevent->globalPos() - mousePoint);
            return true;
        }
    }

    return QWidget::eventFilter(obj, event);
}


void mainWidget::on_getWidgetTraySignal(QSystemTrayIcon::ActivationReason reason)
{
#if 0
    if (QSystemTrayIcon::Trigger == reason)
    {
        m_systemTrayIcon->showMessage("Message Title",
                                  "欢迎使用此程序",
                                  QSystemTrayIcon::Information,
                                  1000);
    }
#endif
    if (QSystemTrayIcon::DoubleClick == reason)
    {
        m_systemTrayIcon->hide();
        this->showMaximized();
    }
    return;
}


void mainWidget::mainWidgetInit()
{
    //this->resize(800, 600);
    this->showMaximized();
    this->setProperty("canMoveAndMax", true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint /* | Qt::WindowMinMaxButtonsHint*/);

    m_mainWidgetUI->lab_Name->setText("水文监测系统");
    m_mainWidgetUI->lab_Name->setFont(QFont("Microsoft Yahei", 20));
    this->setWindowTitle(m_mainWidgetUI->lab_Name->text());

    //m_mainWidgetUI->stackedWidget->setStyleSheet("QLabel{font:60pt;}");

    //安装事件过滤器
    m_mainWidgetUI->widgetTitle->installEventFilter(this);
    //this->installEventFilter(this);

    //设置顶部导航按钮
    QSize icoSize(25, 25);
    int icoWidth = 65;

    QList<QToolButton *> tbtns = m_mainWidgetUI->widgetTitleMenu->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns)
    {
        btn->setIconSize(icoSize);
        btn->setMinimumWidth(icoWidth);
        btn->setCheckable(true);
        connect(btn, &QToolButton::clicked, this, &mainWidget::on_toolBtnAll_clicked);
    }
    connect(m_mainWidgetUI->pushBtn_Min, &QPushButton::clicked, this,
            [=]()
    {
        showMinimized();
    });
    connect(m_mainWidgetUI->pushBtn_Close, &QPushButton::clicked, this,
            [=]()
    {
        close();
    });

}

void mainWidget::systemTrayInit()
{
    m_actionShowWidget = new QAction("显示界面", this);
    connect(m_actionShowWidget, &QAction::triggered,
            [this]()
    {
        this->showMaximized();
    });

    m_actionExit = new QAction("退出程序", this);
    connect(m_actionExit, &QAction::triggered,
            [this]()
    {
        this->close();
    });

    m_menuSystemTray = new QMenu(this);
    m_menuSystemTray->addAction(m_actionShowWidget);
    m_menuSystemTray->addSeparator();
    m_menuSystemTray->addAction(m_actionExit);

    m_systemTrayIcon = new QSystemTrayIcon(this);
    m_systemTrayIcon->setContextMenu(m_menuSystemTray);

    //新建托盘要显示的icon
    QIcon icon = QIcon(":/resource/image/Color_Classic.png");
    //m_systemTrayIcon
    m_systemTrayIcon->setIcon(icon);
    //当鼠标移动到托盘上的图标时，会显示此处设置的内容
    m_systemTrayIcon->setToolTip("水文监测系统");

    connect(m_systemTrayIcon, &QSystemTrayIcon::activated, this, &mainWidget::on_getWidgetTraySignal);

    m_mainWidgetUI->toolBtn_Main->clicked();
}

void mainWidget::settingInit()
{
    m_myModelUserInfo = NULL;
    m_tableViewUserSettingIsShow = false;
    m_userDatas.clear();
    m_userHeaderData << "序号" << "用户名" << "密码" << "创建时间" << "账户有效时长" << "管理员权限";


    m_myModelEquipmentData = NULL;
    m_tableViewEquipmentShow = false;
    m_equipmentDatas.clear();
    m_equipmentDataHeader << "孔口编号" << "孔口坐标X" << "孔口坐标Y" << "孔口坐标Z" << "孔口标高(m)" << "孔口深度(m)" << "监测含水位层" << "初始水位标高(m)" << "分站编号" << "分站卡号" << "分站状态" << "传感器线长(mm)" << "传感器校准值(m)" << "分站安装位置";


    m_mymodelRealTimeData = NULL;
    m_tableViewRealTimeShow = false;
    m_realTimeDatas.clear();
    m_realTimeDataHeader << "孔口编号" << "孔口标高(m)" << "监测含水层位" << "监测时间" << "水位埋深(m)" << "水位标高(m)" << "水温(℃)" << "电池电压(V)" << "分站编号" << "分站卡号" << "分站状态" ;//<< "数据获取源";



    m_myModelQueryData = NULL;
    m_tableViewQueryDataIsShow = false;
    m_queryDatas.clear();
    m_queryDataHeader << "钻孔编号" << "检测含水层" << "监测时间" << "水位埋深(m)" << "水位标高(m)" << "水温(℃)" << "电池电压(V)"<< "分站编号" << "分站卡号" << "分站状态" << "安装位置";





    //设置界面 用户管理菜单单击事件
    connect(m_mainWidgetUI->toolBtn_userSetting, &QToolButton::clicked, this,
            [=]()
    {
        m_mainWidgetUI->stackedWidget_2->setCurrentIndex(0);

        if (false == m_tableViewUserSettingIsShow)
        {
            m_tableViewUserSettingIsShow = true;
            m_mainWidgetUI->stackedWidget_2->setCurrentIndex(0);



            QString strSql = "select id, username, passwd, create_date, effective_time, authority from sys_user_info;";
            if (NULL == m_myModelUserInfo)
            {
                m_myModelUserInfo = new QMyModel(this);
            }
            showData(m_mainWidgetUI->tableView_UserSetting, m_myModelUserInfo, &m_userDatas, m_userHeaderData, strSql);

            m_mainWidgetUI->tableView_UserSetting->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

            cout << m_mainWidgetUI->tableView_UserSetting->width();


            //m_mainWidgetUI->tableView_UserSetting->resize
        }
        return;
    });


    //设置界面 分站管理
    connect(m_mainWidgetUI->toolBtn_equipment, &QToolButton::clicked, this,
            [=]()
    {
        m_mainWidgetUI->stackedWidget_2->setCurrentIndex(1);

        if (false == m_tableViewEquipmentShow)
        {
            m_tableViewEquipmentShow = true;
            m_mainWidgetUI->stackedWidget_2->setCurrentIndex(1);


            QString strSql = "select hole_number, hole_coordinate_X, hole_coordinate_Y, hole_coordinate_Z,\
                    hole_elevation, hole_depth, geological, water_elevation, \
                    site_number, site_phone, site_status, line_lenght, calibration, location from cdr_site_info;";

            if (NULL == m_myModelEquipmentData)
            {
                m_myModelEquipmentData = new QMyModel(this);
            }
            showData(m_mainWidgetUI->tableView_equipment, m_myModelEquipmentData, &m_equipmentDatas, m_equipmentDataHeader, strSql);

            //隐藏某一列数据
//            m_mainWidgetUI->tableView_equipment->hideColumn(1);
//            m_mainWidgetUI->tableView_equipment->hideColumn(2);
//            m_mainWidgetUI->tableView_equipment->hideColumn(3);
//            m_mainWidgetUI->tableView_equipment->hideColumn(12);
            m_mainWidgetUI->tableView_equipment->setColumnWidth(13, 350);
            return;
        }
    });

    connect(m_mainWidgetUI->pushBtn_addEquipment, &QPushButton::clicked, this,
            [=]()
    {
        equipmentDialog dlg(true);
        int res = dlg.exec();
        if (res == QDialog::Accepted)
        {
            QStringList strListData = dlg.getData();

            QString strSql = QString("insert cdr_site_info(hole_number, hole_coordinate_X, hole_coordinate_Y, hole_coordinate_Z,\
                             hole_elevation, hole_depth, geological, water_elevation, \
                             site_number, site_phone, site_status, line_lenght, calibration, location) \
             values('%1', '%2',  '%3', '%4', %5, %6,  '%7', %9, '%10', '%11', '%12', %13, %14, '%15');").arg(strListData.at(0)).arg(strListData.at(1)).arg(strListData.at(2)).arg(strListData.at(3)).arg(strListData.at(4)).arg(strListData.at(5)).arg(strListData.at(6)).arg(strListData.at(7)).arg(strListData.at(8)).arg(strListData.at(9)).arg(strListData.at(10)).arg(strListData.at(11)).arg(strListData.at(12)).arg(strListData.at(13));

            if (-1 == m_mysqlDB->sql_exec(strSql.toUtf8().data()))
            {
                printLog(LOG_ERROR, "Add information of site failed! strSql:%s.", strSql.toUtf8().data());
                return;
            }

            QList<QVariant> listData;
            QStringList strDataList = dlg.getData();
            for(int index = 0; index < strDataList.size(); ++index)
            {
                listData.append(strDataList.at(index));
            }
            m_equipmentDatas.append(listData);
            m_myModelEquipmentData->upData();
        }
        return;
    });


    connect(m_mainWidgetUI->pushBtn_delEquipment, &QPushButton::clicked, this,
            [=]()
    {
        QModelIndex index = m_mainWidgetUI->tableView_equipment->currentIndex();

        if (false == index.isValid())
        {
            QMessageBox::information(NULL, "提醒", "请选择一条要删除的数据", QMessageBox::Yes, QMessageBox::Yes);
        }
        else
        {
            QString strSiteNumber = m_equipmentDatas.at(index.row()).at(8).toString();
            cout << strSiteNumber;
            if (QMessageBox::Yes == QMessageBox::question(NULL, "确定", "确定删除？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
            {
                QString strSql = QString("delete from cdr_site_info where site_number='%1';").arg(strSiteNumber);
                m_mysqlDB->sql_exec(strSql.toUtf8().data());
                m_equipmentDatas.removeAt(index.row());
                m_myModelEquipmentData->upData();
            }

        }
        return;
    });
    connect(m_mainWidgetUI->pushBtn_editEquipment, &QPushButton::clicked, this,
            [=]()
    {
        QModelIndex index = m_mainWidgetUI->tableView_equipment->currentIndex();

        if (false == index.isValid())
        {
            QMessageBox::information(NULL, "提醒", "请选择一条要修改的数据", QMessageBox::Yes, QMessageBox::Yes);
        }
        else
        {
            int currentRow = index.row();
            QList<QVariant> dataRow = m_equipmentDatas.at(currentRow);
            QStringList dataList;
            for(int i = 0; i < dataRow.size(); ++i)
            {
                dataList << dataRow.at(i).toString();
            }
            cout << dataList;

            equipmentDialog dlg(false);
            dlg.setData(dataList);

            int res = dlg.exec();
            if (res == QDialog::Accepted)
            {
                dataRow.clear();
                dataList.clear();
                dataList = dlg.getData();
                for(int index = 0; index < dataList.size(); ++index)
                {
                    dataRow.append(dataList.at(index));
                }
                cout << "outData:" << dataList;

                QString strSql = QString("update cdr_site_info set hole_coordinate_X='%1', hole_coordinate_Y='%2', hole_coordinate_Z='%3', hole_elevation=%4 , hole_depth=%5 , geological='%6', water_elevation=%7 , site_number= '%8', site_phone= '%9', site_status= '%10', line_lenght= %11, calibration= %12, location='%13' where hole_number='%14';").arg(dataList.at(1)).arg(dataList.at(2)).arg(dataList.at(3)).arg(dataList.at(4).toFloat()).arg(dataList.at(5).toFloat()).arg(dataList.at(6)).arg(dataList.at(7).toFloat()).arg(dataList.at(8)).arg(dataList.at(9)).arg(dataList.at(10)).arg(dataList.at(11).toFloat()).arg(dataList.at(12).toFloat()).arg(dataList.at(13)).arg(dataList.at(0));

                if(-1 == m_mysqlDB->sql_exec(strSql.toUtf8().data()))
                {
                    QMessageBox::critical(NULL, "错误", "修改数据失败！", QMessageBox::Yes, QMessageBox::Yes);
                }
                else
                {
                    m_equipmentDatas.replace(currentRow, dataRow);
                    m_myModelEquipmentData->upData();
                }
            }
        }

    });


    //历史数据查询
    m_isShowGroupBox = false;
    m_mainWidgetUI->dateTimeEdit_start->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    m_mainWidgetUI->dateTimeEdit_end->setDisplayFormat("yyyy/MM/dd HH:mm:ss");

    m_mainWidgetUI->dateTimeEdit_start->setDateTime(QDateTime::currentDateTime().addDays(-30));
    m_mainWidgetUI->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());

//    m_mainWidgetUI->dateTimeEdit_start->setCalendarPopup(true);
//    m_mainWidgetUI->dateTimeEdit_end->setCalendarPopup(true);

    connect(m_mainWidgetUI->comBo_holeNumber, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          [=](int index)
    {
        Q_UNUSED(index);
        m_tableViewQueryDataIsShow = false;
    });
    connect(m_mainWidgetUI->dateTimeEdit_start, &QDateTimeEdit::dateTimeChanged, this,
            [=]()
    {
        m_tableViewQueryDataIsShow = false;
    });
    connect(m_mainWidgetUI->dateTimeEdit_end, &QDateTimeEdit::dateTimeChanged, this,
            [=]()
    {
        m_tableViewQueryDataIsShow = false;
    });
    connect(m_mainWidgetUI->pushBtn_show, &QPushButton::clicked, this,
            [=]()
    {
        if(m_isShowGroupBox)
        {
            m_isShowGroupBox = false;
            m_mainWidgetUI->groupBox_2->setVisible(false);
            m_mainWidgetUI->pushBtn_show->setText("<");
        }
        else
        {
            m_isShowGroupBox = true;
            m_mainWidgetUI->groupBox_2->setVisible(true);
            m_mainWidgetUI->pushBtn_show->setText(">");
        }

    });
    connect(m_mainWidgetUI->pushBtn_search, &QPushButton::clicked, this,
            [=]()
    {
        queryDataFromDB();
        //m_myModelQueryData->upData();
    });



}

void mainWidget::threadStart()
{
    serialPortParam serialPortParamTmp;
    serialPortParamTmp.strCom = m_mainWidgetUI->comBox_serialPort->currentText();
    serialPortParamTmp.bundRate = m_mainWidgetUI->comBox_baudRate->currentIndex();
    serialPortParamTmp.dataBits = m_mainWidgetUI->comBox_Data->currentIndex();
    serialPortParamTmp.stopBits = m_mainWidgetUI->comBox_stop->currentIndex();
    serialPortParamTmp.checkOut = m_mainWidgetUI->comBox_parity->currentIndex();


    //serialPort thread
    m_comObject = new comObject(serialPortParamTmp);
    m_comThread = new QThread();
    m_comObject->moveToThread(m_comThread);

    connect(m_comThread, &QThread::started, m_comObject, &comObject::init);
    connect(m_comThread, &QThread::finished, m_comObject, &comObject::deleteLater);
    connect(m_comThread, &QThread::finished, m_comThread, &QThread::deleteLater);

    //connect(m_mainWidgetUI->pushBtn_serialPort, &QPushButton::clicked, m_comObject, &comObject::on_slot_sendAT);
    connect(m_comObject, &comObject::signalString, this,
            [=](int index)
    {
        switch (index)
        {
        case SIGNAL_COM_OPEN_OK:

            break;

        case SIGNAL_COM_OPEN_ERROR:
            threadDestroy();
            m_mainWidgetUI->pushBtn_serialPort->setText("开始通信");
            QMessageBox::critical(NULL, "错误", "打开串口失败！", QMessageBox::Yes);
            break;
        case SIGNAL_AT_CONNECT_OK:
            m_mainWidgetUI->pushBtn_serialPort->setText("正在通信");
            break;
        case SIGNAL_AT_CONNECT_ERROR:
            threadDestroy();
            m_mainWidgetUI->pushBtn_serialPort->setText("开始通信");
            QMessageBox::critical(NULL, "错误", "设备连接异常！", QMessageBox::Yes);
            break;
        case SIGNAL_UPDATE_DATA:
            m_realTimeDatas.clear();
            QString strSql = "select a.hole_number, a.hole_elevation, a.geological, a.report_time, a.deepness,\
                              a.hole_elevation-a.deepness as shuiWei,\
                              a.temperature, a.battery_level, a.site_number, a.site_phone, a.site_status \
                              from cdr_data_full as a \
                              inner join \
                              ( \
                                  select site_number, max(report_time) 'maxReport_time' \
                                  from cdr_data_full  \
                                  group by site_number \
                              ) b \
                              on a.site_number=b.site_number and a.report_time=b.maxReport_time;";

            if (NULL == m_mymodelRealTimeData)
            {
                m_mymodelRealTimeData = new QMyModel(this);
            }
            showData(m_mainWidgetUI->tableView_realTimeData, m_mymodelRealTimeData, &m_realTimeDatas, m_realTimeDataHeader, strSql);

            m_mainWidgetUI->tableView_realTimeData->setColumnWidth(3, 140);
            m_mainWidgetUI->tableView_realTimeData->setColumnWidth(10, 130);
            break;

        }
    });

    m_comThread->start();
}

void mainWidget::threadStop()
{

}

void mainWidget::threadDestroy()
{
    if (m_comThread)
    {
        m_comThread->quit();
    }
    m_comThread->wait();
    m_comThread = NULL;
    m_comObject = NULL;

}

void mainWidget::queryDataFromDB()
{
    if (false == m_tableViewQueryDataIsShow)
    {

        if(0 >= m_mainWidgetUI->dateTimeEdit_start->dateTime().secsTo(m_mainWidgetUI->dateTimeEdit_end->dateTime()))
        {
            QMessageBox::warning(this, "告警", "开始时间不可以大于结束时间", QMessageBox::Yes);
            return;
        }
        m_queryDatas.clear();
        m_tableViewQueryDataIsShow = true;

        QString strQueryDateTimeStart = m_mainWidgetUI->dateTimeEdit_start->dateTime().toString("yyyyMMddhhmmss");
        QString strQueryDateTimeEnd = m_mainWidgetUI->dateTimeEdit_end->dateTime().toString("yyyyMMddhhmmss");



        QString strSql = QString("select a.hole_number, a.geological, a.report_time, a.deepness, a.hole_elevation-a.deepness as shuiWei, \
                a.temperature, a.battery_level, a.site_number, a.site_phone, a.site_status, a.location \
            from cdr_data_full as a \
            where  a.report_time>'%1' and a.report_time < '%2'").arg(strQueryDateTimeStart).arg(strQueryDateTimeEnd);

        if (0 != m_mainWidgetUI->comBo_holeNumber->currentText().compare("全部"))
        {
            strSql += QString("and a.hole_number='%1'").arg(m_mainWidgetUI->comBo_holeNumber->currentText());
        }
        strSql += "order by a.report_time desc";

        if (NULL == m_myModelQueryData)
        {
            m_myModelQueryData = new QMyModel(this);
        }
        showData(m_mainWidgetUI->tableView_query, m_myModelQueryData, &m_queryDatas, m_queryDataHeader, strSql);
        m_mainWidgetUI->tableView_query->setColumnWidth(2, 140);
        m_mainWidgetUI->tableView_query->setColumnWidth(10, 250);

        //max, min, avg
        strSql.clear();
        strSql = QString("select max(deepness), max(hole_elevation-deepness), max(temperature), min(deepness), min(hole_elevation-deepness), min(temperature), round(avg(deepness), 3), round(avg(hole_elevation-deepness), 3), round(avg(temperature), 3),  min(battery_level) from cdr_data_full  where report_time>'%1' and report_time < '%2' ").arg(strQueryDateTimeStart).arg(strQueryDateTimeEnd);
        if (0 != m_mainWidgetUI->comBo_holeNumber->currentText().compare("全部"))
        {
            strSql += QString("and hole_number='%1'").arg(m_mainWidgetUI->comBo_holeNumber->currentText());
        }

        QList<QList<QVariant>> datas;
        if (-1 == m_mysqlDB->sql_open(strSql.toLatin1().data(), datas))
        {
            printLog(LOG_ERROR, "Query data from mysql failed! strSql:%s", strSql.toLatin1().data());
            return;
        }
        if (1 == datas.size() && 10 == datas.at(0).size())
        {
            QList<QVariant> dataTmp = datas.at(0);
            m_mainWidgetUI->lineEdit_maxDeepness->setText(dataTmp.at(0).toString());
            m_mainWidgetUI->lineEdit_maxWater->setText(dataTmp.at(1).toString());
            m_mainWidgetUI->lineEdit_maxTemperature->setText(dataTmp.at(2).toString());

            m_mainWidgetUI->lineEdit_minDeepness->setText(dataTmp.at(3).toString());
            m_mainWidgetUI->lineEdit_minWater->setText(dataTmp.at(4).toString());
            m_mainWidgetUI->lineEdit_minTemperature_2->setText(dataTmp.at(5).toString());

            m_mainWidgetUI->lineEdit_avgDeepness->setText(dataTmp.at(6).toString());
            m_mainWidgetUI->lineEdit_avgWater->setText(dataTmp.at(7).toString());
            m_mainWidgetUI->lineEdit_avgTemperature->setText(dataTmp.at(8).toString());

            m_mainWidgetUI->lineEdit_minBatty->setText(dataTmp.at(9).toString());

            m_mainWidgetUI->pushBtn_show->setText(">");
            m_mainWidgetUI->groupBox_2->setVisible(true);
            m_isShowGroupBox = true;
        }
    }
    return;
}

void mainWidget::on_toolBtnAll_clicked()
{
    QToolButton *b = (QToolButton *)sender();
    QString name = b->text();

    QList<QToolButton *> tbtns = m_mainWidgetUI->widgetTitleMenu->findChildren<QToolButton *>();
    foreach (QToolButton *btn, tbtns)
    {
        if (btn == b)
        {
            btn->setChecked(true);
        }
        else
        {
            btn->setChecked(false);
        }
    }

    if (name == "主界面")
    {
        m_mainWidgetUI->stackedWidget->setCurrentIndex(0);
        //m_mainWidgetUI->stackedWidget->show();
    }
    else if (name == "分站管理")
    {
        m_mainWidgetUI->stackedWidget->setCurrentIndex(1);
        m_mainWidgetUI->widget_2->setVisible(false);

        emit m_mainWidgetUI->toolBtn_equipment->click();
    }
    else if (name == "数据查询")
    {
        m_mainWidgetUI->stackedWidget->setCurrentIndex(2);
        m_mainWidgetUI->groupBox_2->setVisible(false);

        if (m_holeNumberList.isEmpty())
        {
            QList<QList<QVariant>> datas;
            QString strSql = "select hole_number from cdr_site_info;";
            if (-1 == m_mysqlDB->sql_open(strSql.toLatin1().data(), datas))
            {
                printLog(LOG_ERROR, "Query data from mysql failed! strSql:%s", strSql.toLatin1().data());
                return;
            }

            int iRowCount = datas.size();
            for(int i = 0; i < iRowCount; ++i)
            {
                m_holeNumberList.append(datas.at(i).at(0).toString());
            }

            m_mainWidgetUI->comBo_holeNumber->addItems(m_holeNumberList);
        }
        //queryDataFromDB();
    }
    else if (name == "报表输出")
    {
        reportForms report(m_mysqlDB);
        if(QDialog::Accepted == report.exec())
        {
            cout << "accept";
            QMessageBox::information(this, "报表打印", "报表打印成功！", QMessageBox::Accepted);

        }
    }
    else if (name == "退出系统")
    {
        close();
    }

}

void mainWidget::on_pushBtn_exportExcel_clicked()
{
    QString csvFilePath = QFileDialog::getExistingDirectory(this) + "/";
    if(csvFilePath.isEmpty())
        return;

    QString dateTime = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".csv";
    csvFilePath += dateTime;

    //打开.csv文件
    QFile csvFile(csvFilePath);
    if(!csvFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        printLog(LOG_ERROR, "open file of csv failed! file path:%s.", csvFilePath);
        return;
    }

    QTextStream outStream(&csvFile);
    QString strLine;

    int row = m_queryDatas.size();
    int col = m_queryDatas.at(0).size();
    for(int i = 0; i < row; ++i)
    {
        QList<QVariant> rows = m_queryDatas.at(i);
        for(int j = 0; j < col; ++j)
        {
            strLine = rows.at(j).toString();
            outStream << strLine << ",";
        }
        outStream << "\n";
    }
    csvFile.close();
    printLog(LOG_INFO, "save file of csv successed.");
    return;
}



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
int mainWidget::getWinCom(int index, bool isGetKeyValue, QString &outKey)
{
    int ret = 0;
    QString t_outKeyName;//键名
    QString t_outKeyValue;//键值

    HKEY t_hKey;
    //LPCWSTR m_subkey;
    if(0 != ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &t_hKey))
    {
        //无法打开注册表时返回error
        QString error="Can`t open regedit!";
        return -1;
    }


    wchar_t t_keyName[256] = {0}; //键名数组
    DWORD t_keyNameSize = sizeof(t_keyName);

    char t_keyValue[256] = {0};  //键值数组
    DWORD t_keyValueSize = sizeof(t_keyValue);
    DWORD t_type;

    //列举键名和值
    if(0 == ::RegEnumValue(t_hKey, index, t_keyName, &t_keyNameSize, 0, &t_type, (BYTE*)t_keyValue, &t_keyValueSize))
    {
        //读取键名
        if (false == isGetKeyValue)
        {
            for(unsigned int i = 0; i < t_keyNameSize; i++)
            {
                t_outKeyName.append(t_keyName[i]);
            }

            outKey = t_outKeyName;
        }


        //读取键值
        if (true == isGetKeyValue)
        {
            for(unsigned int j = 0; j < t_keyValueSize; j++)
            {
                 if(0x00 != t_keyValue[j])
                 {
                     t_outKeyValue.append(t_keyValue[j]);
                 }
            }

            outKey = t_outKeyValue;
        }
    }
    else
    {
        ret = -1;
        outKey = "NOKEY";
    }

    ::RegCloseKey(t_hKey);
    return ret;
}

void mainWidget::showData(QTableView *tableView, QMyModel *myModel, QList<QList<QVariant>> *data, QStringList dataHeader, QString &strSql)
{
    if (NULL == tableView || NULL == myModel)
        return;

    if(-1 == m_mysqlDB->sql_open(strSql.toLatin1().data(), *data))
    {
        printLog(LOG_ERROR, "Query data of real failed! strSql:%s.", strSql.toLatin1().data());
        return;
    }

    myModel->setDoubleListPtr(data, dataHeader);

    tableView->setModel(myModel);
    //隐藏某一列数据
    //tableView->hideColumn(1);

    //不可编辑
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 只能单选
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    // 以行作为选择标准
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 列头隐藏
    //tableView->verticalHeader()->hide();
    // 让列头可被点击，触发点击事件
    //tableView->horizontalHeader()->setSectionsClickable(true);

    // 去掉选中表格时，列头的文字高亮
    tableView->horizontalHeader()->setHighlightSections(false);
    tableView->horizontalHeader()->setBackgroundRole(QPalette::Background);

    // 列头灰色
    tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:rgb(225,225,225)};");
    myModel->upData();
}

void mainWidget::on_pushBtn_serialPort_clicked()
{
    if (m_mainWidgetUI->pushBtn_serialPort->text() == "开始通信")
    {
        //校验COM口
        QString path = "HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM";
        QSettings *settings = new QSettings(path, QSettings::NativeFormat);
        QStringList key = settings->allKeys();
        QString comValue ;
        for (int i = 0; i < key.size(); ++i)
        {
            getWinCom(i, "value", comValue);
            if (comValue == m_mainWidgetUI->comBox_serialPort->currentText())
            {
                threadStart();
                return;
            }
        }
        m_mainWidgetUI->comBox_serialPort->clear();
        m_mainWidgetUI->pushBtn_serialPort->setText("查找COM口");
        QMessageBox::about(NULL, "提醒", "COM口无效，请检查COM！");
    }
    else if (m_mainWidgetUI->pushBtn_serialPort->text() == "查找COM口")
    {
        QString path = "HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM";
        QSettings *settings = new QSettings(path, QSettings::NativeFormat);
        QStringList key = settings->allKeys();
        QString comValue ;
        for (int i = 0; i < key.size(); ++i)
        {
            getWinCom(i, "value", comValue);
            m_mainWidgetUI->comBox_serialPort->addItem(comValue);
        }
        if (0 != key.size())
        {
            m_mainWidgetUI->pushBtn_serialPort->setText("开始通信");
        }
    }
    else
    {
        if(QMessageBox::Yes == QMessageBox::question(NULL, "提醒", "确定要停止接受数据吗？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
        {
            threadDestroy();
            m_mainWidgetUI->pushBtn_serialPort->setText("开始通信");
        }

    }
    return;
}

