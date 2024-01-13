QT += core gui quickcontrols2

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TEMPLATE = app
TARGET = SmartCalc_v2-0

SOURCES += \
    main.cpp \
    view/s21_view.cpp \
    view/qcustomplot.cpp \
    controller/s21_controller.cpp \
    model/s21_model.cpp

HEADERS += \
    view/s21_view.h \
    view/qcustomplot.h \
    controller/s21_controller.h \
    model/s21_model.h

FORMS += \
    view/view.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
