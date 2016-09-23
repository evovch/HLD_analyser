#include "global_defines.h"

UInt_t TDCidToInteger(UInt_t tdcId)
{
    UInt_t v_tdcUID = ((((tdcId >> 4) & 0x00ff) - 1) * 4 + (tdcId & 0x000f));
    /*if (tdcId == 0x0100 || tdcId == 0x0101 || tdcId == 0x0102 || tdcId == 0x0103 ||
        tdcId == 0x0110 || tdcId == 0x0111 || tdcId == 0x0112 || tdcId == 0x0113) {
        printf ("%04x --> %d\n", tdcId, v_tdcUID);
    }*/
    return v_tdcUID;
}

UInt_t IntegerToTDCid(UInt_t index)
{
    return (((index/4) + 1) << 4) + (index%4);
}

// only leading edge number should be given as input
// 0 as channel is not a valid input
UInt_t PixelUID(UInt_t tdcIndex, UInt_t lch)
{
    return tdcIndex*16+lch/2;
}

// only leading edge number should be given as input
// 0 as channel is not a valid input
UInt_t PixelUIDtopRightQuarterOnly(UInt_t tdcIndex, UInt_t lch)
{
    UInt_t v_A = tdcIndex;
    if (v_A > 7) v_A -= 8;
    return v_A*16+lch/2;
}

// True for PMTs 1, 2, 5, 6
Bool_t IsTopRightQuarter(UInt_t index)
{
    UInt_t v_tdcId = IntegerToTDCid(index);
    if (v_tdcId == 0x0010 ||
        v_tdcId == 0x0011 ||
        v_tdcId == 0x0012 ||
        v_tdcId == 0x0013 ||
        v_tdcId == 0x0020 ||
        v_tdcId == 0x0021 ||
        v_tdcId == 0x0022 ||
        v_tdcId == 0x0023 ||
        v_tdcId == 0x0050 ||
        v_tdcId == 0x0051 ||
        v_tdcId == 0x0052 ||
        v_tdcId == 0x0053 ||
        v_tdcId == 0x0060 ||
        v_tdcId == 0x0061 ||
        v_tdcId == 0x0062 ||
        v_tdcId == 0x0063) {
        return kTRUE;
    }
    return kFALSE;
}

// True for PMTs 1, 2, 3, 5, 6, 7
Bool_t IsH12700(UInt_t index)
{
    UInt_t v_tdcId = IntegerToTDCid(index);
    if (v_tdcId == 0x0010 ||
        v_tdcId == 0x0011 ||
        v_tdcId == 0x0012 ||
        v_tdcId == 0x0013 ||
        v_tdcId == 0x0020 ||
        v_tdcId == 0x0021 ||
        v_tdcId == 0x0022 ||
        v_tdcId == 0x0023 ||
        v_tdcId == 0x0030 ||
        v_tdcId == 0x0031 ||
        v_tdcId == 0x0032 ||
        v_tdcId == 0x0033 ||
        v_tdcId == 0x0050 ||
        v_tdcId == 0x0051 ||
        v_tdcId == 0x0052 ||
        v_tdcId == 0x0053 ||
        v_tdcId == 0x0060 ||
        v_tdcId == 0x0061 ||
        v_tdcId == 0x0062 ||
        v_tdcId == 0x0063 ||
        v_tdcId == 0x0070 ||
        v_tdcId == 0x0071 ||
        v_tdcId == 0x0072 ||
        v_tdcId == 0x0073) {
        return kTRUE;
    }
    return kFALSE;
}

Bool_t IsGoodPixel(UInt_t p_pixelID)
{
    if (p_pixelID == 16 || p_pixelID == 20 || p_pixelID == 51 || p_pixelID == 52 || p_pixelID == 54 || p_pixelID == 66 || p_pixelID == 68 ||
        p_pixelID == 71 || p_pixelID == 78 || p_pixelID == 80 || p_pixelID == 82 || p_pixelID == 84 || p_pixelID == 85 || p_pixelID == 87 ||
        p_pixelID == 88 || p_pixelID == 90 || p_pixelID == 92 || p_pixelID == 94 || p_pixelID == 95 || p_pixelID == 96 || p_pixelID == 100 ||
        p_pixelID == 124 || p_pixelID == 126 || p_pixelID == 127 || p_pixelID == 151 || p_pixelID == 161 || p_pixelID == 162 || p_pixelID == 164 ||
        p_pixelID == 169 || p_pixelID == 184 || p_pixelID == 192 || p_pixelID == 193 || p_pixelID == 195 || p_pixelID == 197 || p_pixelID == 198 ||
        p_pixelID == 201 || p_pixelID == 202 || p_pixelID == 203 || p_pixelID == 208 || p_pixelID == 216 || p_pixelID == 224 || p_pixelID == 225 ||
        p_pixelID == 226 || p_pixelID == 227 || p_pixelID == 228 || p_pixelID == 229 || p_pixelID == 231 || p_pixelID == 244 ) {
        return kFALSE;
    }
    return kTRUE;
}

std::vector<std::string>
tokenize(const std::string & str, const std::string & delimiters)
{
  std::vector<std::string> tokens;

  // Skip delimiters at beginning.
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

  // Find first "non-delimiter".
  std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos)
  {
    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));

    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);

    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }

  return tokens;
}
