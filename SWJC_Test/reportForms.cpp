#include "reportForms.h"
#include "ui_reportForms.h"


reportForms::reportForms(mymysql *mysqlDB) :
    ui(new Ui::reportForms)
{
    ui->setupUi(this);

    ui->customPlot->setVisible(false);
    ui->customPlot_temperature->setVisible(false);

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);

    this->setWindowTitle("报表打印");
    this->resize(500, 170);


    if (NULL == mysqlDB)
    {
        printLog(LOG_ERROR, "pararm of mysql db is null!");
        return;
    }

    m_mysqlDB = mysqlDB;
    QString filenameTmp = QApplication::applicationDirPath() + QDateTime::currentDateTime().toString("/yyyyMMddhhmmss");
    filenameOfDeepness = filenameTmp +"_deepness.png";
    filenameOfTemperature = filenameTmp +"_temperature.png";

    QList<QList<QVariant>> datas;
    QString strSql = "select hole_number from cdr_site_info;";
    if(-1 == m_mysqlDB->sql_open(strSql.toLatin1().data(), datas))
    {
        printLog(LOG_ERROR, "query data from mysql failed! strSql:%s", strSql.toLatin1().data());
    }

    for(int iRow = 0; iRow < datas.size(); ++iRow)
    {
        ui->comBox_holeNumber->addItem(datas.at(iRow).at(0).toString());
    }

    ui->comBox_reportFormsType->setCurrentIndex(-1);
    ui->comBox_reportFormsData->setCurrentIndex(-1);

    ui->lineEdit_filePath->setReadOnly(true);
    ui->lineEdit_filePath->setPlaceholderText("请选择报表保存目录...");

    connect(ui->comBox_reportFormsType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            [=](int index)
    {
        Q_UNUSED(index);
        ui->comBox_reportFormsData->clear();

        if (ui->comBox_reportFormsType->currentText() == "月报表")
        {
            QDate date = QDate::currentDate();

            for(int i = 0; i < 12; ++i)
            {
                ui->comBox_reportFormsData->addItem(date.toString("yyyy-MM"));
                date = date.addMonths(-1);
            }
            ui->comBox_reportFormsData->setCurrentText(QDate::currentDate().toString("yyyy-MM"));
        }
        else
        {
            QDate date = QDate::currentDate();

            for(int i = 0; i < 5; ++i)
            {
                ui->comBox_reportFormsData->addItem(date.toString("yyyy"));
                date = date.addYears(-1);
            }
            ui->comBox_reportFormsData->setCurrentText(QDate::currentDate().toString("yyyy"));
        }
    });

    ui->comBox_reportFormsType->setCurrentIndex(0);
    return;
}

reportForms::~reportForms()
{
    QFile file(filenameOfDeepness);
    if (file.exists())
    {
        file.remove();
    }
    QFile file1(filenameOfTemperature);
    if (file1.exists())
    {
        file1.remove();
    }
    delete ui;
}

void reportForms::on_pushBtn_confirm_clicked()
{
    if (ui->lineEdit_filePath->text().isEmpty())
    {
        ui->lineEdit_filePath->setStyleSheet("background-color: red;");
        return;
    }

    ui->pushBtn_confirm->setVisible(false);
    ui->pushBtn_cancel->setVisible(false);
    ui->progressBar->setVisible(true);

    //绘图
    QList<QList<QVariant>> datas;
    QString strSqlImage = QString("select report_time, deepness, temperature from cdr_data_full where hole_number='%1' order by report_time").arg(ui->comBox_holeNumber->currentText());

    if(-1 == m_mysqlDB->sql_open(strSqlImage.toLatin1().data(), datas))
    {
        printLog(LOG_ERROR, "query data from mysql failed! strSql:%s", strSqlImage.toLatin1().data());
        return;
    }
    ui->progressBar->setValue(2);

    int iRowCount = datas.size();
    for(int i = 0; i < iRowCount; ++i)
    {
        QList<QVariant> dataRow = datas.at(i);
        uint tmp = QDateTime::fromString(dataRow.at(0).toString(), "yyyy-MM-dd hh:mm:ss").toTime_t();
        m_reportTime.append(tmp);
        m_deepness.append(dataRow.at(1).toFloat());
        m_temperature.append(dataRow.at(2).toFloat());
    }


    //报表所有数据
    QString strSql = QString("select report_time, deepness, temperature \
from cdr_data_full  \
where hole_number='%1' and report_time like '%2%'order by report_time;").arg(ui->comBox_holeNumber->currentText()).arg(ui->comBox_reportFormsData->currentText());
    if(-1 == m_mysqlDB->sql_open(strSql.toLatin1().data(), m_datas))
    {
        printLog(LOG_ERROR, "query data from mysql failed! strSql:%s", strSql.toLatin1().data());
    }
    qDebug() << strSql;
    ui->progressBar->setValue(6);


    //报表平均值/最大值/最小值
    datas.clear();
    QString strSqlMaxMinAvg = QString("select max(deepness), max(temperature), min(deepness), min(temperature), round(avg(deepness), 3), round(avg(temperature), 3), min(battery_level), geological, hole_elevation  from cdr_data_full where hole_number='%1' and report_time like '%2%'").arg(ui->comBox_holeNumber->currentText()).arg(ui->comBox_reportFormsData->currentText());
    if(-1 == m_mysqlDB->sql_open(strSqlMaxMinAvg.toLatin1().data(), datas))
    {
        printLog(LOG_ERROR, "query data from mysql failed! strSql:%s", strSqlMaxMinAvg.toLatin1().data());
    }

    if (1 == datas.size())
    {
        QList<QVariant> dataRow = datas.at(0);
        strMaxDeepness = dataRow.at(0).toString();
        strMaxTemperature = dataRow.at(1).toString();
        strMinDeepnees = dataRow.at(2).toString();
        strMinTemperature = dataRow.at(3).toString();
        strAvgDeepnees = dataRow.at(4).toString();
        strAvgTemperature = dataRow.at(5).toString();
        strMinBattery = dataRow.at(6).toString();
        strGeological = dataRow.at(7).toString();
        strHoleElevation = dataRow.at(8).toString();

    }
    ui->progressBar->setValue(8);

    saveImageOfDeepness();
    saveImageOfTemperature();
    ui->progressBar->setValue(10);
    //printReportForms();

    printFormsThread *workerThread = new printFormsThread(ui->comBox_holeNumber->currentText(),
                                                          ui->comBox_reportFormsData->currentText(),
                                                          ui->lineEdit_filePath->text(),
                                                          filenameOfDeepness,
                                                          filenameOfTemperature,
                                                          strMaxDeepness,
                                                          strMaxTemperature,
                                                          strMinDeepnees,
                                                          strMinTemperature,
                                                          strAvgDeepnees,
                                                          strAvgTemperature,
                                                          strMinBattery,
                                                          strGeological,
                                                          strHoleElevation,
                                                          m_datas,
                                                          this);

    connect(workerThread, &printFormsThread::resultReady, this,
             [=](int value)
    {
        ui->progressBar->setValue(value);
        if (100 == value)
        {
            return accept();
        }
    });
    // 线程结束后，自动销毁
    connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));
    workerThread->start();


}

void reportForms::on_pushBtn_cancel_clicked()
{
    close();
}

void reportForms::on_pushBtn_selectPath_clicked()
{
    QString defaultFileName = "报表文件" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString filePath = QFileDialog::getSaveFileName(0, "LogInfo", defaultFileName, "*.doc");

    QFile file(filePath);
    if(file.exists())
    {
        printLog(LOG_ERROR, "Filename exist! filename:%s.", filePath.toLatin1().data());
        return;
    }
    ui->lineEdit_filePath->setText(filePath);


    return;
}


void reportForms::printReportForms()
{
    QWord word;
    word.createNewWord(ui->lineEdit_filePath->text());

    word.setFontSize(18);				//字体大小
    word.setParagraphAlignment(0);		//下面文字置中
    word.setFontBold(true);				//字体加粗
    word.insertText(QString("(%1)水文常观孔(%2)月报表").arg(ui->comBox_holeNumber->currentText()).arg(ui->comBox_reportFormsData->currentText()));//插入文字
    word.insertMoveDown();

    word.setParagraphAlignment(1);		//下面文字置左
    word.setFontBold(false);			//字体加粗
    word.setFontSize(14);				//字体大小

    QString tmp = "监测含水层：" + strGeological + "   ";
    word.insertText(tmp);//插入文字
    tmp.clear();
    tmp = "孔口标高(m)：" + strHoleElevation + "   ";
    word.insertText(tmp);//插入文字
    word.insertMoveDown();

    word.setParagraphAlignment(0);		//下面文字置中

    int iDataRows = m_datas.size();
    word.insertTable(iDataRows+4, 5);

    word.setColumnWidth(1, 40);
    word.setColumnWidth(2, 140);

    word.setCellString(1, 1, "序号");
    word.setCellString(1, 2, "监测时间");
    word.setCellString(1, 3, "埋深(m)");
    word.setCellString(1, 4, "温度(℃)");
    word.setCellString(1, 5, "备注");


    int i = 0;
    for(; i < iDataRows; ++i)
    {
        QList<QVariant> dataRow = m_datas.at(i);
        for(int j = 0; j < 4; ++j)
        {
            if (j == 0)
                word.setCellString(i+2, j+1, QString::number(i+1));
            else
                word.setCellString(i+2, j+1, dataRow.at(j-1).toString());

        }
    }

    word.setCellString(i+2, 2, "最大值");
    word.setCellString(i+2, 3, strMaxDeepness);
    word.setCellString(i+2, 4, strMaxTemperature);
    word.setCellString(i+3, 2, "最小值");
    word.setCellString(i+3, 3, strMinDeepnees);
    word.setCellString(i+3, 4, strMinTemperature);
    word.setCellString(i+4, 2, "平均值");
    word.setCellString(i+4, 3, strAvgDeepnees);
    word.setCellString(i+4, 4, strAvgTemperature);

    word.moveForEnd();
    word.insertMoveDown();

    word.insertTable(4, 1);
    word.setCellString(1, 1, "埋深变化曲线图");
    word.insertCellPic(2, 1, filenameOfDeepness);
    word.setCellString(3, 1, "水温变化曲线图");
    word.insertCellPic(4, 1, filenameOfTemperature);
    word.moveForEnd();
    word.insertMoveDown();

    word.setParagraphAlignment(1);		//下面文字置左
    word.setFontSize(14);
    word.insertText("制表：__________  审核：__________  科长：__________");




    word.close();
    return;
}

void reportForms::saveImageOfDeepness()
{
    //创建Qui->customPlot_temperature,添加曲线graph,并设置曲线的数据
    //Qui->customPlot_temperature *ui->customPlot_temperature = new Qui->customPlot_temperature;

    ui->customPlot->setLocale(QLocale(QLocale::Chinese, QLocale::China));
    ui->customPlot->addGraph();

    QPen pen;
    pen.setColor(QColor(0, 0, 255, 200));
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);//设置数据点由一条直线连接
    ui->customPlot->graph(0)->setPen(pen);

    ui->customPlot->addGraph(); // 红色点
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);


    //配置底部轴以显示日期而不是编号：
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    QDateTime dateTimeStart;
    QDateTime dateTimeEnd;
    int iDays;
    if ("月报表" == ui->comBox_reportFormsType->currentText())
    {
        dateTimeStart = QDateTime::fromString(ui->comBox_reportFormsData->currentText(), "yyyy-MM");
        QDate date = dateTimeStart.date();
        iDays = date.daysInMonth();
        dateTimeEnd = dateTimeStart.addDays(iDays);

        dateTicker->setTickCount(iDays);
        dateTicker->setDateTimeFormat("dd");
    }
    else
    {
        dateTimeStart = QDateTime::fromString(ui->comBox_reportFormsData->currentText(), "yyyy");
        QDate date = dateTimeStart.date();
        iDays = date.daysInYear();
        dateTimeEnd = dateTimeStart.addDays(iDays);

        dateTicker->setTickCount(12);
        dateTicker->setDateTimeFormat("MM");
    }


    ui->customPlot->xAxis->setTicker(dateTicker);
    ui->customPlot->xAxis->setRange(dateTimeStart.toTime_t(), dateTimeEnd.toTime_t());
    double dTmp = (strMaxDeepness.toDouble() - strMinDeepnees.toDouble()) / 2;
    ui->customPlot->yAxis->setRange(strMinDeepnees.toDouble()-dTmp, strMaxDeepness.toDouble()+dTmp);



    // 设置一个更紧凑的字体大小为底部和左轴刻度标签：
    ui->customPlot->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    // 设置一天为固定的一个刻度
//    ui->customPlot->xAxis->setAutoTickStep(false);
//    ui->customPlot->xAxis->setTickStep(3600*24); // 一天的秒数
//    ui->customPlot->xAxis->setSubTickCount(9);//一个大刻度包含4个小刻度

    // 设置轴标签
    ui->customPlot->xAxis->setLabel("时间(time)");
    ui->customPlot->yAxis->setLabel("埋深(m)");
    // 设置上边和右边轴没有刻度和标签
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->yAxis2->setVisible(true);//坐标轴是否可见
    ui->customPlot->xAxis2->setTicks(false);//刻度是否可见
    ui->customPlot->yAxis2->setTicks(false);
    ui->customPlot->xAxis2->setTickLabels(false);//轴标签是否可见
    ui->customPlot->yAxis2->setTickLabels(false);
    // 设置轴范围和显示全部数据
    //ui->customPlot->xAxis->setRange(time[0], time[0]+24*3600*7);
    //ui->customPlot->yAxis->setRange(30, 45);
    // 显示图例
    ui->customPlot->graph(0)->setName("埋深折线");
    ui->customPlot->graph(1)->setName("具体数值");
    ui->customPlot->legend->setVisible(true);

    ui->customPlot->graph(0)->setData(m_reportTime, m_deepness);//设置数据
    ui->customPlot->graph(0)->rescaleValueAxis();
    ui->customPlot->graph(1)->addData(m_reportTime, m_deepness);//设置点
    ui->customPlot->replot();
    ui->customPlot->savePng(filenameOfDeepness, 500, 240);
}

void reportForms::saveImageOfTemperature()
{
    //创建Qui->customPlot_temperature,添加曲线graph,并设置曲线的数据
    //Qui->customPlot_temperature *ui->customPlot_temperature = new Qui->customPlot_temperature;

    ui->customPlot_temperature->setLocale(QLocale(QLocale::Chinese, QLocale::China));
    ui->customPlot_temperature->addGraph();

    QPen pen;
    pen.setColor(QColor(0, 0, 255, 200));
    ui->customPlot_temperature->graph(0)->setLineStyle(QCPGraph::lsLine);//设置数据点由一条直线连接
    ui->customPlot_temperature->graph(0)->setPen(pen);

    ui->customPlot_temperature->addGraph(); // 红色点
    ui->customPlot_temperature->graph(1)->setPen(QPen(Qt::red));
    ui->customPlot_temperature->graph(1)->setLineStyle(QCPGraph::lsNone);
    ui->customPlot_temperature->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);


    //配置底部轴以显示日期而不是编号：
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    QDateTime dateTimeStart;
    QDateTime dateTimeEnd;
    int iDays;
    if ("月报表" == ui->comBox_reportFormsType->currentText())
    {
        dateTimeStart = QDateTime::fromString(ui->comBox_reportFormsData->currentText(), "yyyy-MM");
        QDate date = dateTimeStart.date();
        iDays = date.daysInMonth();
        dateTimeEnd = dateTimeStart.addDays(iDays);

        dateTicker->setTickCount(iDays);
        dateTicker->setDateTimeFormat("dd");
    }
    else
    {
        dateTimeStart = QDateTime::fromString(ui->comBox_reportFormsData->currentText(), "yyyy");
        QDate date = dateTimeStart.date();
        iDays = date.daysInYear();
        dateTimeEnd = dateTimeStart.addDays(iDays);

        dateTicker->setTickCount(12);
        dateTicker->setDateTimeFormat("MM");
    }


    ui->customPlot_temperature->xAxis->setTicker(dateTicker);
    ui->customPlot_temperature->xAxis->setRange(dateTimeStart.toTime_t(), dateTimeEnd.toTime_t());
    double dTmp = (strMaxTemperature.toDouble() - strMinTemperature.toDouble()) / 2;
    ui->customPlot_temperature->yAxis->setRange(strMinTemperature.toDouble()-dTmp, strMaxTemperature.toDouble()+dTmp);


    // 设置一个更紧凑的字体大小为底部和左轴刻度标签：
    ui->customPlot_temperature->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlot_temperature->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

    // 设置一天为固定的一个刻度
//    ui->customPlot_temperature->xAxis->setAutoTickStep(false);
//    ui->customPlot_temperature->xAxis->setTickStep(3600*24); // 一天的秒数
//    ui->customPlot_temperature->xAxis->setSubTickCount(9);//一个大刻度包含4个小刻度

    // 设置轴标签
    ui->customPlot_temperature->xAxis->setLabel("时间(time)");
    ui->customPlot_temperature->yAxis->setLabel("水温(℃)");
    // 设置上边和右边轴没有刻度和标签
    ui->customPlot_temperature->xAxis2->setVisible(true);
    ui->customPlot_temperature->yAxis2->setVisible(true);//坐标轴是否可见
    ui->customPlot_temperature->xAxis2->setTicks(false);//刻度是否可见
    ui->customPlot_temperature->yAxis2->setTicks(false);
    ui->customPlot_temperature->xAxis2->setTickLabels(false);//轴标签是否可见
    ui->customPlot_temperature->yAxis2->setTickLabels(false);
    // 设置轴范围和显示全部数据
    //ui->customPlot_temperature->xAxis->setRange(time[0], time[0]+24*3600*7);
    //ui->customPlot_temperature->yAxis->setRange(30, 45);
    // 显示图例
    ui->customPlot_temperature->graph(0)->setName("水温折线");
    ui->customPlot_temperature->graph(1)->setName("具体数值");
    ui->customPlot_temperature->legend->setVisible(true);

    ui->customPlot_temperature->graph(0)->setData(m_reportTime, m_temperature);//设置数据
    ui->customPlot_temperature->graph(0)->rescaleValueAxis();
    ui->customPlot_temperature->graph(1)->addData(m_reportTime, m_temperature);//设置点
    ui->customPlot_temperature->replot();
    ui->customPlot_temperature->savePng(filenameOfTemperature, 500, 240);
}


printFormsThread::printFormsThread(QString strHoleNumber,
                                   QString strDate,
                                   QString filePathOfWord,
                                   QString filenameOfDeepness,
                                   QString filenameOfTemperature,
                                   QString strMaxDeepness,
                                   QString strMaxTemperature,
                                   QString strMinDeepnees,
                                   QString strMinTemperature,
                                   QString strAvgDeepnees,
                                   QString strAvgTemperature,
                                   QString strMinBattery,
                                   QString strGeological,
                                   QString strHoleElevation,
                                   QList<QList<QVariant>> &datas,
                                   QObject *parent)
{
    m_strHoleNumber = strHoleNumber;
    m_strDate = strDate;
    m_filePathOfWord = filePathOfWord;
    m_filenameOfDeepness = filenameOfDeepness;
    m_filenameOfTemperature = filenameOfTemperature;

    m_strMaxDeepness = strMaxDeepness;
    m_strMaxTemperature= strMaxTemperature;
    m_strMinDeepnees = strMinDeepnees;
    m_strMinTemperature = strMinTemperature;
    m_strAvgDeepnees = strAvgDeepnees;
    m_strAvgTemperature = strAvgTemperature;
    m_strMinBattery = strMinBattery;
    m_strGeological = strGeological;
    m_strHoleElevation = strHoleElevation;

    m_datas = datas;

    qDebug() << "Worker Run Thread : " << QThread::currentThreadId();
}

void printFormsThread::run()
{
    qDebug() << "Worker Run Thread : " << QThread::currentThreadId();
    printFormsDemo();

}

void printFormsThread::printFormsDemo()
{
    QWord word;
    word.createNewWord(m_filePathOfWord);
    emit resultReady(15);

    word.setFontSize(18);				//字体大小
    word.setParagraphAlignment(0);		//下面文字置中
    word.setFontBold(true);				//字体加粗
    word.insertText(QString("(%1)水文常观孔(%2)月报表").arg(m_strHoleNumber).arg(m_strDate));//插入文字
    word.insertMoveDown();

    word.setParagraphAlignment(1);		//下面文字置左
    word.setFontBold(false);			//字体加粗
    word.setFontSize(14);				//字体大小

    emit resultReady(20);
    QString tmp = "监测含水层：" + m_strGeological + "   ";
    word.insertText(tmp);//插入文字
    tmp.clear();
    tmp = "孔口标高(m)：" + m_strHoleElevation + "   ";
    word.insertText(tmp);//插入文字
    word.insertMoveDown();

    word.setParagraphAlignment(0);		//下面文字置中

    int iDataRows = m_datas.size();
    word.insertTable(iDataRows+4, 5);
    emit resultReady(25);
    word.setColumnWidth(1, 40);
    word.setColumnWidth(2, 140);

    word.setCellString(1, 1, "序号");
    word.setCellString(1, 2, "监测时间");
    word.setCellString(1, 3, "埋深(m)");
    word.setCellString(1, 4, "温度(℃)");
    word.setCellString(1, 5, "备注");

    emit resultReady(30);
    int i = 0;
    for(; i < iDataRows; ++i)
    {
        if (i < 20)
        {
            emit resultReady(30+i);
        }
        QList<QVariant> dataRow = m_datas.at(i);
        for(int j = 0; j < 4; ++j)
        {
            if (j == 0)
                word.setCellString(i+2, j+1, QString::number(i+1));
            else
                word.setCellString(i+2, j+1, dataRow.at(j-1).toString());

        }
    }
    emit resultReady(50);

    word.setCellString(i+2, 2, "最大值");
    word.setCellString(i+2, 3, m_strMaxDeepness);
    word.setCellString(i+2, 4, m_strMaxTemperature);
    word.setCellString(i+3, 2, "最小值");
    word.setCellString(i+3, 3, m_strMinDeepnees);
    word.setCellString(i+3, 4, m_strMinTemperature);
    word.setCellString(i+4, 2, "平均值");
    word.setCellString(i+4, 3, m_strAvgDeepnees);
    word.setCellString(i+4, 4, m_strAvgTemperature);

    word.moveForEnd();
    word.insertMoveDown();

    emit resultReady(75);
    word.insertTable(4, 1);
    word.setCellString(1, 1, "埋深变化曲线图");
    word.insertCellPic(2, 1, m_filenameOfDeepness);
    emit resultReady(80);
    word.setCellString(3, 1, "水温变化曲线图");
    word.insertCellPic(4, 1, m_filenameOfTemperature);
    word.moveForEnd();
    word.insertMoveDown();
    emit resultReady(90);

    word.setParagraphAlignment(1);		//下面文字置左
    word.setFontSize(14);
    word.insertText("制表：__________  审核：__________  科长：__________");


    word.close();
    emit resultReady(100);
    return;
}
