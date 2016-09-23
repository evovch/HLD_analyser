#ifndef CLS_EVENTSANALYSER_H
#define CLS_EVENTSANALYSER_H

#include <vector>
#include <QString>
#include "Event.h"

class TH1D;

class cls_EventsAnalyser
{
public:
    cls_EventsAnalyser();
    ~cls_EventsAnalyser();

    void RunAnalysis(void);

    void ProcessPerTDC(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_TdcId, UInt_t p_histoIndex);
    void ProcessPerPMT(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_PmtId, UInt_t p_histoIndex);
    void ProcessTRQ(std::vector<cls_Event>::iterator p_iterEvents);

    void ProcessPerTDCGood(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_TdcId, UInt_t p_histoIndex);
    void ProcessPerPMTGood(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_PmtId, UInt_t p_histoIndex);
    void ProcessTRQGood(std::vector<cls_Event>::iterator p_iterEvents);

    void ProcessLeadingEdgeDiff(std::vector<cls_Event>::iterator p_iterEvents);
    void ProcessLeadingEdgeDiffMean(void);

    UInt_t ExportHistos(TString p_filename);
    UInt_t ExportHistos(QString p_qfilename);

    UInt_t ExportCorrections(TString p_filename);
    UInt_t ExportCorrections(QString p_qfilename);

private:

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

};

#endif // CLS_EVENTSANALYSER_H
