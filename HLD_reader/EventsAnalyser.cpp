#include "EventsAnalyser.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <TH1D.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TString.h>

#include "global_defines.h"
#include "DataHandler.h"

#include <fstream>

cls_EventsAnalyser::cls_EventsAnalyser()
{
    TString v_histoName;
    TString v_histoTitle;

    // ----------------------------------------------------------------------------------------------------------------------------------
    // Without filtering bad channels

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        v_histoName.Form("fhDiffFirstInTimeOnePMT_%d", v_pmt);
        v_histoTitle.Form("fhDiffFirstInTimeOnePMT_%d;ns;Entries", v_pmt);
        fhDiffFirstInTimeOnePMT[v_pmt] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 4000, 0., 200.);
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        v_histoName.Form("fhDiffFirstInTimeOnePADIWA_%d", v_padiwa);
        v_histoTitle.Form("fhDiffFirstInTimeOnePADIWA_%d;ns;Entries", v_padiwa);
        fhDiffFirstInTimeOnePADIWA[v_padiwa] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 4000, 0., 200.);
    }
    fhDiffFirstInTimeTopRightQuarter = new TH1D("fhDiffFirstInTimeTopRightQuarter", "fhDiffFirstInTimeTopRightQuarter;ns;Entries", 4000, 0., 200.);

    fhLeadingEdgeDiffMean = new TH1D("fhLeadingEdgeDiffMean", "fhLeadingEdgeDiffMean;ns;Entries", 1200, -30., 30.);

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        v_histoName.Form("fhLeadingEdgeDiffOnePMT_%d", v_pmt);
        v_histoTitle.Form("fhLeadingEdgeDiffOnePMT_%d;ns;Entries", v_pmt);
        fhLeadingEdgeDiffOnePMT[v_pmt] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 2400, -60., 60.);
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        v_histoName.Form("fhLeadingEdgeDiffOnePADIWA_%d", v_padiwa);
        v_histoTitle.Form("fhLeadingEdgeDiffOnePADIWA_%d;ns;Entries", v_padiwa);
        fhLeadingEdgeDiffOnePADIWA[v_padiwa] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 2400, -60., 60.);
    }

    fhLeadingEdgeDiffTopRightQuarter = new TH1D("fhLeadingEdgeDiffTopRightQuarter", "fhLeadingEdgeDiffTopRightQuarter;ns;Entries", 2400, -60., 60.);

    // ----------------------------------------------------------------------------------------------------------------------------------
    // With filtering bad channels

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        v_histoName.Form("fhDiffFirstInTimeOnePMTGood_%d", v_pmt);
        v_histoTitle.Form("fhDiffFirstInTimeOnePMTGood_%d;ns;Entries", v_pmt);
        fhDiffFirstInTimeOnePMTGood[v_pmt] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 4000, 0., 200.);
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        v_histoName.Form("fhDiffFirstInTimeOnePADIWAGood_%d", v_padiwa);
        v_histoTitle.Form("fhDiffFirstInTimeOnePADIWAGood_%d;ns;Entries", v_padiwa);
        fhDiffFirstInTimeOnePADIWAGood[v_padiwa] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 4000, 0., 200.);
    }
    fhDiffFirstInTimeTopRightQuarterGood = new TH1D("fhDiffFirstInTimeTopRightQuarterGood", "fhDiffFirstInTimeTopRightQuarterGood;ns;Entries", 4000, 0., 200.);

    fhLeadingEdgeDiffMeanGood = new TH1D("fhLeadingEdgeDiffMeanGood", "fhLeadingEdgeDiffMeanGood;ns;Entries", 1200, -30., 30.);

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        v_histoName.Form("fhLeadingEdgeDiffOnePMTGood_%d", v_pmt);
        v_histoTitle.Form("fhLeadingEdgeDiffOnePMTGood_%d;ns;Entries", v_pmt);
        fhLeadingEdgeDiffOnePMTGood[v_pmt] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 2400, -60., 60.);
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        v_histoName.Form("fhLeadingEdgeDiffOnePADIWAGood_%d", v_padiwa);
        v_histoTitle.Form("fhLeadingEdgeDiffOnePADIWAGood_%d;ns;Entries", v_padiwa);
        fhLeadingEdgeDiffOnePADIWAGood[v_padiwa] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 2400, -60., 60.);
    }

    fhLeadingEdgeDiffTopRightQuarterGood = new TH1D("fhLeadingEdgeDiffTopRightQuarterGood", "fhLeadingEdgeDiffTopRightQuarterGood;ns;Entries", 2400, -60., 60.);

    // ----------------------------------------------------------------------------------------------------------------------------------
    // fhLeadingEdgeDiff for each pair of pixels is here after all the other histograms

    for (UInt_t v_pixel1=0; v_pixel1<256-1; v_pixel1++) {
        for (UInt_t v_pixel2=v_pixel1+1; v_pixel2<256; v_pixel2++) {
            v_histoName.Form("fhLeadingEdgeDiff_ID1_%02d_ID2_%02d", v_pixel1, v_pixel2);
            v_histoTitle.Form("fhLeadingEdgeDiff_ID1_%02d_ID2_%02d;ns;Entries", v_pixel1, v_pixel2);
            fhLeadingEdgeDiff[v_pixel1][v_pixel2] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 1200, -30., 30.);
        }
    }
}

cls_EventsAnalyser::~cls_EventsAnalyser()
{
    // ----------------------------------------------------------------------------------------------------------------------------------
    // Without filtering bad channels

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        delete fhDiffFirstInTimeOnePMT[v_pmt];
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        delete fhDiffFirstInTimeOnePADIWA[v_padiwa];
    }
    delete fhDiffFirstInTimeTopRightQuarter;

    delete fhLeadingEdgeDiffMean;

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        delete fhLeadingEdgeDiffOnePMT[v_pmt];
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        delete fhLeadingEdgeDiffOnePADIWA[v_padiwa];
    }

    delete fhLeadingEdgeDiffTopRightQuarter;

    // ----------------------------------------------------------------------------------------------------------------------------------
    // With filtering bad channels

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        delete fhDiffFirstInTimeOnePMTGood[v_pmt];
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        delete fhDiffFirstInTimeOnePADIWAGood[v_padiwa];
    }
    delete fhDiffFirstInTimeTopRightQuarterGood;

    delete fhLeadingEdgeDiffMeanGood;

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        delete fhLeadingEdgeDiffOnePMTGood[v_pmt];
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        delete fhLeadingEdgeDiffOnePADIWAGood[v_padiwa];
    }

    delete fhLeadingEdgeDiffTopRightQuarterGood;

    // ----------------------------------------------------------------------------------------------------------------------------------
    // fhLeadingEdgeDiff for each pair of pixels is here after all the other histograms

    for (UInt_t v_pixel1=0; v_pixel1<256-1; v_pixel1++) {
        for (UInt_t v_pixel2=v_pixel1+1; v_pixel2<256; v_pixel2++) {
            delete fhLeadingEdgeDiff[v_pixel1][v_pixel2];
        }
    }
}

void cls_EventsAnalyser::RunAnalysis(void)
{
    cout << "Running analysis..." << endl;

    cls_DataHandler* v_dataHandler = cls_DataHandler::Instance();
    std::vector<cls_Event>& v_Events = v_dataHandler->mEvents;
    std::vector<cls_Event>::iterator v_iterEvents;

    UInt_t tmpCounter = 0;

    // Loop over all events
    for (v_iterEvents=v_Events.begin(); v_iterEvents!=v_Events.end(); ++v_iterEvents) {

        //cout << "Event " << ++tmpCounter << ". " << (*v_iterEvents).mHits.size() << " hits." << endl;

        ProcessPerTDC(v_iterEvents, 0x0010, 0);
        ProcessPerTDC(v_iterEvents, 0x0011, 1);
        ProcessPerTDC(v_iterEvents, 0x0012, 2);
        ProcessPerTDC(v_iterEvents, 0x0013, 3);
        ProcessPerTDC(v_iterEvents, 0x0020, 4);
        ProcessPerTDC(v_iterEvents, 0x0021, 5);
        ProcessPerTDC(v_iterEvents, 0x0022, 6);
        ProcessPerTDC(v_iterEvents, 0x0023, 7);
        ProcessPerTDC(v_iterEvents, 0x0050, 8);
        ProcessPerTDC(v_iterEvents, 0x0051, 9);
        ProcessPerTDC(v_iterEvents, 0x0052, 10);
        ProcessPerTDC(v_iterEvents, 0x0053, 11);
        ProcessPerTDC(v_iterEvents, 0x0060, 12);
        ProcessPerTDC(v_iterEvents, 0x0061, 13);
        ProcessPerTDC(v_iterEvents, 0x0062, 14);
        ProcessPerTDC(v_iterEvents, 0x0063, 15);

        ProcessPerPMT(v_iterEvents, 1, 0);
        ProcessPerPMT(v_iterEvents, 2, 1);
        ProcessPerPMT(v_iterEvents, 5, 2);
        ProcessPerPMT(v_iterEvents, 6, 3);

        ProcessTRQ(v_iterEvents);

        ProcessPerTDCGood(v_iterEvents, 0x0010, 0);
        ProcessPerTDCGood(v_iterEvents, 0x0011, 1);
        ProcessPerTDCGood(v_iterEvents, 0x0012, 2);
        ProcessPerTDCGood(v_iterEvents, 0x0013, 3);
        ProcessPerTDCGood(v_iterEvents, 0x0020, 4);
        ProcessPerTDCGood(v_iterEvents, 0x0021, 5);
        ProcessPerTDCGood(v_iterEvents, 0x0022, 6);
        ProcessPerTDCGood(v_iterEvents, 0x0023, 7);
        ProcessPerTDCGood(v_iterEvents, 0x0050, 8);
        ProcessPerTDCGood(v_iterEvents, 0x0051, 9);
        ProcessPerTDCGood(v_iterEvents, 0x0052, 10);
        ProcessPerTDCGood(v_iterEvents, 0x0053, 11);
        ProcessPerTDCGood(v_iterEvents, 0x0060, 12);
        ProcessPerTDCGood(v_iterEvents, 0x0061, 13);
        ProcessPerTDCGood(v_iterEvents, 0x0062, 14);
        ProcessPerTDCGood(v_iterEvents, 0x0063, 15);

        ProcessPerPMTGood(v_iterEvents, 1, 0);
        ProcessPerPMTGood(v_iterEvents, 2, 1);
        ProcessPerPMTGood(v_iterEvents, 5, 2);
        ProcessPerPMTGood(v_iterEvents, 6, 3);

        ProcessTRQGood(v_iterEvents);

        ProcessLeadingEdgeDiff(v_iterEvents);
    }

    ProcessLeadingEdgeDiffMean();

    cout << "Finished analysis." << endl;
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
// One event as input
void cls_EventsAnalyser::ProcessPerTDC(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_TdcId, UInt_t p_histoIndex)
{
    Bool_t v_refFound = kFALSE;
    Double_t v_refTime = 0.;
    std::vector<cls_Hit>::iterator v_iterRef;

    // Loop over the hits of the event - find the reference
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {
        if ((*v_iterHits).GetTDC() == TDCidToInteger(p_TdcId)) {
            v_refFound = kTRUE;
            v_refTime = (*v_iterHits).GetLtime();
            v_iterRef = v_iterHits;
            break;
        }
    }

    // If no reference found in the current event - return
    if (!v_refFound) return;

    // Loop once again - fill the histo
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {

        if (v_iterHits == v_iterRef) continue;

        if ((*v_iterHits).GetTDC() == TDCidToInteger(p_TdcId)) {
            Double_t v_curTime = (*v_iterHits).GetLtime();
            fhDiffFirstInTimeOnePADIWA[p_histoIndex]->Fill(v_curTime - v_refTime);
        }
    }

    // Leading edge difference for this TDC
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {
        if (!((*v_iterHits).GetTDC() == TDCidToInteger(p_TdcId))) continue;

        for (std::vector<cls_Hit>::iterator v_iterHits2 = (*p_iterEvents).mHits.begin(); v_iterHits2 != (*p_iterEvents).mHits.end(); ++v_iterHits2) {
            if (!((*v_iterHits2).GetTDC() == TDCidToInteger(p_TdcId))) continue;

            // Compute pixels unique ID
            UInt_t v_pixel1uid = PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch());  // Check that GetLch() does not return -1
            UInt_t v_pixel2uid = PixelUIDtopRightQuarterOnly((*v_iterHits2).GetTDC(), (*v_iterHits2).GetLch());  // Check that GetLch() does not return -1

            // Skip
            if (v_pixel1uid == v_pixel2uid) continue;

            Double_t v_diff = (*v_iterHits2).GetLtime() - (*v_iterHits).GetLtime();

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiffOnePADIWA[p_histoIndex]->Fill(v_diff);
            else
                fhLeadingEdgeDiffOnePADIWA[p_histoIndex]->Fill(-v_diff);
        }
    }
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
// p_PmtId starts from 1
void cls_EventsAnalyser::ProcessPerPMT(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_PmtId, UInt_t p_histoIndex)
{
    Bool_t v_refFound = kFALSE;
    Double_t v_refTime = 0.;
    std::vector<cls_Hit>::iterator v_iterRef;

    // Loop over the hits of the event - find the reference
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {
        if ( ((*v_iterHits).GetTDC() / 4 ) == (p_PmtId-1) ) {
            v_refFound = kTRUE;
            v_refTime = (*v_iterHits).GetLtime();
            v_iterRef = v_iterHits;
            break;
        }
    }

    // If no reference found in the current event - return
    if (!v_refFound) return;

    // Loop once again - fill the histo
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {

        if (v_iterHits == v_iterRef) continue;

        if ( ((*v_iterHits).GetTDC() / 4 ) == (p_PmtId-1) ) {
            Double_t v_curTime = (*v_iterHits).GetLtime();
            fhDiffFirstInTimeOnePMT[p_histoIndex]->Fill(v_curTime - v_refTime);
        }
    }

    // Leading edge difference for this TDC
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {
        if (!(((*v_iterHits).GetTDC() / 4 ) == (p_PmtId-1))) continue;

        for (std::vector<cls_Hit>::iterator v_iterHits2 = (*p_iterEvents).mHits.begin(); v_iterHits2 != (*p_iterEvents).mHits.end(); ++v_iterHits2) {
            if (!(((*v_iterHits2).GetTDC() / 4 ) == (p_PmtId-1))) continue;

            // Compute pixels unique ID
            UInt_t v_pixel1uid = PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch());  // Check that GetLch() does not return -1
            UInt_t v_pixel2uid = PixelUIDtopRightQuarterOnly((*v_iterHits2).GetTDC(), (*v_iterHits2).GetLch());  // Check that GetLch() does not return -1

            // Skip
            if (v_pixel1uid == v_pixel2uid) continue;

            Double_t v_diff = (*v_iterHits2).GetLtime() - (*v_iterHits).GetLtime();

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiffOnePMT[p_histoIndex]->Fill(v_diff);
            else
                fhLeadingEdgeDiffOnePMT[p_histoIndex]->Fill(-v_diff);
        }
    }
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
void cls_EventsAnalyser::ProcessTRQ(std::vector<cls_Event>::iterator p_iterEvents)
{
    Bool_t v_refFound = kFALSE;
    Double_t v_refTime = 0.;
    std::vector<cls_Hit>::iterator v_iterRef;

    // Loop over the hits of the event - find the reference
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {
        if ( IsTopRightQuarter((*v_iterHits).GetTDC()) ) {
            v_refFound = kTRUE;
            v_refTime = (*v_iterHits).GetLtime();
            v_iterRef = v_iterHits;
            break;
        }
    }

    // If no reference found in the current event - return
    if (!v_refFound) return;

    // Loop once again - fill the histo
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {

        if (v_iterHits == v_iterRef) continue;

        if ( IsTopRightQuarter((*v_iterHits).GetTDC()) ) {
            Double_t v_curTime = (*v_iterHits).GetLtime();
            fhDiffFirstInTimeTopRightQuarter->Fill(v_curTime - v_refTime);
        }
    }

    // Leading edge difference for this TDC
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {
        if (!(IsTopRightQuarter((*v_iterHits).GetTDC()))) continue;

        for (std::vector<cls_Hit>::iterator v_iterHits2 = (*p_iterEvents).mHits.begin(); v_iterHits2 != (*p_iterEvents).mHits.end(); ++v_iterHits2) {
            if (!(IsTopRightQuarter((*v_iterHits2).GetTDC()))) continue;

            // Compute pixels unique ID
            UInt_t v_pixel1uid = PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch());  // Check that GetLch() does not return -1
            UInt_t v_pixel2uid = PixelUIDtopRightQuarterOnly((*v_iterHits2).GetTDC(), (*v_iterHits2).GetLch());  // Check that GetLch() does not return -1

            // Skip
            if (v_pixel1uid == v_pixel2uid) continue;

            Double_t v_diff = (*v_iterHits2).GetLtime() - (*v_iterHits).GetLtime();

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiffTopRightQuarter->Fill(v_diff);
            else
                fhLeadingEdgeDiffTopRightQuarter->Fill(-v_diff);
        }
    }
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
// One event as input
void cls_EventsAnalyser::ProcessPerTDCGood(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_TdcId, UInt_t p_histoIndex)
{
    Bool_t v_refFound = kFALSE;
    Double_t v_refTime = 0.;
    std::vector<cls_Hit>::iterator v_iterRef;

    // Loop over the hits of the event - find the reference
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {
        if ((*v_iterHits).GetTDC() == TDCidToInteger(p_TdcId)) {
            if ( IsGoodPixel(PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch())) ) {
                v_refFound = kTRUE;
                v_refTime = (*v_iterHits).GetLtime();
                v_iterRef = v_iterHits;
                break;
            }
        }
    }

    // If no reference found in the current event - return
    if (!v_refFound) return;

    // Loop once again - fill the histo
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {

        if (v_iterHits == v_iterRef) continue;

        if ((*v_iterHits).GetTDC() == TDCidToInteger(p_TdcId)) {
            if ( IsGoodPixel(PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch())) )
            {
                Double_t v_curTime = (*v_iterHits).GetLtime();
                fhDiffFirstInTimeOnePADIWAGood[p_histoIndex]->Fill(v_curTime - v_refTime);
            }
        }
    }

    // Leading edge difference for this TDC
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {
        if (!((*v_iterHits).GetTDC() == TDCidToInteger(p_TdcId))) continue;
        if ( !IsGoodPixel(PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch())) ) continue;

        for (std::vector<cls_Hit>::iterator v_iterHits2 = (*p_iterEvents).mHits.begin(); v_iterHits2 != (*p_iterEvents).mHits.end(); ++v_iterHits2) {
            if (!((*v_iterHits2).GetTDC() == TDCidToInteger(p_TdcId))) continue;
            if ( !IsGoodPixel(PixelUIDtopRightQuarterOnly((*v_iterHits2).GetTDC(), (*v_iterHits2).GetLch())) ) continue;

            // Compute pixels unique ID
            UInt_t v_pixel1uid = PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch());  // Check that GetLch() does not return -1
            UInt_t v_pixel2uid = PixelUIDtopRightQuarterOnly((*v_iterHits2).GetTDC(), (*v_iterHits2).GetLch());  // Check that GetLch() does not return -1

            // Skip
            if (v_pixel1uid == v_pixel2uid) continue;

            Double_t v_diff = (*v_iterHits2).GetLtime() - (*v_iterHits).GetLtime();

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiffOnePADIWAGood[p_histoIndex]->Fill(v_diff);
            else
                fhLeadingEdgeDiffOnePADIWAGood[p_histoIndex]->Fill(-v_diff);
        }
    }
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
// p_PmtId starts from 1
void cls_EventsAnalyser::ProcessPerPMTGood(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_PmtId, UInt_t p_histoIndex)
{
    Bool_t v_refFound = kFALSE;
    Double_t v_refTime = 0.;
    std::vector<cls_Hit>::iterator v_iterRef;

    // Loop over the hits of the event - find the reference
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {
        if ( ((*v_iterHits).GetTDC() / 4 ) == (p_PmtId-1) ) {
            if ( IsGoodPixel(PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch())) )  {
                v_refFound = kTRUE;
                v_refTime = (*v_iterHits).GetLtime();
                v_iterRef = v_iterHits;
                break;
            }
        }
    }

    // If no reference found in the current event - return
    if (!v_refFound) return;

    // Loop once again - fill the histo
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {

        if (v_iterHits == v_iterRef) continue;

        if ( ((*v_iterHits).GetTDC() / 4 ) == (p_PmtId-1) ) {
            if ( IsGoodPixel(PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch())) )  {
                Double_t v_curTime = (*v_iterHits).GetLtime();
                fhDiffFirstInTimeOnePMTGood[p_histoIndex]->Fill(v_curTime - v_refTime);
            }
        }
    }

    // Leading edge difference for this TDC
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {
        if (!(((*v_iterHits).GetTDC() / 4 ) == (p_PmtId-1))) continue;
        if ( !IsGoodPixel(PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch())) )  continue;

        for (std::vector<cls_Hit>::iterator v_iterHits2 = (*p_iterEvents).mHits.begin(); v_iterHits2 != (*p_iterEvents).mHits.end(); ++v_iterHits2) {
            if (!(((*v_iterHits2).GetTDC() / 4 ) == (p_PmtId-1))) continue;
            if ( !IsGoodPixel(PixelUIDtopRightQuarterOnly((*v_iterHits2).GetTDC(), (*v_iterHits2).GetLch())) )  continue;

            // Compute pixels unique ID
            UInt_t v_pixel1uid = PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch());  // Check that GetLch() does not return -1
            UInt_t v_pixel2uid = PixelUIDtopRightQuarterOnly((*v_iterHits2).GetTDC(), (*v_iterHits2).GetLch());  // Check that GetLch() does not return -1

            // Skip
            if (v_pixel1uid == v_pixel2uid) continue;

            Double_t v_diff = (*v_iterHits2).GetLtime() - (*v_iterHits).GetLtime();

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiffOnePMTGood[p_histoIndex]->Fill(v_diff);
            else
                fhLeadingEdgeDiffOnePMTGood[p_histoIndex]->Fill(-v_diff);
        }
    }
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
void cls_EventsAnalyser::ProcessTRQGood(std::vector<cls_Event>::iterator p_iterEvents)
{
    Bool_t v_refFound = kFALSE;
    Double_t v_refTime = 0.;
    std::vector<cls_Hit>::iterator v_iterRef;

    // Loop over the hits of the event - find the reference
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {
        if ( IsTopRightQuarter((*v_iterHits).GetTDC()) ) {
            if ( IsGoodPixel(PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch())) )  {
                v_refFound = kTRUE;
                v_refTime = (*v_iterHits).GetLtime();
                v_iterRef = v_iterHits;
                break;
            }
        }
    }

    // If no reference found in the current event - return
    if (!v_refFound) return;

    // Loop once again - fill the histo
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {

        if (v_iterHits == v_iterRef) continue;

        if ( IsTopRightQuarter((*v_iterHits).GetTDC()) ) {
            if ( IsGoodPixel(PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch())) )  {
                Double_t v_curTime = (*v_iterHits).GetLtime();
                fhDiffFirstInTimeTopRightQuarterGood->Fill(v_curTime - v_refTime);
            }
        }
    }

    // Leading edge difference for this TDC
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits) {
        if (!(IsTopRightQuarter((*v_iterHits).GetTDC()))) continue;
        if ( !IsGoodPixel(PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch())) )  continue;

        for (std::vector<cls_Hit>::iterator v_iterHits2 = (*p_iterEvents).mHits.begin(); v_iterHits2 != (*p_iterEvents).mHits.end(); ++v_iterHits2) {
            if (!(IsTopRightQuarter((*v_iterHits2).GetTDC()))) continue;
            if ( !IsGoodPixel(PixelUIDtopRightQuarterOnly((*v_iterHits2).GetTDC(), (*v_iterHits2).GetLch())) )  continue;

            // Compute pixels unique ID
            UInt_t v_pixel1uid = PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch());  // Check that GetLch() does not return -1
            UInt_t v_pixel2uid = PixelUIDtopRightQuarterOnly((*v_iterHits2).GetTDC(), (*v_iterHits2).GetLch());  // Check that GetLch() does not return -1

            // Skip
            if (v_pixel1uid == v_pixel2uid) continue;

            Double_t v_diff = (*v_iterHits2).GetLtime() - (*v_iterHits).GetLtime();

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiffTopRightQuarterGood->Fill(v_diff);
            else
                fhLeadingEdgeDiffTopRightQuarterGood->Fill(-v_diff);
        }
    }
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
void cls_EventsAnalyser::ProcessLeadingEdgeDiff(std::vector<cls_Event>::iterator p_iterEvents)
{
    for (std::vector<cls_Hit>::iterator v_iterHits = (*p_iterEvents).mHits.begin(); v_iterHits != (*p_iterEvents).mHits.end(); ++v_iterHits)
    {
        // Consider only hits in the top-tight quarter of the camera
        if (!IsTopRightQuarter((*v_iterHits).GetTDC())) continue;

        for (std::vector<cls_Hit>::iterator v_iterHits2 = (*p_iterEvents).mHits.begin(); v_iterHits2 != (*p_iterEvents).mHits.end(); ++v_iterHits2)
        {
            // Consider only hits in the top-tight quarter of the camera
            if (!IsTopRightQuarter((*v_iterHits2).GetTDC())) continue;

            // Compute pixels unique ID
            UInt_t v_pixel1uid = PixelUIDtopRightQuarterOnly((*v_iterHits).GetTDC(), (*v_iterHits).GetLch());  // Check that GetLch() does not return -1
            UInt_t v_pixel2uid = PixelUIDtopRightQuarterOnly((*v_iterHits2).GetTDC(), (*v_iterHits2).GetLch());  // Check that GetLch() does not return -1

            // Skip
            if (v_pixel1uid == v_pixel2uid) continue;

            Double_t v_diff = (*v_iterHits2).GetLtime() - (*v_iterHits).GetLtime();

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiff[v_pixel1uid][v_pixel2uid]->Fill(v_diff);
            else
                fhLeadingEdgeDiff[v_pixel2uid][v_pixel1uid]->Fill(-v_diff);
        }
    }
}

void cls_EventsAnalyser::ProcessLeadingEdgeDiffMean(void)
{
    // One histogram for the mean values of all LeadingEdgeDiff
    // Top-right quarter
    for (UInt_t v_pixel1=0; v_pixel1<256-1; v_pixel1++) {
        for (UInt_t v_pixel2=v_pixel1+1; v_pixel2<256; v_pixel2++) {
            if (fhLeadingEdgeDiff[v_pixel1][v_pixel2]->GetEntries() > 0) {
                fhLeadingEdgeDiffMean->Fill(fhLeadingEdgeDiff[v_pixel1][v_pixel2]->GetMean());
                // Skip bad pixels listed in a special method
                if (IsGoodPixel(v_pixel1) && IsGoodPixel(v_pixel2)) {
                    fhLeadingEdgeDiffMeanGood->Fill(fhLeadingEdgeDiff[v_pixel1][v_pixel2]->GetMean());
                }
            }
        }
    }
}

UInt_t cls_EventsAnalyser::ExportHistos(TString p_filename)
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

    // ----------------------------------------------------------------------------------------------------------------------------------
    // Without filtering bad channels

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        fhDiffFirstInTimeOnePMT[v_pmt]->Write();
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        fhDiffFirstInTimeOnePADIWA[v_padiwa]->Write();
    }
    fhDiffFirstInTimeTopRightQuarter->Write();

    fhLeadingEdgeDiffMean->Write();

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        fhLeadingEdgeDiffOnePMT[v_pmt]->Write();
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        fhLeadingEdgeDiffOnePADIWA[v_padiwa]->Write();
    }

    fhLeadingEdgeDiffTopRightQuarter->Write();

    // ----------------------------------------------------------------------------------------------------------------------------------
    // With filtering bad channels

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        fhDiffFirstInTimeOnePMTGood[v_pmt]->Write();
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        fhDiffFirstInTimeOnePADIWAGood[v_padiwa]->Write();
    }
    fhDiffFirstInTimeTopRightQuarterGood->Write();

    fhLeadingEdgeDiffMeanGood->Write();

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        fhLeadingEdgeDiffOnePMTGood[v_pmt]->Write();
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        fhLeadingEdgeDiffOnePADIWAGood[v_padiwa]->Write();
    }

    fhLeadingEdgeDiffTopRightQuarterGood->Write();

    // ----------------------------------------------------------------------------------------------------------------------------------
    // fhLeadingEdgeDiff for each pair of pixels is here after all the other histograms

    for (UInt_t v_pixel1=0; v_pixel1<256-1; v_pixel1++) {
        TString dirName;
        dirName.Form("LeadingEdgeDiff_ID1_%02d", v_pixel1);
        gDirectory->mkdir(dirName);
        gDirectory->cd(dirName);
        for (UInt_t v_pixel2=v_pixel1+1; v_pixel2<256; v_pixel2++) {
            fhLeadingEdgeDiff[v_pixel1][v_pixel2]->Write();
        }
        gDirectory->cd("..");
    }

    v_outputFile.Close();

    cout << "Successfully exported analysis histograms into " << p_filename.Data() << "." << endl;

    gDirectory = prevDir;
    gFile = prevFile;
    return 0; // OK
}

// Export correction table into a text file
UInt_t cls_EventsAnalyser::ExportCorrections(QString p_qfilename)
{
    std::ofstream v_corrTableFile;
    v_corrTableFile.open(p_qfilename.toStdString());
    if (v_corrTableFile.is_open()) {

        // PMTs 1 and 2
        UInt_t v_pixel1 = 0; // reference is 0
        v_corrTableFile << std::fixed  << 0. << endl;
        for (UInt_t v_pixel2=1; v_pixel2<128; v_pixel2++) {
            Double_t v_meanVal = fhLeadingEdgeDiff[v_pixel1][v_pixel2]->GetMean();
            v_corrTableFile << std::fixed << v_meanVal << endl;
        }
        // PMTs 3 and 4 are skipped
        for (UInt_t i=0; i<128; i++) v_corrTableFile << std::fixed  << 0. << endl;
        // PMTs 5 and 6
        for (UInt_t v_pixel2=128; v_pixel2<256; v_pixel2++) {
            Double_t v_meanVal = fhLeadingEdgeDiff[v_pixel1][v_pixel2]->GetMean();
            v_corrTableFile << std::fixed << v_meanVal << endl;
        }
        // PMTs 7 to 16 are skipped
        for (UInt_t i=0; i<128*5; i++) v_corrTableFile << std::fixed  << 0. << endl;

        v_corrTableFile.close();
        cout << "Successfully exported correction tables into " << p_qfilename.toStdString() << endl;
        return 0; // ok
    } else {
        cout << "Failed to open " << p_qfilename.toStdString() << endl;
        return 1; // fail
    }
}

/* Just a wrapper */
UInt_t cls_EventsAnalyser::ExportHistos(QString p_qfilename)
{
    char v_cfilename[255];
    strcpy(v_cfilename, p_qfilename.toStdString().c_str());
    TString v_filename(v_cfilename);
    return this->ExportHistos(v_filename);
}

/* Just a wrapper */
UInt_t cls_EventsAnalyser::ExportCorrections(TString p_filename)
{
    QString v_filename(p_filename.Data());
    return this->ExportCorrections(v_filename);
}
