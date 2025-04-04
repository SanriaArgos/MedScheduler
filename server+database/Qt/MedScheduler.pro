QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/doctorwindow.cpp \
    src/homepage.cpp \
    src/junioradminwindow.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/registrationwindow.cpp \
    src/senioradminwindow.cpp \
    src/utils.cpp \
    ../client/src/client_senior_admin.cpp \
    ../client/src/common_for_all.cpp

HEADERS += \
    include/doctorwindow.h \
    include/homepage.h \
    include/junioradminwindow.h \
    include/mainwindow.h \
    include/registrationwindow.h \
    include/senioradminwindow.h \
    include/utils.h \
    ../client/include/client_senior_admin.hpp

FORMS += \
    doctorwindow.ui \
    homepage.ui \
    junioradminwindow.ui \
    mainwindow.ui \
    registrationwindow.ui \
    senioradminwindow.ui

INCLUDEPATH += ../../client/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

LIBS += -lcurl

