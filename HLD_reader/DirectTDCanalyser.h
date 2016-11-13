#ifndef CLS_DIRECTTDCANALYSER_H
#define CLS_DIRECTTDCANALYSER_H

#include <list>

#include <QString>
#include <TString.h>
#include <TH1D.h>

#include "global_defines.h"
#include "RawMessage.h"
#include "Hit.h"

class cls_DirectTDCanalyser
{
public:
    cls_DirectTDCanalyser();
    ~cls_DirectTDCanalyser();

    void AddMessage(cls_RawMessage p_mess);

    void RunPostAnalysis(void);

    UInt_t ExportHistos(TString p_filename);
    UInt_t ExportHistos(QString p_qfilename);

private:
    std::list<cls_RawMessage> fInputMessages;

    std::list<cls_Hit> fOutputHits[NUMTDCs-4][NUMHITCHs];

private:
    TH1D* fhToT[16];
    TH1D* fhLeadingEdgeDiff[16];

    TH1D* fhSyncMessages;
    TH1D* fhErasedLeadingEdges;
    TH1D* fhErasedTrailingEdges;
    TH1D* fhFoundPairs;

};

#endif // CLS_DIRECTTDCANALYSER_H
