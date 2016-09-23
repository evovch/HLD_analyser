#ifndef GLOBAL_DEFINES_H
#define GLOBAL_DEFINES_H

#include <Rtypes.h>
#include <vector>
#include <string>

const UInt_t NUMTDCs = 68;
const UInt_t NUMCHs = 33;
const UInt_t NUMHITCHs = 16;

// Edge matching parameters
const Double_t PAIRNEGATIVEWIN = 10.;   // for negative ToT
const Double_t PAIRPOSITIVEWIN = 20.;   // for positive ToT

// Event building parameters
// laser
const Double_t EVENTNEGATIVEWINLASER =  3484060.;     // minus is added automatically. negative value here means positive!
const Double_t EVENTPOSITIVEWINLASER = -3483950.;

// hodo coincidence
const Double_t EVENTNEGATIVEWINHODO = 1000000.;     // minus is added automatically. negative value here means positive!
const Double_t EVENTPOSITIVEWINHODO = 1000000.;

const Double_t EVENTWINHISTOL =  3484060.;
const Double_t EVENTWINHISTOR = -3483950.;

// The value is actually the channel number
enum enu_triggerType {
    // TDC 0x0110
    kTDC0110sync = 0,
    kHodo1X = 1,
    kHodo1Y = 3,
    kHodoCoinc = 5,
    kLED = 7,
    kCh1disc = 9,
    kCh2disc = 11,
    kSpill = 13,
    kLaser = 15,
    // Aux
    kHodo1XT = 2,
    kHodo1YT = 4,
    kHodoCoincT = 6,
    kLEDT = 8,
    kCh1discT = 10,
    kCh2discT = 12,
    kSpillT = 14,
    kLaserT = 16
    /*
    // change to 33+0, 33+1, 33+2, etc...
    // TDC 0x0111
    kTDC0111sync = 33, // 33+0
    kCh1padiwaAmp = 17, // 16+1
    kCh1padiwaAmpT = 18, // 16+2
    kCh2padiwaAmp = 21, // 16+5
    kCh2padiwaAmpT = 22, // 16+6
    kLeadGlass = 25,    // 16+9
    kLeadGlassT = 26,    // 16+10
    // TDC 0x0113
    kLeadGlass2 = 57,    // 48+9
    kLeadGlass2T = 58    // 48+10
    */
};

// Functions
UInt_t TDCidToInteger(UInt_t tdcId);
UInt_t IntegerToTDCid(UInt_t index);
UInt_t PixelUID(UInt_t tdcIndex, UInt_t ch);
UInt_t PixelUIDtopRightQuarterOnly(UInt_t tdcIndex, UInt_t ch);
Bool_t IsTopRightQuarter(UInt_t index);
Bool_t IsH12700(UInt_t index);
Bool_t IsGoodPixel(UInt_t p_pixelID);

std::vector<std::string>
tokenize(const std::string & str, const std::string & delimiters);

#endif // GLOBAL_DEFINES_H
