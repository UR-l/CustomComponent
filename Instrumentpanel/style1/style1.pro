QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

TARGET      = style1
TEMPLATE    = app

SOURCES     += main.cpp \
            arcWidget.cpp

INCLUDEPATH += $$PWD

HEADERS += \
    arcWidget.h

FORMS +=
