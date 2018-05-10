#ifndef EQUIPMENTDIALOG_H
#define EQUIPMENTDIALOG_H

#include <QDialog>
#include "common/comm.h"

namespace Ui {
class equipmentDialog;
}

class equipmentDialog : public QDialog
{
    Q_OBJECT
    enum
    {
        INFORMATION_EDIT,
        INFORMAFTION_ADD
    };

public:
    explicit equipmentDialog(bool isAdd);
    ~equipmentDialog();

    QStringList getData();
    void setData(QStringList &data);
    bool isChangeOfControl();

private slots:
    void on_pushBtn_sure_clicked();
    void on_pushBtn_cancel_clicked();

    bool isFullOfControl();
private:
    Ui::equipmentDialog *ui;

    bool m_isAddInfo;
    bool m_isChangeOfControl;


};

#endif // EQUIPMENTDIALOG_H
