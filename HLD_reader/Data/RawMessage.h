#ifndef RAWMESSAGE_H
#define RAWMESSAGE_H

#include <Rtypes.h>

class cls_RawMessage
{
public:
    cls_RawMessage(UInt_t p_tdc, UInt_t p_ch, UInt_t p_epoch, UInt_t p_coarse, UInt_t p_fine, Double_t p_fullTime);
    ~cls_RawMessage();

    bool operator< (const cls_RawMessage& rhs){ return this->mFullTime < rhs.mFullTime; }

    Bool_t IsSyncEdge(void);
    Bool_t IsLeadingEdge(void);
    Bool_t IsTrailingEdge(void);

public:
    UInt_t mTDC;
    UInt_t mChannel;
    UInt_t mEpoch;
    UInt_t mCoarse;
    UInt_t mFine;

    Double_t mFullTime;

};

#endif // RAWMESSAGE_H
