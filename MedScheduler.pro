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
    server+database/client/src/client_auth.cpp \
    server+database/client/src/client_doctor.cpp \
    server+database/client/src/client_junior_admin.cpp \
    server+database/client/src/client_patient.cpp \
    server+database/client/src/client_senior_admin.cpp \
    server+database/client/src/common_for_all.cpp

HEADERS += \
    include/doctorwindow.h \
    include/homepage.h \
    include/junioradminwindow.h \
    include/mainwindow.h \
    include/registrationwindow.h \
    include/senioradminwindow.h \
    include/utils.h \
    server+database/client/include/client_auth.hpp \
    server+database/client/include/client_doctor.hpp \
    server+database/client/include/client_junior_admin.hpp \
    server+database/client/include/client_patient.hpp \
    server+database/client/include/client_senior_admin.hpp \
    server+database/client/include/common_for_all.hpp

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
INCLUDEPATH += $$PWD/include

