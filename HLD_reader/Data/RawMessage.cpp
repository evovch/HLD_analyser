#include "RawMessage.h"

cls_RawMessage::cls_RawMessage(UInt_t p_tdc, UInt_t p_ch, UInt_t p_epoch, UInt_t p_coarse, UInt_t p_fine, Double_t p_fullTime) :
   mTDC(p_tdc),
   mChannel(p_ch),
   mEpoch(p_epoch),
   mCoarse(p_coarse),
   mFine(p_fine),
   mFullTime(p_fullTime)
{
}

cls_RawMessage::~cls_RawMessage()
{
}

Bool_t cls_RawMessage::IsSyncEdge(void)
{
    if (mChannel == 0) return kTRUE;
    return kFALSE;
}

Bool_t cls_RawMessage::IsLeadingEdge(void)
{
    if (mChannel == 0) return kFALSE;       // skip SYNC
    if (mChannel % 2 == 1) return kTRUE;
    return kFALSE;
}

Bool_t cls_RawMessage::IsTrailingEdge(void)
{
    if (mChannel == 0) return kFALSE;       // skip SYNC
    if (mChannel % 2 == 0) return kTRUE;
    return kFALSE;
}
