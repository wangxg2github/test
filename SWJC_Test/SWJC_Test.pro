#-------------------------------------------------
#
# Project created by QtCreator 2018-01-27T20:13:17
#
#-------------------------------------------------

QT       += core gui xml charts axcontainer charts
#CONFIG += qaxcontainer
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET      = SWJC_Test
TEMPLATE    = app
#MOC_DIR     = temp/moc
#RCC_DIR     = temp/rcc
#UI_DIR      = temp/ui
#OBJECTS_DIR = temp/obj
DESTDIR     = $$PWD/../../bin/swjc_test

DEFINES += QT_DEPRECATED_WARNINGS

#在windows下使用第三方库的方法
LIBS += -LD:\Software\mysql\lib -llibmysql
#在linux下使用第三方库的方法
#LIBS += -lmysqlclient



SOURCES += main.cpp \
    mainWidget.cpp \
    common/st_logger.cpp \
    module/login/loginDlg.cpp \
    common/mymysql.cpp \
    common/qmymodel.cpp \
    common/ExcelBase.cpp \
    module/serialPort/comObject.cpp \
    module/serialPort/qextserialbase.cpp \
    module/serialPort/win_qextserialport.cpp \
    equipmentDialog.cpp \
    reportForms.cpp \
    common/qword.cpp \
    common/qcustomplot.cpp

HEADERS  += \
    mainWidget.h \
    common/st_logger.h \
    common/comm.h \
    module/login/loginDlg.h \
    common/mymysql.h \
    common/qmymodel.h \
    common/ExcelBase.h \
    module/serialPort/comObject.h \
    module/serialPort/qextserialbase.h \
    module/serialPort/win_qextserialport.h \
    equipmentDialog.h \
    common/commdata.h \
    reportForms.h \
    common/qword.h \
    common/qcustomplot.h

FORMS    += \
    mainWidget.ui \
    equipmentDialog.ui \
    reportForms.ui

RESOURCES += \
    image.qrc \
    qss.qrc
