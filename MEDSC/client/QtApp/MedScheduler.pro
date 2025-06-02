QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/appointment.cpp \
    src/doctor_window.cpp \
    src/homepage.cpp \
    src/junior_admin_window.cpp \
    src/main.cpp \
    src/main_window.cpp \
    src/registration_window.cpp \
    src/senior_admin_window.cpp \
    src/utils.cpp \
    ../client/src/client_auth.cpp \
    ../client/src/client_doctor.cpp \
    ../client/src/client_junior_admin.cpp \
    ../client/src/client_patient.cpp \
    ../client/src/client_senior_admin.cpp \
    ../client/src/common_for_all.cpp

HEADERS += \
    include/appointment.h \
    include/doctor_window.h \
    include/homepage.h \
    include/junior_admin_window.h \
    include/main_window.h \
    include/registration_window.h \
    include/senior_admin_window.h \
    include/utils.h \
    ../client/include/client_auth.hpp \
    ../client/include/client_doctor.hpp \
    ../client/include/client_junior_admin.hpp \
    ../client/include/client_patient.hpp \
    ../client/include/client_senior_admin.hpp \
    ../client/include/common_for_all.hpp

FORMS += \
    forms/appointment.ui \
    forms/doctor_window.ui \
    forms/homepage.ui \
    forms/junior_admin_window.ui \
    forms/main_window.ui \
    forms/registration_window.ui \
    forms/senior_admin_window.ui

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
