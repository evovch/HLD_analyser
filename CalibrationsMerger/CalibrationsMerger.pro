QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CalibrationsMerger

TEMPLATE = app

SOURCES += main.cpp \
    MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wpedantic

INCLUDEPATH += \
    /home/evovch/soft/root_install/include

LIBS += \
    -L/home/evovch/soft/root_install/lib \
    -lGui -lCore -lCling -lRIO -lNet -lHist -lGraf -lGraf3d \
    -lGpad -lTree -lRint -lPostscript -lMatrix \
    -lPhysics -lMathCore -lThread -lMultiProc \
    -lGed -lTreePlayer \
    -pthread -lm -ldl -rdynamic
