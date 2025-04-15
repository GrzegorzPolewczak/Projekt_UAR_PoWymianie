QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GWZ.cpp \
    ModelARX.cpp \
    NetworkManager.cpp \
    RegulatorPID.cpp \
    UkladSterowania.cpp \
    main.cpp \
    oknoarx.cpp \
    qcustomplot.cpp \
    ukladautomatycznejregulacji.cpp

HEADERS += \
    GWZ.h \
    ModelARX.h \
    NetworkManager.h \
    RegulatorPID.h \
    UkladSterowania.h \
    oknoarx.h \
    qcustomplot.h \
    ukladautomatycznejregulacji.h

FORMS += \
    oknoarx.ui \
    ukladautomatycznejregulacji.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
