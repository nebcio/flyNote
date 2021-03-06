QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17 -Wall -Wextra -pedantic

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    askerwindow.cpp \
    flynote.cpp \
    main.cpp \
    mainwindow.cpp \
    panelinputtime.cpp \
    titlebar.cpp

HEADERS += \
    askerwindow.h \
    flynote.h \
    mainwindow.h \
    panelinputtime.h \
    titlebar.h \
    ui_flynote.h \
    ui_mainwindow.h

TRANSLATIONS += \
    flyNote_pl_PL.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS +=

DISTFILES += \
    img/close.png \
    img/icon.ico \
    img/new.png \
    img/open.png \
    img/tick.png \
    img/trash.png \
    style/domestic-manners.regular.ttf \
    style/flyNote.css
