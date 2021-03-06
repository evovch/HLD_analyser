#include "MainWindow.h"

#include "ui_MainWindow.h"

#include <TApplication.h>
#include "QRootApplication.h"

#include <iostream>

int main(int argc, char *argv[])
{
    TApplication app("HLD_reader", &argc, argv);
    QRootApplication myapp(argc, argv);

    cls_MainWindow w;
    w.show();

    if (app.Argc() > 1) {
        w.SetConfigFile(QString(app.Argv(1)));
        w.ImportConfig();
        w.GenFilenames();
    }

    if (app.Argc() > 2) {

        QString v_mode(app.Argv(2));

        if (v_mode.contains("gencalib")) {
            w.BatchGenCalib();
        }
        if (v_mode.contains("laboratory_calib_nocorr")) {
            w.BatchLabAnalysisCalibNoCorr();
        }
        if (v_mode.contains("laser_nocalib_nocorr")) {
            w.BatchLaserAnalysisNoCalibNoCorr();
        }
        if (v_mode.contains("laser_calib_nocorr")) {
            w.BatchLaserAnalysisCalibNoCorr();
        }
        if (v_mode.contains("laser_nocalib_corr")) {
            w.BatchLaserAnalysisNoCalibCorr();
        }
        if (v_mode.contains("laser_calib_corr")) {
            w.BatchLaserAnalysisCalibCorr();
        }
        if (v_mode.contains("rings_nocalib_nocorr")) {
            w.BatchRingsAnalysisNoCalibNoCorr();
        }
        if (v_mode.contains("rings_calib_nocorr")) {
            w.BatchRingsAnalysisCalibNoCorr();
        }
        if (v_mode.contains("rings_nocalib_corr")) {
            w.BatchRingsAnalysisNoCalibCorr();
        }
        if (v_mode.contains("rings_calib_corr")) {
            w.BatchRingsAnalysisCalibCorr();
        }
        if (v_mode.contains("direct_tdc_nocalib")) {
            w.BatchDirectTDCnocalib();
        }
        if (v_mode.contains("direct_tdc_calib")) {
            w.BatchDirectTDCcalib();
        }
        if (v_mode.contains("direct_tdc_individ_pseudocalib")) {
            w.BatchDirectTDCindividPseudocalib();
        }
        if (v_mode.contains("direct_tdc_global_pseudocalib")) {
            w.BatchDirectTDCglobalPseudocalib();
        }

        return 0;
    }

    return myapp.exec();
}
