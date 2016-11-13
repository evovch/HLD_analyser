QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HLD_reader

TEMPLATE = app

SOURCES += main.cpp \
    Go4QtRoot/QRootApplication.cpp \
    Go4QtRoot/QRootCanvas.cpp \
    Go4QtRoot/QRootDialog.cpp \
    Go4QtRoot/QRootWindow.cpp \
    Reco/CbmRichProtRingFinderHoughImpl.cxx \
    Reco/CbmRichRingFitterCOP.cxx \
    Data/DataHandler.cpp \
    Data/Event.cpp \
    Data/Hit.cpp \
    Data/PixelMapEntry.cpp \
    Data/RawMessage.cpp \
    HistoWidget.cpp \
    GraphWidget.cpp \
    MainWindow.cpp \
    HLD_file.cpp \
    Calibrator.cpp \
    Unpacking_info.cpp \
    EdgeMatcher.cpp \
    global_defines.cpp \
    EventsAnalyser.cpp \
    RingsAnalyser.cpp \
    RingVisu.cpp \
    DirectTDCanalyser.cpp

HEADERS += \
    Go4QtRoot/QRootApplication.h \
    Go4QtRoot/QRootCanvas.h \
    Go4QtRoot/QRootDialog.h \
    Go4QtRoot/QRootWindow.h \
    Go4QtRoot/TGo4LockGuard.h \
    Reco/CbmLitMemoryManagment.h \
    Reco/CbmRichProtRingFinderHoughImpl.h \
    Reco/CbmRichRingFinderData.h \
    Reco/CbmRichRingFitterBase.h \
    Reco/CbmRichRingFitterCOP.h \
    Reco/CbmRichRingLight.h \
    Data/DataHandler.h \
    Data/Event.h \
    Data/Hit.h \
    Data/PixelMapEntry.h \
    Data/RawMessage.h \
    HistoWidget.h \
    GraphWidget.h \
    MainWindow.h \
    HLD_file.h \
    Calibrator.h \
    Unpacking_info.h \
    EdgeMatcher.h \
    global_defines.h \
    EventsAnalyser.h \
    RingsAnalyser.h \
    RingVisu.h \
    DirectTDCanalyser.h

FORMS += \
    HistoWidget.ui \
    GraphWidget.ui \
    MainWindow.ui \
    Unpacking_info.ui \
    RingVisu.ui

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wpedantic -fopenmp

INCLUDEPATH += \
    Go4QtRoot/ \
    Go4LockGuard/ \
    Reco/ \
    Data/ \
    /home/evovch/soft/root_install/include

LIBS += -lX11 -fopenmp \
    -L/home/evovch/soft/root_install/lib \
    -lGui -lCore -lCling -lRIO -lNet -lHist -lGraf -lGraf3d \
    -lGpad -lTree -lRint -lPostscript -lMatrix \
    -lPhysics -lMathCore -lThread -lMultiProc \
    -lGed -lTreePlayer \
    -pthread -lm -ldl -rdynamic

DISTFILES += \
    batch_laser_calib_corr.sh \
    batch_laser_calib_nocorr.sh \
    batch_laser_nocalib_corr.sh \
    batch_laser_nocalib_nocorr.sh \
    batch_gencalib.sh \
    batch_rings_calib_corr.sh \
    batch_rings_calib_nocorr.sh \
    batch_rings_nocalib_corr.sh \
    batch_rings_nocalib_nocorr.sh \
    batch_buildCorrections.sh \
    batch_direct_tdc.sh
