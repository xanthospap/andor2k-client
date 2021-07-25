TEMPLATE = app
TARGET = anguc

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH +=/usr/local/include

SOURCES += \
    advancedsettings.cpp \
    connectiontab.cpp \
    exposuretab.cpp \
    main.cpp \
    mainoptionwindow.cpp \
    tabdialog.cpp \
    temperaturetab.cpp

HEADERS += \
    advancedsettings.h \
    connectiontab.h \
    exposuretab.h \
    mainoptionwindow.h \
    tabdialog.h \
    temperaturetab.h

unix|win32: LIBS += -landor

unix|win32: LIBS += -lhelmosandor2k

unix|win32: LIBS += -lcfitsio
