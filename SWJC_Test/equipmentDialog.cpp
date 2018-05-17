#include "equipmentDialog.h"
#include "ui_equipmentDialog.h"

#include <QRegExp>

equipmentDialog::equipmentDialog(bool isAdd) :
    ui(new Ui::equipmentDialog)
{
    ui->setupUi(this);

    m_isAddInfo = isAdd;
    m_isChangeOfControl = false;

    ui->comBo_siteStatus->addItem("使用");
    ui->comBo_siteStatus->addItem("停用");
    ui->comBo_siteStatus->addItem("维修");

    if (m_isAddInfo)
    {
        this->setWindowTitle("添加分站信息");
        //添加分站信息
        //数字
        QRegExp reg1("[0-9]+$");
        QRegExpValidator *pValidator1 = new QRegExpValidator(this);
        pValidator1->setRegExp(reg1);

        // 字符和数字
        QRegExp reg("[a-zA-Z0-9]+$");
        QRegExpValidator *pValidator = new QRegExpValidator(this);
        pValidator->setRegExp(reg);

        // 浮点型 范围：[-360, 360] 精度：小数点后2位
        QDoubleValidator *pDoubleValidator = new QDoubleValidator(this);
        //pDoubleValidator->setRange(-360, 360);
        pDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
        pDoubleValidator->setDecimals(3);

        ui->lineEdit_holeNumber->setPlaceholderText("由字母和数字组成");
        ui->lineEdit_holeNumber->setValidator(pValidator);

        ui->lineEdit_holeX->setPlaceholderText("eg:1234.123");
        ui->lineEdit_holeX->setValidator(pDoubleValidator);

        ui->lineEdit_holeY->setPlaceholderText("eg:1234.123");
        ui->lineEdit_holeY->setValidator(pDoubleValidator);

        ui->lineEdit_holeZ->setPlaceholderText("eg:1234.123");
        ui->lineEdit_holeZ->setValidator(pDoubleValidator);

        ui->lineEdit_holeElevation->setPlaceholderText("eg:1234.123");
        ui->lineEdit_holeElevation->setValidator(pDoubleValidator);

        ui->lineEdit_holeDepth->setPlaceholderText("eg:1234.123");
        ui->lineEdit_holeDepth->setValidator(pDoubleValidator);

        //ui->lineEdit_geological

        ui->lineEdit_waterElevation->setPlaceholderText("eg:1234.123");
        ui->lineEdit_waterElevation->setValidator(pDoubleValidator);


        ui->lineEdit_siteNumber->setPlaceholderText("由字母和数字组成");
        ui->lineEdit_siteNumber->setValidator(pValidator);

        ui->lineEdit_sitePhoneNo->setPlaceholderText("eg:18700001111");
        ui->lineEdit_sitePhoneNo->setValidator(pValidator1);

//        ui->comBo_siteStatus->addItem("使用");
//        ui->comBo_siteStatus->addItem("停用");
//        ui->comBo_siteStatus->addItem("维修");

        ui->lineEdit_lineLenght->setPlaceholderText("eg:1234.123");
        ui->lineEdit_lineLenght->setValidator(pDoubleValidator);

        ui->lineEdit_calibration->setPlaceholderText("eg:1234.123");
        ui->lineEdit_calibration->setValidator(pDoubleValidator);


    }
    else
    {
        this->setWindowTitle("修改分站信息");
        //修改分站信息
        ui->lineEdit_holeNumber->setReadOnly(true);
        ui->lineEdit_siteNumber->setReadOnly(true);
    }



}

equipmentDialog::~equipmentDialog()
{
    delete ui;
}

/**
 * @brief equipmentDialog::getData
 * @return
 *
 *1 -> 孔号	hole_number
 *2 -> 钻孔坐标          hole_coordinate_X
 *3 ->                  hole_coordinate_Y
 *4 ->                  hole_coordinate_X
 *5 -> 孔口标高（m）     hole_elevation
 *6 -> 终孔深度 （m）	hole_depth
 *7 -> 水层地质层    	geological
 *8 -> 初始水位标高(m)	water_elevation
 *9 -> 分站编号         site_numer
 *10 -> 分站卡号        site_phone
 *11 -> 分站状态        site_atatus
 *12 -> 传感器线长(mm)   line_lenght
 *13 -> 传感器校准（m）	 calibration
 *14 -> 分站安装位置	    location
 */

QStringList equipmentDialog::getData()
{
    QStringList dataList;
    dataList << ui->lineEdit_holeNumber->text();
    dataList << ui->lineEdit_holeX->text();
    dataList << ui->lineEdit_holeY->text();
    dataList << ui->lineEdit_holeZ->text();
    dataList << ui->lineEdit_holeElevation->text();
    dataList << ui->lineEdit_holeDepth->text();
    dataList << ui->lineEdit_geological->text();
    dataList << ui->lineEdit_waterElevation->text();
    dataList << ui->lineEdit_siteNumber->text();
    dataList << ui->lineEdit_sitePhoneNo->text();
    dataList << ui->comBo_siteStatus->currentText();
    dataList << ui->lineEdit_lineLenght->text();
    dataList << ui->lineEdit_calibration->text();
    dataList << ui->lineEdit_location->text();
    return dataList;
}

void equipmentDialog::setData(QStringList &data)
{
    if (14 == data.size())
    {
        ui->lineEdit_holeNumber->setText(data.at(0));
        ui->lineEdit_holeX->setText(data.at(1));
        ui->lineEdit_holeY->setText(data.at(2));
        ui->lineEdit_holeZ->setText(data.at(3));
        ui->lineEdit_holeElevation->setText(data.at(4));
        ui->lineEdit_holeDepth->setText(data.at(5));
        ui->lineEdit_geological->setText(data.at(6));
        ui->lineEdit_waterElevation->setText(data.at(7));
        ui->lineEdit_siteNumber->setText(data.at(8));
        ui->lineEdit_sitePhoneNo->setText(data.at(9));
        ui->comBo_siteStatus->setCurrentText(data.at(10));
        ui->lineEdit_lineLenght->setText(data.at(11));
        ui->lineEdit_calibration->setText(data.at(12));
        ui->lineEdit_location->setText(data.at(13));
    }
    else
    {
        printLog(LOG_ERROR, " ");
    }

}

bool equipmentDialog::isChangeOfControl()
{
    return m_isChangeOfControl;
}

void equipmentDialog::on_pushBtn_sure_clicked()
{
    if (isFullOfControl())
    {
        accept();
    }
}

void equipmentDialog::on_pushBtn_cancel_clicked()
{
    close();
}

bool equipmentDialog::isFullOfControl()
{
    if (ui->lineEdit_holeNumber->text().isEmpty())
    {
        ui->lineEdit_holeNumber->setStyleSheet("background-color: blue;");
        return false;
    }
    else
    {
        ui->lineEdit_holeNumber->setStyleSheet("background-color: white;");
    }
    if (ui->lineEdit_holeX->text().isEmpty())
    {
        ui->lineEdit_holeX->setStyleSheet("background-color: blue;");
        return false;
    }
    ui->lineEdit_holeX->setStyleSheet("background-color: white;");

    if (ui->lineEdit_holeY->text().isEmpty())
    {
        ui->lineEdit_holeY->setStyleSheet("background-color: blue;");
        return false;
    }
    ui->lineEdit_holeY->setStyleSheet("background-color: white;");

    if (ui->lineEdit_holeZ->text().isEmpty())
    {
        ui->lineEdit_holeZ->setStyleSheet("background-color: blue;");
        return false;
    }
    ui->lineEdit_holeZ->setStyleSheet("background-color: white;");

    if (ui->lineEdit_holeElevation->text().isEmpty())
    {
        ui->lineEdit_holeElevation->setStyleSheet("background-color: blue;");
        return false;
    }
    ui->lineEdit_holeElevation->setStyleSheet("background-color: white;");

    if (ui->lineEdit_holeDepth->text().isEmpty())
    {
        ui->lineEdit_holeDepth->setStyleSheet("background-color: blue;");
        return false;
    }
    ui->lineEdit_holeDepth->setStyleSheet("background-color: white;");

    if (ui->lineEdit_waterElevation->text().isEmpty())
    {
        ui->lineEdit_waterElevation->setStyleSheet("background-color: blue;");
        return false;
    }
    ui->lineEdit_waterElevation->setStyleSheet("background-color: white;");

    if (ui->lineEdit_siteNumber->text().isEmpty())
    {
        ui->lineEdit_siteNumber->setStyleSheet("background-color: blue;");
        return false;
    }
    ui->lineEdit_siteNumber->setStyleSheet("background-color: white;");

    if (ui->lineEdit_sitePhoneNo->text().isEmpty())
    {
        ui->lineEdit_sitePhoneNo->setStyleSheet("background-color: blue;");
        return false;
    }
    ui->lineEdit_sitePhoneNo->setStyleSheet("background-color: white;");

    if (ui->lineEdit_lineLenght->text().isEmpty())
    {
        ui->lineEdit_lineLenght->setStyleSheet("background-color: blue;");
        return false;
    }
    ui->lineEdit_lineLenght->setStyleSheet("background-color: white;");

    if (ui->lineEdit_calibration->text().isEmpty())
    {
        ui->lineEdit_calibration->setStyleSheet("background-color: blue;");
        return false;
    }
    ui->lineEdit_calibration->setStyleSheet("background-color: white;");

    if (ui->lineEdit_location->text().isEmpty())
    {
        ui->lineEdit_location->setStyleSheet("background-color: blue;");
        return false;
    }
    ui->lineEdit_location->setStyleSheet("background-color: white;");

    return true;
}
