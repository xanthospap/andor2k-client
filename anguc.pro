TEMPLATE = app
TARGET = anguc

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH +=/usr/local/include

SOURCES += \
    advancedsettings.cpp \
    connectiontab.cpp \
    exposuretab.cpp \
    temperaturetab.cpp \
    statustab.cpp \
    tabdialog.cpp \
    exposurestatusdialog.cpp \
    main.cpp \

HEADERS += \
    advancedsettings.h \
    connectiontab.h \
    exposuretab.h \
    temperaturetab.h \
    statustab.h \
    exposurestatusdialog.h \
    tabdialog.h

unix|win32: LIBS += -landor

unix|win32: LIBS += -lhelmosandor2k

unix|win32: LIBS += -lcfitsio
