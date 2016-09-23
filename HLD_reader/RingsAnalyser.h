#ifndef CLS_RINGSANALYSER_H
#define CLS_RINGSANALYSER_H

#include <map>
#include <vector>

#include <QString>

#include <Rtypes.h>
#include <TString.h>

#include "PixelMapEntry.h"
#include "Hit.h"
#include "Event.h"

class TH1D;
class TH2D;
class CbmRichRingLight;

class cls_RingsAnalyser
{
public:
    cls_RingsAnalyser();
    ~cls_RingsAnalyser();

    UInt_t ImportPixelMap(QString p_filename="../pixel-coord_channel-register.ascii");
    void RunAnalysis(void);
    void ProcessOneRing(CbmRichRingLight* p_theRing, std::vector<cls_Hit>& p_eventHits);

    // Copied from the EventsAnalysis class -----------------------------------
    void ProcessPerTDC(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_TdcId, UInt_t p_histoIndex);
    void ProcessPerPMT(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_PmtId, UInt_t p_histoIndex);
    void ProcessTRQ(std::vector<cls_Event>::iterator p_iterEvents);

    void ProcessPerTDCGood(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_TdcId, UInt_t p_histoIndex);
    void ProcessPerPMTGood(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_PmtId, UInt_t p_histoIndex);
    void ProcessTRQGood(std::vector<cls_Event>::iterator p_iterEvents);

    void ProcessLeadingEdgeDiff(std::vector<cls_Event>::iterator p_iterEvents);
    void ProcessLeadingEdgeDiffMean(void);
    // ------------------------------------------------------------------------

    UInt_t ExportHistos(TString p_filename);
    UInt_t ExportHistos(QString p_qfilename);

    // Pixel map
    std::map<UInt_t, cls_PixelMapEntry> fPixelMap;

    TH1D* fhNumOfFoundRings;
    TH1D* fhNumOfHitsInTheRing;
    TH2D* fhRingsCenter;
    TH1D* fhRingsRadius;
    TH1D* fhNumOfHitsInTheRingInCut;
    TH2D* fhRingsCenterInCut;
    TH1D* fhRingsRadiusInCut;

private:

    // Copied from the EventsAnalysis class -----------------------------------

    // ----------------------------------------------------------------------------------------------------------------------------------
    // Without filtering bad channels

    TH1D* fhDiffFirstInTimeOnePMT[4];
    TH1D* fhDiffFirstInTimeOnePADIWA[16];
    TH1D* fhDiffFirstInTimeTopRightQuarter;

    // Only for top-right quarter
    TH1D* fhLeadingEdgeDiffMean;                // Mean of all fhLeadingEdgeDiff distributions for 4 top-right quarter PMTs
    TH1D* fhLeadingEdgeDiffOnePMT[4];           // Sum for 4 top-right quarter PMTs separately
    TH1D* fhLeadingEdgeDiffOnePADIWA[16];       // Sum for 16 top-right quarter PADIWAs separately
    TH1D* fhLeadingEdgeDiffTopRightQuarter;     // Sum for 4 top-right quarter PMTs together - same as 16 top-right quarter PADIWAs

    // ----------------------------------------------------------------------------------------------------------------------------------
    // With filtering bad channels

    TH1D* fhDiffFirstInTimeOnePMTGood[4];
    TH1D* fhDiffFirstInTimeOnePADIWAGood[16];
    TH1D* fhDiffFirstInTimeTopRightQuarterGood;

    // Only for top-right quarter
    TH1D* fhLeadingEdgeDiffMeanGood;            // Mean of all fhLeadingEdgeDiff distributions for 4 top-right quarter PMTs
    TH1D* fhLeadingEdgeDiffOnePMTGood[4];       // Sum for 4 top-right quarter PMTs separately
    TH1D* fhLeadingEdgeDiffOnePADIWAGood[16];   // Sum for 16 top-right quarter PADIWAs separately
    TH1D* fhLeadingEdgeDiffTopRightQuarterGood; // Sum for 4 top-right quarter PMTs together

    // ----------------------------------------------------------------------------------------------------------------------------------
    // fhLeadingEdgeDiff for each pair of pixels is here after all the other histograms

    TH1D* fhLeadingEdgeDiff[256][256];          // For each pair

    // ------------------------------------------------------------------------

};

#endif // CLS_RINGSANALYSER_H
