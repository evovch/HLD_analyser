#include "DirectTDCanalyser.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <TDirectory.h>
#include <TFile.h>

cls_DirectTDCanalyser::cls_DirectTDCanalyser()
{
    TString histoName;
    TString histoTitle;
    for (UInt_t i=0; i<16; i++) {
        histoName.Form("ToT_ch%d", i+1);
        histoTitle.Form("ToT_ch%d", i+1);
        fhToT[i] = new TH1D(histoName, histoTitle, 100*(DIRECTTDCPAIRPOSWIN+DIRECTTDCPAIRNEGWIN), -DIRECTTDCPAIRNEGWIN, DIRECTTDCPAIRPOSWIN);
        histoName.Form("LeadingEdgeDiff_ch%d", i+1);
        histoTitle.Form("LeadingEdgeDiff_ch%d", i+1);
        fhLeadingEdgeDiff[i] = new TH1D(histoName, histoTitle, 100.*4., -2., 2.);
    }

    fhSyncMessages = new TH1D("fhSyncMessages", "fhSyncMessages", NUMTDCs-4, 0., (Double_t)(NUMTDCs-4));
    fhErasedLeadingEdges = new TH1D("fhErasedLeadingEdges", "fhErasedLeadingEdges", (NUMTDCs-4)*NUMCHs, 0., (Double_t)((NUMTDCs-4)*NUMCHs));
    fhErasedTrailingEdges = new TH1D("fhErasedTrailingEdges", "fhErasedTrailingEdges", (NUMTDCs-4)*NUMCHs, 0., (Double_t)((NUMTDCs-4)*NUMCHs));
    fhFoundPairs = new TH1D("fhFoundPairs", "fhFoundPairs", (NUMTDCs-4)*NUMCHs, 0., (Double_t)((NUMTDCs-4)*NUMCHs));
}

cls_DirectTDCanalyser::~cls_DirectTDCanalyser()
{
    for (UInt_t i=0; i<16; i++) {
        if (fhToT[i]) { delete fhToT[i]; fhToT[i]=nullptr; }
        if (fhLeadingEdgeDiff[i]) { delete fhLeadingEdgeDiff[i]; fhLeadingEdgeDiff[i]=nullptr; }
    }
}

void cls_DirectTDCanalyser::AddMessage(cls_RawMessage p_mess)
{
    UInt_t v_tdcId = IntegerToTDCid(p_mess.mTDC);

    // Skip data from TDCs 0x0110, 0x0111, 0x0113
    if (v_tdcId == 0x0110 || v_tdcId == 0x0111 || v_tdcId == 0x0113) {
        return;
    }

    if (p_mess.IsSyncEdge()) {
        fhSyncMessages->Fill(p_mess.mTDC);

    } else if (p_mess.IsLeadingEdge()) {
        // If the input message is the leading edge - just store it
        fInputMessages.push_back(p_mess);

    } else if (p_mess.IsTrailingEdge()) {
        // Process trailing edge

        // Get the current trailing edge timestamp and define the window
        Double_t tTime = p_mess.mFullTime;

        // Yes, it is correct to subtract the 'positive' side of the window and add the 'negative side
        // Lookig to the left (in negative direction) from the trailing edge gives positive ToT.
        Double_t leftBound = tTime - DIRECTTDCPAIRPOSWIN;
        Double_t rightBound = tTime + DIRECTTDCPAIRNEGWIN;

        // Loop over the leading edges in the buffer
        // The difference with the "standard" edge matcher here is that there is just one
        // buffer for all leadging edges - there is no splitting for different channels.
        std::list<cls_RawMessage>::iterator lIter;
        for (lIter = fInputMessages.begin(); lIter != fInputMessages.end(); /*++lIter*/) {

            Double_t lTime = lIter->mFullTime;

            // Condition to stop searching
            if (lTime < leftBound) {
                //TODO do something
                //if (p_mess.mChannel == lIter->mChannel + 1)
                {
                    fhErasedLeadingEdges->Fill(lIter->mTDC*NUMCHs + lIter->mChannel);
                    lIter = fInputMessages.erase(lIter);
                }
                continue;
            }
            if (lTime > rightBound) break;
            if (lTime >= leftBound && lTime <= rightBound) {
                // Edge is in window!
                if (p_mess.mChannel == lIter->mChannel + 1) {

                    // Fill time-over-threshold histogram
                    fhToT[p_mess.mChannel/2-1]->Fill(tTime-lTime);

                    fhFoundPairs->Fill(p_mess.mTDC*NUMCHs + p_mess.mChannel); // trailing
                    fhFoundPairs->Fill(lIter->mTDC*NUMCHs + lIter->mChannel); // leading

                    fOutputHits[p_mess.mTDC][p_mess.mChannel/2-1].push_back(cls_Hit(p_mess.mTDC, kTRUE, kTRUE, lIter->mChannel, p_mess.mChannel, lTime, tTime));
                    lIter = fInputMessages.erase(lIter);
                }
            }

        }

    }

}

// Very custom analysis here...
void cls_DirectTDCanalyser::RunPostAnalysis(void)
{
    UInt_t v_studiedTDC = 0;
    UInt_t v_refChannel = 0;
    UInt_t v_studiedChannel;

    for (UInt_t v_ch=1; v_ch<NUMHITCHs; v_ch++) {
        if (fOutputHits[v_studiedTDC][v_ch].size() > 0) {
            v_studiedChannel = v_ch;
        }
    }

    std::list<cls_Hit>::iterator hitIter;
    std::list<cls_Hit>::iterator refIter;
    for (hitIter = fOutputHits[v_studiedTDC][v_studiedChannel].begin(); hitIter!=fOutputHits[v_studiedTDC][v_studiedChannel].end(); ++hitIter) {

        Double_t studiedLtime = hitIter->GetLtime();

        for (refIter = fOutputHits[v_studiedTDC][v_refChannel].begin(); refIter!=fOutputHits[v_studiedTDC][v_refChannel].end(); /*++refIter*/) {

            Double_t refLtime = refIter->GetLtime();

            if (refLtime < studiedLtime - 2.) {
                refIter = fOutputHits[v_studiedTDC][v_refChannel].erase(refIter);
            }
            if (refLtime > studiedLtime + 2.) {
                break;
            }
            if (refLtime >= studiedLtime - 2. && refLtime <= studiedLtime + 2.) {
                fhLeadingEdgeDiff[v_studiedChannel]->Fill(studiedLtime-refLtime);
                refIter = fOutputHits[v_studiedTDC][v_refChannel].erase(refIter);
                break;
            }

        }

    }
}

UInt_t cls_DirectTDCanalyser::ExportHistos(TString p_filename)
{
    TDirectory* prevDir = gDirectory;
    TFile* prevFile = gFile;

    TFile v_outputFile(p_filename, "RECREATE");

    if (v_outputFile.IsZombie()) {
        cerr << "Error opening file " << p_filename << endl;
        gDirectory = prevDir;
        gFile = prevFile;
        return 1; // FAIL
    }

    fhSyncMessages->Write();
    fhErasedLeadingEdges->Write();
    fhErasedTrailingEdges->Write();
    fhFoundPairs->Write();

    for (UInt_t i=0; i<16; i++) {
        fhToT[i]->Write();
    }
    for (UInt_t i=0; i<16; i++) {
        fhLeadingEdgeDiff[i]->Write();
    }

    v_outputFile.Close();

    cout << "Successfully exported direct TDC measurements analysis results into " << p_filename << "." << endl;

    gDirectory = prevDir;
    gFile = prevFile;
    return 0; // OK
}

/* Just a wrapper */
UInt_t cls_DirectTDCanalyser::ExportHistos(QString p_qfilename)
{
    char v_cfilename[255];
    strcpy(v_cfilename, p_qfilename.toStdString().c_str());
    TString v_filename(v_cfilename);
    return this->ExportHistos(v_filename);
}
