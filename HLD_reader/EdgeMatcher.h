#ifndef EDGEMATCHER_H
#define EDGEMATCHER_H

#include <list>
#include <map>

#include <QString>

#include "global_defines.h"
#include "RawMessage.h"
#include "Hit.h"
#include "Event.h"

class TH1D;

class cls_EdgeMatcher
{
public: // methods
    cls_EdgeMatcher();
    ~cls_EdgeMatcher();

    void AddInputMessage(cls_RawMessage p_mess) { fInputMessages[p_mess.mTDC][p_mess.mChannel].push_back(p_mess); }

    void SetTrigger(enu_triggerType p_type) { fTriggerType = p_type; }

    void Process(void);
    void MatchEdges(UInt_t p_tdc, UInt_t p_ch);
    void ProcessSyncs(UInt_t p_tdc);
    void BuildEvents(void);

    UInt_t ExportMatchedEdges(TString p_filename);
    UInt_t ExportMatchedEdges(QString p_qfilename);

    UInt_t ImportMatchedEdges(TString p_filename);
    UInt_t ImportMatchedEdges(QString p_qfilename);

    UInt_t ExportEventBuildingHistos(TString p_filename);
    UInt_t ExportEventBuildingHistos(QString p_qfilename);

    UInt_t ExportHistos(TString p_filename);
    UInt_t ExportHistos(QString p_qfilename);

private: // data members
    // Input raw messages orginized per-channel.
    // This allows parallel processing without any additional cost.
    std::list<cls_RawMessage> fInputMessages[NUMTDCs][NUMCHs];

    // Output hits orginized per-channel.
    std::list<cls_Hit> fOutputHits[NUMTDCs-4][NUMHITCHs];
    std::list<cls_Hit> fOutputHitsBeamDetectors[4][NUMCHs];
    std::list<cls_Hit> fOutputHitsSync[NUMTDCs];

private: // data members
    static UInt_t fVerbosityLevel;

    TH1D* fhToT[NUMTDCs][NUMHITCHs];

    // Event building statistics
    TH1D* fhTriggerCorrelation;
    TH1D* fhTriggerCorrelationInCut;
    TH1D* fhNumOfHitsInEvent;

    // Basic edge matching statistics
    TH1D* fhMessagesPerLchannel;
    TH1D* fhMessagesPerTchannel;
    TH1D* fhPairsPerChannel;
    TH1D* fhTedgesWithoutLedgeChannel;
    TH1D* fhMultipleLedgesPerChannel;

    enu_triggerType fTriggerType;

};

#endif // EDGEMATCHER_H
