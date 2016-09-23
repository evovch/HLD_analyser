#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include "global_defines.h"

#include <Rtypes.h>
#include <TString.h>
#include <QString>

#include <cassert>

class TH1D;
class TH1C;
class TH1I;

class cls_Calibrator
{
public: // methods
    cls_Calibrator();
    ~cls_Calibrator();

    cls_Calibrator& operator=(const cls_Calibrator& other);

    void Allocate(void);
    void Deallocate(void);

    //void Reset(void);

    void AddFineTime(UInt_t p_tdcId, UInt_t p_ch, UInt_t p_fine);

    Double_t GetFullTime(UInt_t p_tdcId, UInt_t p_ch, UInt_t p_epoch, UInt_t p_coarse, UInt_t p_fine);

    UInt_t Import(TString p_filename);
    UInt_t Import(QString p_qfilename);

    UInt_t Export(TString p_filename);
    UInt_t Export(QString p_qfilename);

    UInt_t CalibrateOneChannel(UInt_t p_tdcId, UInt_t p_ch);
    void CalibrateOneTDC(UInt_t p_tdcId);
    void CalibrateAll(void);
    void SetNoCalibration(void);

    UInt_t ImportCorrections(TString p_filename);
    UInt_t ImportCorrections(QString p_qfilename);

private: // data members

    TH1C* fCalibDone[NUMTDCs];
    TH1I* fCalibEntries[NUMTDCs];
    TH1D* fFineBuffer[NUMTDCs][NUMCHs];
    TH1D* fCalcBinWidth[NUMTDCs][NUMCHs];
    TH1D* fCalTable[NUMTDCs][NUMCHs];
    TH1D* fCalTableMinusLinear[NUMTDCs][NUMCHs];

    Double_t fCorrections[1024];

    static UInt_t fMinimumEntries;

public:
    Bool_t fAllocated;

};

#endif // CALIBRATOR_H
