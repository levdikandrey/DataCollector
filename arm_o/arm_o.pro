QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(3rdparty/qtxlsx/src/xlsx/qtxlsx.pri)

PSQL_INCDIR = C:\PostgreSQL\10\include
PSQL_LIBDIR = C:\PostgreSQL\10\lib

SOURCES += \
    cimportdata.cpp \
    cimportdatakinopoisk.cpp \
    d_addgroup.cpp \
    daddavp.cpp \
    daddtask.cpp \
    dadduser.cpp \
    daddviolation.cpp \
    deditgroup.cpp \
    dedittask.cpp \
    dedittaskuser.cpp \
    dedituser.cpp \
    denter.cpp \
    dgroup.cpp \
    dimportdataoutexcel.cpp \
    duser.cpp \
    dviolation.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    cimportdata.h \
    cimportdatakinopoisk.h \
    d_addgroup.h \
    daddavp.h \
    daddtask.h \
    dadduser.h \
    daddviolation.h \
    deditgroup.h \
    dedittask.h \
    dedittaskuser.h \
    dedituser.h \
    denter.h \
    dgroup.h \
    dimportdataoutexcel.h \
    duser.h \
    dviolation.h \
    mainwindow.h

FORMS += \
    d_addavp.ui \
    d_addgroup.ui \
    d_addtask.ui \
    d_adduser.ui \
    d_addviolation.ui \
    d_editgroup.ui \
    d_edittask.ui \
    d_edittaskusers.ui \
    d_edituser.ui \
    d_enter.ui \
    d_group.ui \
    d_importdata.ui \
    d_user.ui \
    d_violation.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
