QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    form.cpp \
    farm_login.cpp \
    regisist.cpp
HEADERS += \
    main.h \
    mainwindow.h \
    form.h \
    farm_login.h \
    regisist.h
FORMS += \
    mainwindow.ui \
    form.ui \
    farm_login.ui \
    regisist.ui
RESOURCES += \
        imag.qrc
LIBS += -lws2_32
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
