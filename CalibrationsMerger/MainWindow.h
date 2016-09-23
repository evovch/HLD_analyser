#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Rtypes.h>
#include <TH1D.h>

const UInt_t NUMTDCs = 68;
const UInt_t NUMCHs = 33;

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
    void SelectInputFiles(void);
    void SelectOutputFile(void);
    void Merge(void);

private: // data members
    Ui::cls_MainWindow *ui;

    // Imported from the input files
    TH1D* fFineBuffer[NUMTDCs][NUMCHs];

    // Generated from scratch
    TH1C* fCalibDone[NUMTDCs];
    TH1I* fCalibEntries[NUMTDCs];
    TH1D* fCalcBinWidth[NUMTDCs][NUMCHs];
    TH1D* fCalTable[NUMTDCs][NUMCHs];
    TH1D* fCalTableMinusLinear[NUMTDCs][NUMCHs];

    static UInt_t fMinimumEntries;

private: // methods

    void InitHistos(void);

    UInt_t CalibrateOneChannel(UInt_t p_tdcId, UInt_t p_ch);
    void CalibrateOneTDC(UInt_t p_tdcId);
    void CalibrateAll(void);

    UInt_t Export(void);

    UInt_t IntegerToTDCid(UInt_t index) {
        return (((index/4) + 1) << 4) + (index%4);
    }



};

#endif // MAINWINDOW_H
