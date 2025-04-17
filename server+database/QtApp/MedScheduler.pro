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
    ../client/src/client_auth.cpp \
    ../client/src/client_doctor.cpp \
    ../client/src/client_junior_admin.cpp \
    ../client/src/client_patient.cpp \
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
    ../client/include/client_auth.hpp \
    ../client/include/client_doctor.hpp \
    ../client/include/client_junior_admin.hpp \
    ../client/include/client_patient.hpp \
    ../client/include/client_senior_admin.hpp \
    ../client/include/common_for_all.hpp

FORMS += \
    forms/doctorwindow.ui \
    forms/homepage.ui \
    forms/junioradminwindow.ui \
    forms/mainwindow.ui \
    forms/registrationwindow.ui \
    forms/senioradminwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

LIBS += -lcurl

INCLUDEPATH += $$PWD/server+database/client/include
INCLUDEPATH += $$PWD/include
INCLUDEPATH += $$PWD/../client/include
