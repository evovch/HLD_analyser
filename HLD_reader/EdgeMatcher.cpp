#include "EdgeMatcher.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <fstream>
#include <sstream>
using std::stringstream;

#include <TH1D.h>
#include <TString.h>
#include <TDirectory.h>
#include <TFile.h>

#include <ctime>
#include <cstdio>
#include <algorithm>
#include <omp.h>

#include "DataHandler.h"

/*#include "GraphWidget.h"*/

UInt_t cls_EdgeMatcher::fVerbosityLevel = 1;

cls_EdgeMatcher::cls_EdgeMatcher()
{
    // Edge matching histograms
    TString histoName;
    TString histoTitle;
    for (UInt_t v_tdc=0; v_tdc<NUMTDCs; v_tdc++) {
        for (UInt_t v_ch=0; v_ch<NUMHITCHs; v_ch++) {
            histoName.Form("fhToT_TDC_%d_pch_%d", v_tdc, v_ch);
            histoTitle.Form("fhToT_TDC_%d_pch_%d", v_tdc, v_ch);
            fhToT[v_tdc][v_ch] = new TH1D(histoName.Data(), histoTitle.Data(), 600, -PAIRNEGATIVEWIN, PAIRPOSITIVEWIN);
        }
    }

    UInt_t nbins = (NUMTDCs-4)*NUMHITCHs;

    fhMessagesPerLchannel = new TH1D("fhMessagesPerLchannel", "fhMessagesPerLchannel", nbins, 0., (Double_t)nbins);
    fhMessagesPerTchannel = new TH1D("fhMessagesPerTchannel", "fhMessagesPerTchannel", nbins, 0., (Double_t)nbins);
    fhPairsPerChannel = new TH1D("fhPairsPerChannel", "fhPairsPerChannel", nbins, 0., (Double_t)nbins);
    fhTedgesWithoutLedgeChannel = new TH1D("fhSingleEdgesPerChannel", "fhSingleEdgesPerChannel", nbins, 0., (Double_t)nbins);
    fhMultipleLedgesPerChannel = new TH1D("fhMultipleLedgesPerChannel", "fhMultipleLedgesPerChannel", nbins, 0., (Double_t)nbins);

    // Event building histograms
    fhTriggerCorrelation = new TH1D("fhTriggerCorrelation", "fhTriggerCorrelation", 2000, -EVENTWINHISTOL, EVENTWINHISTOR);
    fhTriggerCorrelationInCut = new TH1D("fhTriggerCorrelationInCut", "fhTriggerCorrelationInCut", 2000, -EVENTWINHISTOL, EVENTWINHISTOR);
    fhNumOfHitsInEvent = new TH1D("fhNumOfHitsInEvent", "fhNumOfHitsInEvent", 250, 0., 250.);

    //mAnalyser = new cls_EventsAnalyser();
}

cls_EdgeMatcher::~cls_EdgeMatcher()
{
    for (UInt_t v_tdc=0; v_tdc<NUMTDCs; v_tdc++) {
        for (UInt_t v_ch=0; v_ch<NUMHITCHs; v_ch++) {
            delete fhToT[v_tdc][v_ch];
        }
    }

    delete fhMessagesPerLchannel;
    delete fhMessagesPerTchannel;
    delete fhPairsPerChannel;
    delete fhTedgesWithoutLedgeChannel;
    delete fhMultipleLedgesPerChannel;

    delete fhTriggerCorrelation;
    delete fhTriggerCorrelationInCut;
    delete fhNumOfHitsInEvent;

   // delete mAnalyser;
}

void cls_EdgeMatcher::Process(void)
{
    cout << "cls_EdgeMatcher::Process" << endl;

    // ----------------------------------------------------------------------

    cout << "Sorting..." << endl;
    std::clock_t v_timer_start = std::clock();

    #pragma omp parallel for
    for (UInt_t v_tdc=0; v_tdc<NUMTDCs; v_tdc++) {
        for (UInt_t v_ch=0; v_ch<NUMCHs; v_ch++) {
            if (fInputMessages[v_tdc][v_ch].size() > 0)
                std::sort(fInputMessages[v_tdc][v_ch].begin(), fInputMessages[v_tdc][v_ch].end());
        }
    }

    std::clock_t v_timer_end = std::clock();
    cout << "Done sorting. " << v_timer_end-v_timer_start << endl;

    // ----------------------------------------------------------------------

    /*cls_GraphWidget* grWin = new cls_GraphWidget();
    grWin->FillFromData(fInputMessages);
    grWin->show();*/

    // ----------------------------------------------------------------------

    /*FILE* outputFile = fopen("dump.txt", "w");
    for (auto iter: fInputMessages) {
        fprintf (outputFile, "%f:\tTDC %04x ch %02d\n", iter.mFullTime, iter.mTDC, iter.mChannel);
    }
    fclose(outputFile);
    cout << "Dump finished" << endl;*/

    // ----------------------------------------------------------------------

    cout << "Matching edges..." << endl;
    v_timer_start = std::clock();

    // For lab data analysis - process sync messages as trigger messages
    //TODO - select between the beamtime data and lab data!
    for (UInt_t v_tdc=0; v_tdc<NUMTDCs; v_tdc++) {
        this->ProcessSyncs(v_tdc);
    }

    #pragma omp parallel for
    for (UInt_t v_tdc=0; v_tdc<NUMTDCs; v_tdc++) {
        for (UInt_t v_ch=2; v_ch<NUMCHs; v_ch+=2) {     // Skip 0 - sync channel. Loop over trailing only. Update, see a few lines above.
            this->MatchEdges(v_tdc, v_ch);
        }
    }

    v_timer_end = std::clock();
    cout << "Done matching edges. " << v_timer_end-v_timer_start << endl;

    cls_DataHandler* v_dataHandler = cls_DataHandler::Instance();
    std::multimap<Double_t, cls_Hit>& v_mapOfHits = v_dataHandler->fMapOfHits;

    cout << "Merging vectors..." << endl;
    v_timer_start = std::clock();

    // Merge separate vectors of hits into a single vector
    for (UInt_t v_tdc=0; v_tdc<NUMTDCs-4; v_tdc++) {
        for (UInt_t v_ch=0; v_ch<NUMHITCHs; v_ch++) {
            //cout << "TDC " << v_tdc << " ch " << v_ch << endl;
            std::vector<cls_Hit>::iterator iter;
            for (iter=fOutputHits[v_tdc][v_ch].begin(); iter!=fOutputHits[v_tdc][v_ch].end(); ++iter) {
                v_mapOfHits.insert(std::pair<Double_t, cls_Hit>(iter->GetMainTime(), *iter));
            }
        }
    }

    v_timer_end = std::clock();
    cout << "Done merging vectors. " << v_timer_end-v_timer_start << endl;

    // ----------------------------------------------------------------------
}

void cls_EdgeMatcher::ProcessSyncs(UInt_t p_tdc)
{
    //cout << "Sync channel vector size: " << fInputMessages[p_tdc][0].size() << endl;
    //unsigned int v_counter = 0;

    // loop over the sync messages
    std::vector<cls_RawMessage>::iterator lIter;
    for (lIter=fInputMessages[p_tdc][0].begin(); lIter!=fInputMessages[p_tdc][0].end(); lIter++)
    {
        //if (v_counter%1000 == 0) cout << "counter=" << v_counter << endl;
        //v_counter++;

        fOutputHitsSync[p_tdc].push_back(cls_Hit(p_tdc, kTRUE, kFALSE, 0, 0xffffffff, lIter->mFullTime, 0.));
        //lIter = fInputMessages[p_tdc][0].erase(lIter); // here we go to the next sync message
    }

    fInputMessages[p_tdc][0].clear();
}

UInt_t cls_EdgeMatcher::ExportHistos(TString p_filename)
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

    fhMessagesPerLchannel->Write();
    fhMessagesPerTchannel->Write();
    fhPairsPerChannel->Write();
    fhTedgesWithoutLedgeChannel->Write();
    fhMultipleLedgesPerChannel->Write();

    for (UInt_t v_tdc=0; v_tdc<NUMTDCs; v_tdc++) {
        UInt_t v_tdcId = IntegerToTDCid(v_tdc);
        TString dirName;
        dirName.Form("TDC%04x", v_tdcId);
        gDirectory->mkdir(dirName);
        gDirectory->cd(dirName);
        for (UInt_t v_ch=0; v_ch<NUMHITCHs; v_ch++) {
            fhToT[v_tdc][v_ch]->Write();
        }
        gDirectory->cd("..");
    }

    v_outputFile.Close();

    cout << "Successfully exported edge matching info into " << p_filename << "." << endl;

    gDirectory = prevDir;
    gFile = prevFile;
    return 0; // OK
}

/* Just a wrapper */
UInt_t cls_EdgeMatcher::ExportHistos(QString p_qfilename)
{
    char v_cfilename[255];
    strcpy(v_cfilename, p_qfilename.toStdString().c_str());
    TString v_filename(v_cfilename);
    return this->ExportHistos(v_filename);
}

// Trailing edge channel as input
void cls_EdgeMatcher::MatchEdges(UInt_t p_tdc, UInt_t p_ch)
{
    UInt_t tchannel = p_ch;
    UInt_t lchannel = p_ch - 1;
    UInt_t v_tdcId = IntegerToTDCid(p_tdc);

    stringstream v_logMessage;

    if (fVerbosityLevel > 0) {
        v_logMessage << "Matching edges for TDC 0x" << std::hex << v_tdcId << std::dec << " ch " << tchannel;
        v_logMessage << ".\tTotal " << fInputMessages[p_tdc][tchannel].size() << " trailing edges in the buffer.\t";
    }

    UInt_t v_counterOfPairs = 0;
    UInt_t v_counterOfTedgesWithoutLedge = 0;
    UInt_t v_counterOfLedgesWithoutTedge = 0;
    UInt_t v_counterOfTedgesWithMultipleLedges = 0;

    // All fInputMessages are sorted beforehand

    // Special case for TDCs 0x0110, 0x0111, 0x0113
    // Do not look for corresponding leading edge, but flush leading and trailing edges separately
    if (v_tdcId == 0x0110 || v_tdcId == 0x0111 || v_tdcId == 0x0113) {

        // loop over leading edges
        std::vector<cls_RawMessage>::iterator lIter;
        for (lIter=fInputMessages[p_tdc][lchannel].begin(); lIter!=fInputMessages[p_tdc][lchannel].end(); /*lIter++*/)
        {
            fOutputHitsBeamDetectors[p_tdc-64][lchannel].push_back(cls_Hit(p_tdc, kTRUE, kFALSE, lchannel, 0xffffffff, lIter->mFullTime, 0.));
            lIter = fInputMessages[p_tdc][lchannel].erase(lIter); // here we go to the next leading edge
            v_counterOfLedgesWithoutTedge++;
        }
        // loop over trailing edges
        std::vector<cls_RawMessage>::iterator tIter;
        for (tIter=fInputMessages[p_tdc][tchannel].begin(); tIter!=fInputMessages[p_tdc][tchannel].end(); /*tIter++*/)
        {
            fOutputHitsBeamDetectors[p_tdc-64][tchannel].push_back(cls_Hit(p_tdc, kFALSE, kTRUE, 0xffffffff, tchannel, 0., tIter->mFullTime));
            tIter = fInputMessages[p_tdc][tchannel].erase(tIter); // here we go to the next trailing edge
            v_counterOfTedgesWithoutLedge++;
        }

        if (fVerbosityLevel > 0) {
            v_logMessage << "0 pairs found;\t" << v_counterOfTedgesWithoutLedge << " trailing edges without a pair;\t"
                 << v_counterOfLedgesWithoutTedge << " leading edges without a pair." << endl;
            cout << v_logMessage.str();
        }

        return;
    }

    // Only for PMT TDCs (NOT beam detectors - 0x0110-0x0113)

    fhMessagesPerTchannel->SetBinContent(p_tdc*NUMHITCHs + tchannel/2 - 1, fInputMessages[p_tdc][tchannel].size());
    fhMessagesPerLchannel->SetBinContent(p_tdc*NUMHITCHs + lchannel/2, fInputMessages[p_tdc][lchannel].size());

    // Loop over the trailing edges in the buffer
    std::vector<cls_RawMessage>::iterator tIter;
    for (tIter=fInputMessages[p_tdc][tchannel].begin(); tIter!=fInputMessages[p_tdc][tchannel].end(); /*tIter++*/)
    {
        // Get the current trailing edge timestamp and define the window
        Double_t tTime = tIter->mFullTime;
        Double_t leftBound = tTime - PAIRPOSITIVEWIN;
        Double_t rightBound = tTime + PAIRNEGATIVEWIN;

        UInt_t v_numOfEdgesFound=0;
        std::vector<cls_RawMessage>::iterator lIter;
        std::vector<cls_RawMessage>::iterator lastLiter;
        // Loop over the leading edges
        // Search for the edge in the window
        // Count them and store last found leading edge
        for (lIter = fInputMessages[p_tdc][lchannel].begin(); lIter!=fInputMessages[p_tdc][lchannel].end(); ++lIter) {
            Double_t lTime = lIter->mFullTime;
            // Condition to stop searching
            if (lTime > rightBound) break;
            if (lTime >= leftBound && lTime <= rightBound) {
                // Edge is in window! Count it.
                v_numOfEdgesFound++;
                lastLiter = lIter;
            }
        }

        // FIXME thread-safe cout
        if (fVerbosityLevel > 1) cout << "Found " << v_numOfEdgesFound << " leading edges in the window" << endl;

        if (v_numOfEdgesFound == 1) {
            // If only one edge found, use it. And remove both edges from the input vectors.
            fOutputHits[p_tdc][tchannel/2-1].push_back(cls_Hit(p_tdc, kTRUE, kTRUE, lchannel, tchannel, lastLiter->mFullTime, tTime));
            fhToT[p_tdc][tchannel/2-1]->Fill(tTime - lastLiter->mFullTime);
            fInputMessages[p_tdc][lchannel].erase(lastLiter);
            tIter = fInputMessages[p_tdc][tchannel].erase(tIter); // here we go to the next trailing edge
            v_counterOfPairs++;
        } else {
            if (v_numOfEdgesFound == 0) v_counterOfTedgesWithoutLedge++;
            if (v_numOfEdgesFound > 1) v_counterOfTedgesWithMultipleLedges++;
            ++tIter; // In any case go to the next trailing edge
        }
    }

    if (fVerbosityLevel > 0) {
        v_logMessage << v_counterOfPairs << " pairs found;\t" << v_counterOfTedgesWithoutLedge << " trailing edges without a pair;\t"
             << v_counterOfTedgesWithMultipleLedges << " trailing edges with multiple leading edges." << endl;
        cout << v_logMessage.str();
    }

    fhPairsPerChannel->SetBinContent(p_tdc*NUMHITCHs + tchannel/2 - 1, v_counterOfPairs);
    fhTedgesWithoutLedgeChannel->SetBinContent(p_tdc*NUMHITCHs + tchannel/2 - 1, v_counterOfTedgesWithoutLedge);
    fhMultipleLedgesPerChannel->SetBinContent(p_tdc*NUMHITCHs + tchannel/2 - 1, v_counterOfTedgesWithMultipleLedges);

}

UInt_t cls_EdgeMatcher::ExportMatchedEdges(TString p_filename)
{
    std::ofstream v_outFile (p_filename.Data(), std::ios::out | std::ios::binary);

    if (!v_outFile.is_open()) {
        cerr << "Error opening file " << p_filename.Data() << endl;
        return 1; // FAIL
    }

    cout << "Exporting matched edges..." << endl;

    cls_DataHandler* v_dataHandler = cls_DataHandler::Instance();
    std::multimap<Double_t, cls_Hit>& v_mapOfHits = v_dataHandler->fMapOfHits;

    // HEADER -----------------------------------------------------------------
    // First comes the header of the file: 4*33+1 integers

    UInt_t v_size1[4][33];
    for (UInt_t i=0; i<4; i++) {
        for (UInt_t j=0; j<33; j++) {
            v_size1[i][j] = fOutputHitsBeamDetectors[i][j].size();
            v_outFile.write(reinterpret_cast<char*>(&v_size1[i][j]), 4);
        }
    }

    UInt_t v_size2 = v_mapOfHits.size();
    v_outFile.write(reinterpret_cast<char*>(&v_size2), 4);

    // ------------------------------------------------------------------------

    // DATA - beam detectors
    for (UInt_t i=0; i<4; i++) {
        for (UInt_t j=0; j<33; j++) {
            std::vector<cls_Hit>::iterator iterBeamSignals;
            for (iterBeamSignals=fOutputHitsBeamDetectors[i][j].begin();
                 iterBeamSignals!=fOutputHitsBeamDetectors[i][j].end(); ++iterBeamSignals)
            {
                char v_buf[29];
                (*iterBeamSignals).Serialize(v_buf);
                v_outFile.write(v_buf, 29);
            }

        }
    }
    // ------------------------------------------------------------------------

    // DATA - hits
    std::multimap<Double_t, cls_Hit>::iterator iterCameraHits;
    for (iterCameraHits=v_mapOfHits.begin(); iterCameraHits!=v_mapOfHits.end(); ++iterCameraHits)
    {
        char v_buf[29];
        (*iterCameraHits).second.Serialize(v_buf);
        v_outFile.write(v_buf, 29);
    }

    // ------------------------------------------------------------------------

    // New section - sync messages
    // Header
    UInt_t v_size3[NUMTDCs];
    for (UInt_t i=0; i<NUMTDCs; i++) {
        v_size3[i] = fOutputHitsSync[i].size();
        v_outFile.write(reinterpret_cast<char*>(&v_size3[i]), 4);
    }

    // Data
    for (UInt_t i=0; i<NUMTDCs; i++) {
        std::vector<cls_Hit>::iterator iterSyncSignals;
        for (iterSyncSignals=fOutputHitsSync[i].begin(); iterSyncSignals!=fOutputHitsSync[i].end(); ++iterSyncSignals)
        {
            char v_buf[29];
            (*iterSyncSignals).Serialize(v_buf);
            v_outFile.write(v_buf, 29);
        }
    }
    // ------------------------------------------------------------------------

    v_outFile.close();

    cout << "Done exporting matched edges." << endl;

    return 0; // ok
}

/* Just a wrapper */
UInt_t cls_EdgeMatcher::ExportMatchedEdges(QString p_qfilename)
{
    char v_cfilename[255];
    strcpy(v_cfilename, p_qfilename.toStdString().c_str());
    TString v_filename(v_cfilename);
    return this->ExportMatchedEdges(v_filename);
}

UInt_t cls_EdgeMatcher::ImportMatchedEdges(TString p_filename)
{
    std::ifstream v_inFile (p_filename.Data(), std::ios::in | std::ios::binary);

    if (!v_inFile.is_open()) {
        cerr << "Error opening file " << p_filename.Data() << endl;
        return 1; // FAIL
    }

    cout << "Importing matched edges..." << endl;

    cls_DataHandler* v_dataHandler = cls_DataHandler::Instance();
    std::multimap<Double_t, cls_Hit>& v_mapOfHits = v_dataHandler->fMapOfHits;

    // Clean existing entries
    for (UInt_t i=0; i<4; i++) {
        for (UInt_t j=0; j<33; j++) {
            fOutputHitsBeamDetectors[i][j].clear();
        }
    }
    v_mapOfHits.clear();

    // Read file header
    // First comes the header of the file: 4*33+1 integers
    char bufHeader[(4*33+1)*4]; // multiply by 4 as UInt_t is 4 bytes
    v_inFile.read(bufHeader, (4*33+1)*4);
    UInt_t v_size1[4][33];
    UInt_t v_size2;

    UInt_t v_cursor = 0;
    for (UInt_t i=0; i<4; i++) {
        for (UInt_t j=0; j<33; j++)
        {
            v_size1[i][j] = ((bufHeader[v_cursor+3] << 3*8) & 0xff000000) |
                            ((bufHeader[v_cursor+2] << 2*8) & 0x00ff0000) |
                            ((bufHeader[v_cursor+1] << 1*8) & 0x0000ff00) |
                            ((bufHeader[v_cursor+0] << 0*8) & 0x000000ff);
            v_cursor+=4;
        }
    }
    v_size2 = ((bufHeader[v_cursor+3] << 3*8) & 0xff000000) |
              ((bufHeader[v_cursor+2] << 2*8) & 0x00ff0000) |
              ((bufHeader[v_cursor+1] << 1*8) & 0x0000ff00) |
              ((bufHeader[v_cursor+0] << 0*8) & 0x000000ff);
    v_cursor+=4;
    // ------------------------------------------------------------------------

    // DATA - beam detectors

    char bufBeamDetectors[29];
    for (UInt_t i=0; i<4; i++) {
        for (UInt_t j=0; j<33; j++) {
            for (UInt_t k=0; k<v_size1[i][j]; k++) {
                v_inFile.read(bufBeamDetectors, 29);
                cls_Hit v_readHit(bufBeamDetectors);
                fOutputHitsBeamDetectors[i][j].push_back(v_readHit);
            }
        }
    }
    // ------------------------------------------------------------------------

    // DATA - hits

    for (UInt_t k=0; k<v_size2; k++)
    {
        char bufBeamDetectors[29];
        v_inFile.read(bufBeamDetectors, 29);
        cls_Hit v_readHit(bufBeamDetectors);
        v_mapOfHits.insert(std::pair<Double_t, cls_Hit>(v_readHit.GetMainTime(), v_readHit));
    }
    // ------------------------------------------------------------------------

    // New section - sync messages
    char buffer2[NUMTDCs*4];
    UInt_t v_size3[NUMTDCs];

    // Header
    v_inFile.read(buffer2, NUMTDCs*4);
    v_cursor = 0;
    for (UInt_t i=0; i<NUMTDCs; i++)
    {
        v_size3[i] = ((buffer2[v_cursor+3] << 3*8) & 0xff000000) |
                        ((buffer2[v_cursor+2] << 2*8) & 0x00ff0000) |
                        ((buffer2[v_cursor+1] << 1*8) & 0x0000ff00) |
                        ((buffer2[v_cursor+0] << 0*8) & 0x000000ff);
        v_cursor+=4;
    }

    // Data
    for (UInt_t i=0; i<NUMTDCs; i++) {
        for (UInt_t k=0; k<v_size3[i]; k++) {
            v_inFile.read(bufBeamDetectors, 29);    // reuse the buffer
            cls_Hit v_readHit(bufBeamDetectors);
            fOutputHitsSync[i].push_back(v_readHit);
        }
    }

    // ------------------------------------------------------------------------

    v_inFile.close();

    cout << "Done importing matched edges." << endl;

    return 0; // ok
}

/* Just a wrapper */
UInt_t cls_EdgeMatcher::ImportMatchedEdges(QString p_qfilename)
{
    char v_cfilename[255];
    strcpy(v_cfilename, p_qfilename.toStdString().c_str());
    TString v_filename(v_cfilename);
    return this->ImportMatchedEdges(v_filename);
}

void cls_EdgeMatcher::BuildEvents(void)
{
    cout << "Bulding events..." << endl;

    cls_DataHandler* v_dataHandler = cls_DataHandler::Instance();
    std::multimap<Double_t, cls_Hit>& v_mapOfHits = v_dataHandler->fMapOfHits;

    // Debug output
    /*
    std::ofstream v_outputTextFile;
    v_outputTextFile.open("out.txt");

    if (v_outputTextFile.is_open()) {
        std::multimap<Double_t, cls_Hit>::iterator iter;
        for (iter=v_mapOfHits.begin(); iter!=v_mapOfHits.end(); ++iter) {
            char line[64];
            sprintf(line, "%f: 0x%04x - %d - %d\n", (*iter).first, (*iter).second.GetTDC(), (*iter).second.GetLch(), (*iter).second.GetTch());
            v_outputTextFile << line;
        }
        v_outputTextFile.close();
    }

    std::vector<cls_Hit>::iterator iter;

    std::ofstream v_outputTextFileCh15;
    v_outputTextFileCh15.open("beamDetectors_ch15.txt");
    if (v_outputTextFileCh15.is_open()) {
        cout << "TDC 0x0110 ch 15 size=" << fOutputHitsBeamDetectors[0][15].size() << endl;
        for (iter=fOutputHitsBeamDetectors[0][15].begin(); iter!=fOutputHitsBeamDetectors[0][15].end(); ++iter) {
            char line[64];
            sprintf(line, "%f\n", (*iter).GetLtime());
            v_outputTextFileCh15 << line;
        }
        v_outputTextFileCh15.close();
    }

    std::ofstream v_outputTextFileCh16;
    v_outputTextFileCh16.open("beamDetectors_ch16.txt");
    if (v_outputTextFileCh16.is_open()) {
        cout << "TDC 0x0110 ch 16 size=" << fOutputHitsBeamDetectors[0][16].size() << endl;
        for (iter=fOutputHitsBeamDetectors[0][16].begin(); iter!=fOutputHitsBeamDetectors[0][16].end(); ++iter) {
            char line[64];
            sprintf(line, "%f\n", (*iter).GetTtime());
            v_outputTextFileCh16 << line;
        }
        v_outputTextFileCh16.close();
    }
    */

    // Here we select the appropriate vector of trigger signals and event time window
    std::vector<cls_Hit>* v_triggersUsed;
    Double_t v_eventLeftTimeWin;
    Double_t v_eventRightTimeWin;

    // Lab data
    if (fTriggerType == kTDC0010sync || fTriggerType == kTDC0011sync ||
        fTriggerType == kTDC0012sync || fTriggerType == kTDC0013sync) {

        v_triggersUsed = &(fOutputHitsSync[fTriggerType-1010]);
        v_eventLeftTimeWin = EVENTNEGATIVEWINLAB;
        v_eventRightTimeWin = EVENTPOSITIVEWINLAB;
    }

    // Beamtime data
    else {
        UInt_t trigTdc = ((UInt_t)fTriggerType) / 33;
        UInt_t trigChannel = (UInt_t)fTriggerType - trigTdc*33;
        v_triggersUsed = &(fOutputHitsBeamDetectors[trigTdc][trigChannel]);
        switch (fTriggerType) {
        case kLaser:
            v_eventLeftTimeWin = EVENTNEGATIVEWINLASER;
            v_eventRightTimeWin = EVENTPOSITIVEWINLASER;
            break;
        case kHodoCoinc:
            v_eventLeftTimeWin = EVENTNEGATIVEWINHODO;
            v_eventRightTimeWin = EVENTPOSITIVEWINHODO;
            break;
        default:
            v_eventLeftTimeWin = EVENTNEGATIVEWINLASER;
            v_eventRightTimeWin = EVENTPOSITIVEWINLASER;
            break;
        }
    }

    cout << "First run" << endl;

    // At the beginning - start from the beginning of the hits map
    std::multimap<Double_t, cls_Hit>::iterator lastIterCameraHits = v_mapOfHits.begin();

    // Loop over detected beam signals
    // First run - just to fill the histogram
    std::vector<cls_Hit>::iterator iterBeamSignals;
    for (iterBeamSignals=v_triggersUsed->begin(); iterBeamSignals!=v_triggersUsed->end(); ++iterBeamSignals) {
        Double_t trigTime = (*iterBeamSignals).GetMainTime();

        //cout << "Trigger " << trigTime << endl;

        // Loop over hits from the camera
        std::multimap<Double_t, cls_Hit>::iterator iterCameraHits;

        for (iterCameraHits=lastIterCameraHits; iterCameraHits!=v_mapOfHits.begin(); --iterCameraHits) {
            Double_t curTimestamp = (*iterCameraHits).first;

            // As soon as we get one step left from the left window boundary - stop searching
            if (curTimestamp < trigTime-EVENTWINHISTOL) {
                break;
            }
        }
        // after this loop the iterCameraHits iterator is before the trigget window


        for (/*no action here*/; iterCameraHits!=v_mapOfHits.end(); ++iterCameraHits) {
            Double_t curTimestamp = (*iterCameraHits).first;

            if (curTimestamp < trigTime-EVENTWINHISTOL) {   // before the window - skip
                continue;
            }
            if (curTimestamp > trigTime+EVENTWINHISTOR) {   // after the window - skip
                // store the first unused hit in the map after the right window bound
                lastIterCameraHits = iterCameraHits;
                break;
            }
            fhTriggerCorrelation->Fill(curTimestamp - trigTime);    // in the window - account
        }
    }

    cout << "Second run" << endl;

    // Loop over detected beam signals
    // Second run - per-se build the events (erase the input hits)
    for (iterBeamSignals=v_triggersUsed->begin(); iterBeamSignals!=v_triggersUsed->end(); ++iterBeamSignals) {
        Double_t trigTime = (*iterBeamSignals).GetMainTime();

        Double_t leftBound = trigTime - v_eventLeftTimeWin;
        Double_t rightBound = trigTime + v_eventRightTimeWin;

        cls_Event v_curEvent;
        UInt_t counterRemovedBeforeWin = 0;

        // Loop over hits from the camera
        // Find those in the event time window, push them into the event
        // object and erase from the initial vector
        std::multimap<Double_t, cls_Hit>::iterator iterCameraHits;
        for (iterCameraHits=v_mapOfHits.begin(); iterCameraHits!=v_mapOfHits.end(); /*++iterCameraHits*/) {
            Double_t curTimestamp = (*iterCameraHits).first;

            // Apply cut on correlation with the trigger
            if (curTimestamp > rightBound) {
                break;
            }
            if (curTimestamp < leftBound) {
                iterCameraHits = v_mapOfHits.erase(iterCameraHits);      // erase from the vector of hits
                counterRemovedBeforeWin++;
            } else {
                if (curTimestamp >= leftBound && curTimestamp <= rightBound) {
                    // Hit is in the event window

                    fhTriggerCorrelationInCut->Fill(curTimestamp - trigTime);

                    v_curEvent.AddHit((*iterCameraHits).second);            // add it to the current event and
                    iterCameraHits = v_mapOfHits.erase(iterCameraHits);      // erase from the vector of hits
                } else {
                    ++iterCameraHits;       // otherwise simply go to the next hit
                }
            }

        }

        //cout << "Removed " << counterRemovedBeforeWin << " hits before the window." << endl;

        if (v_curEvent.GetSize() > 0) {
            //printf ("Built event around trigger message %f size=%d\n", trigTime, v_curEvent.GetSize());
            fhNumOfHitsInEvent->Fill(v_curEvent.GetSize());

            cls_DataHandler* v_dataHandler = cls_DataHandler::Instance();
            v_dataHandler->mEvents.push_back(v_curEvent);
        }
    }

    cout << "Done building events." << endl;
}

UInt_t cls_EdgeMatcher::ExportEventBuildingHistos(TString p_filename)
{
    TDirectory* prevDir = gDirectory;
    TFile* prevFile = gFile;

    TFile v_outputFile(p_filename, "RECREATE");

    if (v_outputFile.IsZombie()) {
        cerr << "Error opening file " << p_filename.Data() << endl;
        gDirectory = prevDir;
        gFile = prevFile;
        return 1; // FAIL
    }

    fhTriggerCorrelation->Write();
    fhTriggerCorrelationInCut->Write();
    fhNumOfHitsInEvent->Write();

    v_outputFile.Close();

    cout << "Successfully exported event building histograms into " << p_filename.Data() << "." << endl;

    gDirectory = prevDir;
    gFile = prevFile;
    return 0; // OK
}

/* Just a wrapper */
UInt_t cls_EdgeMatcher::ExportEventBuildingHistos(QString p_qfilename)
{
    char v_cfilename[255];
    strcpy(v_cfilename, p_qfilename.toStdString().c_str());
    TString v_filename(v_cfilename);
    return this->ExportEventBuildingHistos(v_filename);
}
