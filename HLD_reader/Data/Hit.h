#ifndef HIT_H
#define HIT_H

#include <Rtypes.h>

class cls_Hit
{
public:
    cls_Hit(UInt_t p_TDC, Bool_t p_hasLedge, Bool_t p_hasTedge, UInt_t p_LedgeCh, UInt_t p_TedgeCh, Double_t p_LedgeFullTime, Double_t p_TedgeFullTime);
    cls_Hit(char* p_buf);
    ~cls_Hit();

    UInt_t GetTDC(void) { return mTDC; }

    Double_t GetMainTime(void);

    Double_t GetLtime(void);
    Double_t GetTtime(void);
    Double_t GetToT(void);

    Int_t GetLch(void);
    Int_t GetTch(void);

    char* Serialize(char* o_buf);
    char* Deserialize(char* p_buf);

private:
    UInt_t mTDC;                        // 32 bits - 4 bytes

    ////////////////////////////////////// 6 empty bits
    Bool_t mHasLeadingEdge;             // 1 bit         1 byte
    Bool_t mHasTrailingEdge;            // 1 bit

    UInt_t mLeadingEdgeChannel;         // 32 bits - 4 bytes
    UInt_t mTrailingEdgeChannel;        // 32 bits - 4 bytes

    Double_t mLeadingEdgeFullTime;      // 64 bits - 8 bytes
    Double_t mTrailingEdgeFullTime;     // 64 bits - 8 bytes

};

#endif // HIT_H
