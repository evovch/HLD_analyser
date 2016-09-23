#include "PixelMapEntry.h"

cls_PixelMapEntry::cls_PixelMapEntry(UInt_t p_PMT,
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
                                     TString p_PMTtype) :
    mPMT(p_PMT),
    mPixel(p_Pixel),
    mX(p_X),
    mY(p_Y),
    mSimplX(p_SimplX),
    mSimplY(p_SimplY),
    mTdcId(p_TdcId),
    mLch(p_Lch),
    mTch(p_Tch),
    mPADIWAnr(p_PADIWAnr),
    mTRBnr(p_TRBnr),
    mPMTtype(p_PMTtype)
{

}
