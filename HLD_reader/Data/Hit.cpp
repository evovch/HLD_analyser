#include "Hit.h"

cls_Hit::cls_Hit(UInt_t p_TDC, Bool_t p_hasLedge, Bool_t p_hasTedge, UInt_t p_LedgeCh, UInt_t p_TedgeCh, Double_t p_LedgeFullTime, Double_t p_TedgeFullTime) :
    mTDC(p_TDC),
    mHasLeadingEdge(p_hasLedge),
    mHasTrailingEdge(p_hasTedge),
    mLeadingEdgeChannel(p_LedgeCh),
    mTrailingEdgeChannel(p_TedgeCh),
    mLeadingEdgeFullTime(p_LedgeFullTime),
    mTrailingEdgeFullTime(p_TedgeFullTime)
{
}

cls_Hit::cls_Hit(char* p_buf)
{
    this->Deserialize(p_buf);
}

cls_Hit::~cls_Hit()
{
}

Double_t cls_Hit::GetMainTime(void)
{
    if (mHasLeadingEdge) return mLeadingEdgeFullTime;
    else if (mHasTrailingEdge) return mTrailingEdgeFullTime;
    else return 0.;
}

Double_t cls_Hit::GetLtime(void)
{
    if (mHasLeadingEdge) return mLeadingEdgeFullTime;
    return 0.;
}

Double_t cls_Hit::GetTtime(void)
{
    if (mHasTrailingEdge) return mTrailingEdgeFullTime;
    return 0.;
}

Double_t cls_Hit::GetToT(void)
{
    if (mHasLeadingEdge && mHasTrailingEdge) {
        return mTrailingEdgeFullTime - mLeadingEdgeFullTime;
    }
    return 0.;
}

Int_t cls_Hit::GetLch(void)
{
    if (mHasLeadingEdge) return mLeadingEdgeChannel;
    return -1;
}

Int_t cls_Hit::GetTch(void)
{
    if (mHasTrailingEdge) return mTrailingEdgeChannel;
    return -1;
}

char* cls_Hit::Serialize(char* o_buf)
{
    UInt_t v_cursor=0;

    char* v_tdc = reinterpret_cast<char*>(&mTDC);
    o_buf[v_cursor] = v_tdc[0]; v_cursor++;
    o_buf[v_cursor] = v_tdc[1]; v_cursor++;
    o_buf[v_cursor] = v_tdc[2]; v_cursor++;
    o_buf[v_cursor] = v_tdc[3]; v_cursor++;

    // Leave 1 byte for flags
    // Use two last bits for mHasLeadingEdge and mHasTrailingEdge

    o_buf[v_cursor] = 0x00 | (mHasLeadingEdge << 1) | (mHasTrailingEdge); v_cursor++;

    char* v_lch = reinterpret_cast<char*>(&mLeadingEdgeChannel);
    o_buf[v_cursor] = v_lch[0]; v_cursor++;
    o_buf[v_cursor] = v_lch[1]; v_cursor++;
    o_buf[v_cursor] = v_lch[2]; v_cursor++;
    o_buf[v_cursor] = v_lch[3]; v_cursor++;

    char* v_tch = reinterpret_cast<char*>(&mTrailingEdgeChannel);
    o_buf[v_cursor] = v_tch[0]; v_cursor++;
    o_buf[v_cursor] = v_tch[1]; v_cursor++;
    o_buf[v_cursor] = v_tch[2]; v_cursor++;
    o_buf[v_cursor] = v_tch[3]; v_cursor++;

    char* v_lts = reinterpret_cast<char*>(&mLeadingEdgeFullTime);
    o_buf[v_cursor] = v_lts[0]; v_cursor++;
    o_buf[v_cursor] = v_lts[1]; v_cursor++;
    o_buf[v_cursor] = v_lts[2]; v_cursor++;
    o_buf[v_cursor] = v_lts[3]; v_cursor++;
    o_buf[v_cursor] = v_lts[4]; v_cursor++;
    o_buf[v_cursor] = v_lts[5]; v_cursor++;
    o_buf[v_cursor] = v_lts[6]; v_cursor++;
    o_buf[v_cursor] = v_lts[7]; v_cursor++;

    char* v_tts = reinterpret_cast<char*>(&mTrailingEdgeFullTime);
    o_buf[v_cursor] = v_tts[0]; v_cursor++;
    o_buf[v_cursor] = v_tts[1]; v_cursor++;
    o_buf[v_cursor] = v_tts[2]; v_cursor++;
    o_buf[v_cursor] = v_tts[3]; v_cursor++;
    o_buf[v_cursor] = v_tts[4]; v_cursor++;
    o_buf[v_cursor] = v_tts[5]; v_cursor++;
    o_buf[v_cursor] = v_tts[6]; v_cursor++;
    o_buf[v_cursor] = v_tts[7]; v_cursor++;

    return &o_buf[v_cursor];
}

char* cls_Hit::Deserialize(char* p_buf)
{
    UInt_t v_cursor=0;

    UInt_t* v_tdc = reinterpret_cast<UInt_t*>(&p_buf[v_cursor]);    v_cursor+=4;
    mTDC = *v_tdc;

    mHasLeadingEdge = (p_buf[v_cursor] >> 1) & 0x1;
    mHasTrailingEdge = (p_buf[v_cursor] >> 0) & 0x1;
    v_cursor++;

    UInt_t* v_lch = reinterpret_cast<UInt_t*>(&p_buf[v_cursor]);    v_cursor+=4;
    mLeadingEdgeChannel = *v_lch;

    UInt_t* v_tch = reinterpret_cast<UInt_t*>(&p_buf[v_cursor]);    v_cursor+=4;
    mTrailingEdgeChannel = *v_tch;

    Double_t* v_lts = reinterpret_cast<Double_t*>(&p_buf[v_cursor]);  v_cursor+=8;
    mLeadingEdgeFullTime = *v_lts;

    Double_t* v_tts = reinterpret_cast<Double_t*>(&p_buf[v_cursor]);  v_cursor+=8;
    mTrailingEdgeFullTime = *v_tts;

    return &p_buf[v_cursor];
}
