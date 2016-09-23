#ifndef CLS_PIXELMAPENTRY_H
#define CLS_PIXELMAPENTRY_H

#include <Rtypes.h>
#include <TString.h>

class cls_PixelMapEntry
{
public:
    cls_PixelMapEntry(UInt_t p_PMT,
                        UInt_t p_Pixel,
                        Double_t p_X,
                        Double_t p_Y,
                        UInt_t p_SimplX,
                        UInt_t p_SimplY,
                        UInt_t p_TdcId,
                        UInt_t p_Lch,
                        UInt_t p_Tch,
                        UInt_t p_PADIWAnr,
                        UInt_t p_TRBnr,
                        TString p_PMTtype);

public:
    UInt_t mPMT;
    UInt_t mPixel;
    Double_t mX;
    Double_t mY;
    UInt_t mSimplX;
    UInt_t mSimplY;
    UInt_t mTdcId;
    UInt_t mLch;
    UInt_t mTch;
    UInt_t mPADIWAnr;
    UInt_t mTRBnr;
    TString mPMTtype;

};

#endif // CLS_PIXELMAPENTRY_H
