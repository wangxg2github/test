#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QSettings>
#include <QModelIndex>

#include "common/comm.h"
#include "common/qmymodel.h"
#include "common/mymysql.h"
#include "common/ExcelBase.h"
#include "module/serialPort/comObject.h"
#include "equipmentDialog.h"
#include "reportForms.h"

namespace Ui {
class mainWidget;
}

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainWidget(QWidget *parent = 0);
    ~mainWidget();

    void setMySqlDB(mymysql *mySqlDB);

protected:
    /**
     * @brief 获取系统窗口改变事件，最小化系统
     * @param event ：窗口状态改变
     */
    void changeEvent(QEvent *event);

    /**
     * @brief 重写窗口关闭事件，最小化到托盘
     * @param event ：关闭窗口
     */
    void closeEvent(QCloseEvent *event);

    /**
     * @brief 系统工具栏注册事件过滤器
     * @param obj：窗口工具栏
     * @param event ：双击最大化/恢复，窗口移动
     * @return 若事件被处理，返回true；若事件没有被处理，返回false
     */
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    /**
     * @brief 获取系统托盘信号
     * @param reason
     */
    void on_getWidgetTraySignal(QSystemTrayIcon::ActivationReason reason);

    /**
     * @brief 系统工具栏菜单信号处理
     */
    void on_toolBtnAll_clicked();

    //void on_pushBtn_userConfig_clicked();

    /**
     * @brief 导出excel文件，为csv格式
     */
    void on_pushBtn_exportExcel_clicked();
    void on_pushBtn_serialPort_clicked();

private:
    /**
     * @brief 主界面初始化
     */
    void mainWidgetInit();
    /**
     * @brief 系统托盘初始化
     */
    void systemTrayInit();

    /**
     * @brief settingInit
     */
    void settingInit();

    void threadStart();
    void threadStop();
    void threadDestroy();

    int getWinCom(int index, bool isGetKeyValue, QString &outKey);
    void showData(QTableView *tableView, QMyModel *myModel, QList<QList<QVariant> > *data, QStringList dataHeader, QString &strSql);
private:
    Ui::mainWidget *m_mainWidgetUI;
    mymysql *m_mysqlDB;

    //托盘右键菜单
    QSystemTrayIcon *m_systemTrayIcon;
    QMenu           *m_menuSystemTray;
    QAction         *m_actionShowWidget;
    QAction         *m_actionExit;

    //model
    QMyModel *m_myModelUserInfo;
    QMyModel *m_myModelQueryData;
    QMyModel *m_myModelEquipmentData;
    QMyModel *m_mymodelRealTimeData;


    //tableView userSetting
    bool m_tableViewUserSettingIsShow;
    QList<QList<QVariant>> m_userDatas;
    QStringList m_userHeaderData;

    //query data
    bool m_tableViewQueryDataIsShow;
    QList<QList<QVariant>> m_queryDatas;
    QStringList m_queryDataHeader;
    void queryDataFromDB();

    //equipment
    bool m_tableViewEquipmentShow;
    QList<QList<QVariant>> m_equipmentDatas;
    QStringList m_equipmentDataHeader;
    //void queryDataFromDB();

    //realTime
    bool m_tableViewRealTimeShow;
    QList<QList<QVariant>> m_realTimeDatas;
    QStringList m_realTimeDataHeader;

    //excel
    //QScopedPointer<ExcelBase> *m_excelExport;
    ExcelBase *m_excelExport;

    //serialPort
    comObject *m_comObject;
    QThread *m_comThread;
};

#endif // MAINWIDGET_H
