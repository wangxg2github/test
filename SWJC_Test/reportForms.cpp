#include "reportForms.h"
#include "ui_reportForms.h"


reportForms::reportForms(mymysql *mysqlDB) :
    ui(new Ui::reportForms)
{
    ui->setupUi(this);

    if (NULL == mysqlDB)
    {
        printLog(LOG_ERROR, "pararm of mysql db is null!");
    }

    m_mysqlDB = mysqlDB;

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
    delete ui;
}

void reportForms::on_pushBtn_confirm_clicked()
{

    if (ui->lineEdit_filePath->text().isEmpty())
    {
        ui->lineEdit_filePath->setStyleSheet("background-color: red;");
        return;
    }
    QString strSql = QString("select report_time, deepness, temperature \
from cdr_data_full  \
where hole_number='%1' and report_time like '%2%'order by report_time limit 10;").arg(ui->comBox_holeNumber->currentText()).arg(ui->comBox_reportFormsData->currentText());
    if(-1 == m_mysqlDB->sql_open(strSql.toLatin1().data(), m_datas))
    {
        printLog(LOG_ERROR, "query data from mysql failed! strSql:%s", strSql.toLatin1().data());
    }
    qDebug() << strSql;


    QString strSqlMax = QString("select max(deepness), max(temperature) \
from cdr_data_full  \
where hole_number='%1' and report_time like '%2%'order by report_time limit 10;").arg(ui->comBox_holeNumber->currentText()).arg(ui->comBox_reportFormsData->currentText());
    if(-1 == m_mysqlDB->sql_open(strSqlMax.toLatin1().data(), m_maxDatas))
    {
        printLog(LOG_ERROR, "query data from mysql failed! strSql:%s", strSqlMax.toLatin1().data());
    }
    qDebug() << strSqlMax;

    QString strSqlMin = QString("select min(deepness), min(temperature) \
from cdr_data_full  \
where hole_number='%1' and report_time like '%2%'order by report_time limit 10;").arg(ui->comBox_holeNumber->currentText()).arg(ui->comBox_reportFormsData->currentText());
    if(-1 == m_mysqlDB->sql_open(strSqlMin.toLatin1().data(), m_minDatas))
    {
        printLog(LOG_ERROR, "query data from mysql failed! strSql:%s", strSqlMin.toLatin1().data());
    }
    qDebug() << strSqlMax;

    QString strSqlAvg = QString("select min(deepness), min(temperature) \
from cdr_data_full  \
where hole_number='%1' and report_time like '%2%'order by report_time limit 10;").arg(ui->comBox_holeNumber->currentText()).arg(ui->comBox_reportFormsData->currentText());
    if(-1 == m_mysqlDB->sql_open(strSqlAvg.toLatin1().data(), m_avgDatas))
    {
        printLog(LOG_ERROR, "query data from mysql failed! strSql:%s", strSqlAvg.toLatin1().data());
    }
    qDebug() << strSqlMax;


    printReportForms();
    return accept();
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

    word.insertText(QString("监测含水层：       "));//插入文字
    word.insertText(QString("孔口标高(m)：         "));//插入文字
    word.insertMoveDown();

    word.setParagraphAlignment(0);		//下面文字置中

    int iDataRows = m_datas.size();
    word.intsertTable(iDataRows+4, 5);

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
    word.setCellString(i+2, 3, m_maxDatas.at(0).at(0).toString());
    word.setCellString(i+2, 4, m_maxDatas.at(0).at(1).toString());
    word.setCellString(i+3, 2, "最小值");
    word.setCellString(i+3, 3, m_minDatas.at(0).at(0).toString());
    word.setCellString(i+3, 4, m_minDatas.at(0).at(1).toString());
    word.setCellString(i+4, 2, "平均值");
    word.setCellString(i+4, 3, m_avgDatas.at(0).at(0).toString());
    word.setCellString(i+4, 4, m_avgDatas.at(0).at(1).toString());

    word.moveForEnd();
    word.insertMoveDown();

    word.setParagraphAlignment(1);		//下面文字置左
    word.setFontSize(14);
    word.insertText("制表：__________  审核：__________  科长：__________");




    word.close();
    cout << "word close.";
}
