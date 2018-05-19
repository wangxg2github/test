#include "settingDialog.h"
#include "ui_settingDialog.h"

settingDialog::settingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingDialog)
{
    ui->setupUi(this);
}

settingDialog::~settingDialog()
{
    delete ui;
}
