QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    doctorwindow.cpp \
    homepage.cpp \
    junioradminwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    registrationwindow.cpp \
    senioradminwindow.cpp \
    utils.cpp \

HEADERS += \
    doctorwindow.h \
    homepage.h \
    junioradminwindow.h \
    mainwindow.h \
    registrationwindow.h \
    senioradminwindow.h \
    utils.h

FORMS += \
    doctorwindow.ui \
    homepage.ui \
    junioradminwindow.ui \
    mainwindow.ui \
    registrationwindow.ui \
    senioradminwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

LIBS += -lcurl

INCLUDEPATH += $$PWD/server+database/client/include



