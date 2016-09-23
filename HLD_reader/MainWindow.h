#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Rtypes.h"

class cls_HLD_file;
class cls_Calibrator;

namespace Ui {
class cls_MainWindow;
}

class cls_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit cls_MainWindow(QWidget *parent = 0);
    ~cls_MainWindow();

public slots:
    void SetConfigFile(QString p_filename);
    void SelectConfigFile(void);
    UInt_t ImportConfig(void);
    UInt_t ExportConfig(void);
    void GenFilenames(void);
    void PostprocessConfig(void);

    void ImportFile(void);          // actually Unpack!
    void RunEdgeMatcher(void);
    void RunEventBuilder(void);
    void RunLaserAnalyser(void);
    void RunRingsAnalyser(void);

    void ExportCalibrationOfFile(void);
    void ImportCalibration(void);
    void SetNoCalibration(void);
    void ExportMatchedEdges(void);
    void ImportMatchedEdges(void);

    void ExportUnpackInfo(void);
    void ExportEdgeMatcherInfo(void);
    void ExportEventBuildingInfo(void);
    void ExportAnalysisInfo(void);
    void ExportCorrections(void);
    void ImportCorrections(void);
    void ExportRingsAnalysisInfo(void);


    void BatchGenCalib(void);
    void BatchLaserAnalysisNoCalibNoCorr(void);
    void BatchLaserAnalysisCalibNoCorr(void);
    void BatchLaserAnalysisNoCalibCorr(void);
    void BatchLaserAnalysisCalibCorr(void);
    void BatchRingsAnalysisNoCalibNoCorr(void);
    void BatchRingsAnalysisCalibNoCorr(void);
    void BatchRingsAnalysisNoCalibCorr(void);
    void BatchRingsAnalysisCalibCorr(void);


private:
    Ui::cls_MainWindow *ui;

    cls_HLD_file* mFile;
    cls_Calibrator* mCalibrator;

};

#endif // MAINWINDOW_H
