#include "RingsAnalyser.h"

#include <fstream>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <TH1D.h>
#include <TH2D.h>
#include <TDirectory.h>
#include <TFile.h>

#include "DataHandler.h"
#include "global_defines.h"
#include "CbmRichProtRingFinderHoughImpl.h"
#include "RingVisu.h"

cls_RingsAnalyser::cls_RingsAnalyser()
{
    this->ImportPixelMap();

    fhNumOfFoundRings = new TH1D("fhNumOfFoundRings", "fhNumOfFoundRings;rings;Entries", 10, 0., 10.);

    fhNumOfHitsInTheRing = new TH1D("fhNumOfHitsInTheRing", "fhNumOfHitsInTheRing;hits;Entries", 60, 0., 60.);
    fhRingsCenter = new TH2D("fhRingsCenter", "fhRingsCenter;cm;cm", 440, 0., 22., 440, 0., 22.);
    fhRingsRadius = new TH1D("fhRingsRadius", "fhRingsRadius;cm;Entries", 400, 0., 20.);
    fhNumOfHitsInTheRingInCut = new TH1D("fhNumOfHitsInTheRingInCut", "fhNumOfHitsInTheRingInCut;hits;Entries", 60, 0., 60.);
    fhRingsCenterInCut = new TH2D("fhRingsCenterInCut", "fhRingsCenterInCut;cm;cm", 440, 0., 22., 440, 0., 22.);
    fhRingsRadiusInCut = new TH1D("fhRingsRadiusInCut", "fhRingsRadiusInCut;cm;Entries", 400, 0., 20.);

    // Copied from the EventsAnalysis class -----------------------------------

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
        v_histoName.Form("fhLeadingEdgeDiffOnePMTSymm_%d", v_pmt);
        v_histoTitle.Form("fhLeadingEdgeDiffOnePMTSymm_%d;ns;Entries", v_pmt);
        fhLeadingEdgeDiffOnePMTSymm[v_pmt] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 2400, -60., 60.);
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        v_histoName.Form("fhLeadingEdgeDiffOnePADIWA_%d", v_padiwa);
        v_histoTitle.Form("fhLeadingEdgeDiffOnePADIWA_%d;ns;Entries", v_padiwa);
        fhLeadingEdgeDiffOnePADIWA[v_padiwa] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 2400, -60., 60.);
        v_histoName.Form("fhLeadingEdgeDiffOnePADIWASymm_%d", v_padiwa);
        v_histoTitle.Form("fhLeadingEdgeDiffOnePADIWASymm_%d;ns;Entries", v_padiwa);
        fhLeadingEdgeDiffOnePADIWASymm[v_padiwa] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 2400, -60., 60.);
    }

    fhLeadingEdgeDiffTopRightQuarter = new TH1D("fhLeadingEdgeDiffTopRightQuarter", "fhLeadingEdgeDiffTopRightQuarter;ns;Entries", 2400, -60., 60.);
    fhLeadingEdgeDiffTopRightQuarterSymm = new TH1D("fhLeadingEdgeDiffTopRightQuarterSymm", "fhLeadingEdgeDiffTopRightQuarterSymm;ns;Entries", 2400, -60., 60.);

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
        v_histoName.Form("fhLeadingEdgeDiffOnePMTGoodSymm_%d", v_pmt);
        v_histoTitle.Form("fhLeadingEdgeDiffOnePMTGoodSymm_%d;ns;Entries", v_pmt);
        fhLeadingEdgeDiffOnePMTGoodSymm[v_pmt] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 2400, -60., 60.);
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        v_histoName.Form("fhLeadingEdgeDiffOnePADIWAGood_%d", v_padiwa);
        v_histoTitle.Form("fhLeadingEdgeDiffOnePADIWAGood_%d;ns;Entries", v_padiwa);
        fhLeadingEdgeDiffOnePADIWAGood[v_padiwa] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 2400, -60., 60.);
        v_histoName.Form("fhLeadingEdgeDiffOnePADIWAGoodSymm_%d", v_padiwa);
        v_histoTitle.Form("fhLeadingEdgeDiffOnePADIWAGoodSymm_%d;ns;Entries", v_padiwa);
        fhLeadingEdgeDiffOnePADIWAGoodSymm[v_padiwa] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 2400, -60., 60.);
    }

    fhLeadingEdgeDiffTopRightQuarterGood = new TH1D("fhLeadingEdgeDiffTopRightQuarterGood", "fhLeadingEdgeDiffTopRightQuarterGood;ns;Entries", 2400, -60., 60.);
    fhLeadingEdgeDiffTopRightQuarterGoodSymm = new TH1D("fhLeadingEdgeDiffTopRightQuarterGoodSymm", "fhLeadingEdgeDiffTopRightQuarterGoodSymm;ns;Entries", 2400, -60., 60.);

    // ----------------------------------------------------------------------------------------------------------------------------------
    // fhLeadingEdgeDiff for each pair of pixels is here after all the other histograms

    for (UInt_t v_pixel1=0; v_pixel1<256-1; v_pixel1++) {
        for (UInt_t v_pixel2=v_pixel1+1; v_pixel2<256; v_pixel2++) {
            v_histoName.Form("fhLeadingEdgeDiff_ID1_%02d_ID2_%02d", v_pixel1, v_pixel2);
            v_histoTitle.Form("fhLeadingEdgeDiff_ID1_%02d_ID2_%02d;ns;Entries", v_pixel1, v_pixel2);
            fhLeadingEdgeDiff[v_pixel1][v_pixel2] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 1200, -30., 30.);
            v_histoName.Form("fhLeadingEdgeDiffSymm_ID1_%02d_ID2_%02d", v_pixel1, v_pixel2);
            v_histoTitle.Form("fhLeadingEdgeDiffSymm_ID1_%02d_ID2_%02d;ns;Entries", v_pixel1, v_pixel2);
            fhLeadingEdgeDiffSymm[v_pixel1][v_pixel2] = new TH1D(v_histoName.Data(), v_histoTitle.Data(), 1200, -30., 30.);
        }
    }

    // ------------------------------------------------------------------------
}

cls_RingsAnalyser::~cls_RingsAnalyser()
{
    delete fhNumOfFoundRings;

    delete fhNumOfHitsInTheRing;
    delete fhRingsCenter;
    delete fhRingsRadius;
    delete fhNumOfHitsInTheRingInCut;
    delete fhRingsCenterInCut;
    delete fhRingsRadiusInCut;

    // Copied from the EventsAnalysis class -----------------------------------

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
        delete fhLeadingEdgeDiffOnePMTSymm[v_pmt];
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        delete fhLeadingEdgeDiffOnePADIWA[v_padiwa];
        delete fhLeadingEdgeDiffOnePADIWASymm[v_padiwa];
    }

    delete fhLeadingEdgeDiffTopRightQuarter;
    delete fhLeadingEdgeDiffTopRightQuarterSymm;

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
        delete fhLeadingEdgeDiffOnePMTGoodSymm[v_pmt];
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        delete fhLeadingEdgeDiffOnePADIWAGood[v_padiwa];
        delete fhLeadingEdgeDiffOnePADIWAGoodSymm[v_padiwa];
    }

    delete fhLeadingEdgeDiffTopRightQuarterGood;
    delete fhLeadingEdgeDiffTopRightQuarterGoodSymm;

    // ----------------------------------------------------------------------------------------------------------------------------------
    // fhLeadingEdgeDiff for each pair of pixels is here after all the other histograms

    for (UInt_t v_pixel1=0; v_pixel1<256-1; v_pixel1++) {
        for (UInt_t v_pixel2=v_pixel1+1; v_pixel2<256; v_pixel2++) {
            delete fhLeadingEdgeDiff[v_pixel1][v_pixel2];
            delete fhLeadingEdgeDiffSymm[v_pixel1][v_pixel2];
        }
    }

    // ------------------------------------------------------------------------
}

UInt_t cls_RingsAnalyser::ImportPixelMap(QString p_filename)
{
    std::ifstream v_mapFile(p_filename.toStdString());

    if (v_mapFile.is_open()) {

        std::string v_line;
        std::string v_delims("\t");
        while (getline(v_mapFile, v_line))
        {
            if (v_line.size() == 0) continue;
            if (v_line.substr(0, 1) == std::string("#")) continue;

            std::vector<std::string> v_parsedLine = tokenize(v_line, v_delims);

            /*for (std::vector<std::string>::iterator v_parsedLineIter = v_parsedLine.begin();
                 v_parsedLineIter != v_parsedLine.end(); ++v_parsedLineIter) {
                cout << (*v_parsedLineIter) << "\t";
            }
            cout << endl;*/

            UInt_t v_PMT = std::stoi(v_parsedLine[0]);
            UInt_t v_Pixel = std::stoi(v_parsedLine[1]);
            Double_t v_X = std::stod(v_parsedLine[2]);
            Double_t v_Y = std::stod(v_parsedLine[3]);
            UInt_t v_SimplX = std::stoi(v_parsedLine[4]);
            UInt_t v_SimplY = std::stoi(v_parsedLine[5]);
            Int_t v_TdcId = std::stoi(v_parsedLine[6], 0, 16);  // hex base
            UInt_t v_Lch = std::stoi(v_parsedLine[7]);
            UInt_t v_Tch = std::stoi(v_parsedLine[8]);
            UInt_t v_PADIWAnr = std::stoi(v_parsedLine[9]);
            UInt_t v_TRBnr = std::stoi(v_parsedLine[10]);
            TString v_PMTtype = TString(v_parsedLine[11]);

            cls_PixelMapEntry v_obj(v_PMT, v_Pixel, v_X, v_Y, v_SimplX, v_SimplY, v_TdcId, v_Lch, v_Tch, v_PADIWAnr, v_TRBnr, v_PMTtype);

            UInt_t v_tdcIndex = TDCidToInteger(v_TdcId);
            UInt_t v_pixelUID = PixelUID(v_tdcIndex, v_Lch);
            fPixelMap.insert(std::pair<UInt_t, cls_PixelMapEntry>(v_pixelUID, v_obj));
        }

        v_mapFile.close();
        cout << "Successfully read pixel map file " << p_filename.toStdString() << endl;
        return 0; // ok
    } else {
        cout << "Failed to read pixel map file " << p_filename.toStdString() << endl;
        return 1; // fail
    }

}

void cls_RingsAnalyser::RunAnalysis()
{
    cout << "Running reconstruction and rings analysis." << endl;

    cls_DataHandler* v_dataHandler = cls_DataHandler::Instance();
    std::vector<cls_Event>& v_Events = v_dataHandler->mEvents;

    cout << "Total " << v_Events.size() << " events." << endl;

    UInt_t v_eventI = 0;

    // Create ring finder
    CbmRichProtRingFinderHoughImpl* v_RingFinderHTimpl = new CbmRichProtRingFinderHoughImpl();
    v_RingFinderHTimpl->Init();

    // Loop over events
    for (std::vector<cls_Event>::iterator v_iterEvents=v_Events.begin(); v_iterEvents!=v_Events.end(); ++v_iterEvents)
    {
        UInt_t v_numHitsInEvent = (*v_iterEvents).GetSize();
        //cout << "Event size=" << v_numHitsInEvent << endl;

        vector<CbmRichHoughHit> v_recoInputEventHits;
        v_recoInputEventHits.reserve(v_numHitsInEvent);

        // Get hits of the current event
        std::vector<cls_Hit>& v_eventHits = (*v_iterEvents).mHits;

        // Loop over the hits of the events to form an input vector for reconstruction
        UInt_t iHit = 0;
        for (std::vector<cls_Hit>::iterator v_iterHits=v_eventHits.begin(); v_iterHits!=v_eventHits.end(); ++v_iterHits)
        {
            // Get the current hit and its parameters from the pixel map
            cls_Hit& curHit = (*v_iterHits);
            UInt_t v_pixelUID = PixelUID(curHit.GetTDC(), curHit.GetLch());
            Double_t v_hitX = fPixelMap.at(v_pixelUID).mX;
            Double_t v_hitY = fPixelMap.at(v_pixelUID).mY;

            //cout << iHit << ":\tX=" << v_hitX << "\t\tY=" << v_hitY << endl;

            CbmRichHoughHit v_tempPoint;
            v_tempPoint.fHit.fX = v_hitX/10.;
            v_tempPoint.fHit.fY = v_hitY/10.;
            v_tempPoint.fHit.fLtime = curHit.GetLtime();
            v_tempPoint.fHit.fTtime = curHit.GetTtime();
            v_tempPoint.fX2plusY2 = v_hitX*v_hitX/100. + v_hitY*v_hitY/100.;
            v_tempPoint.fId = iHit;
            v_tempPoint.fIsUsed = false;
            v_recoInputEventHits.push_back(v_tempPoint);

            iHit++;
        }

        v_RingFinderHTimpl->SetData(v_recoInputEventHits);

        // Actually run reconstruction
        v_RingFinderHTimpl->DoFind();

        //cout << "Found " << v_RingFinderHTimpl->GetFoundRings().size() << " rings." << endl;

        fhNumOfFoundRings->Fill(v_RingFinderHTimpl->GetFoundRings().size());

        /*if (v_RingFinderHTimpl->GetFoundRings().size() > 4) {
            cls_RingVisu* v_ringVisu = new cls_RingVisu(*v_iterEvents);
            v_ringVisu->show();
        }*/

        if (v_RingFinderHTimpl->GetFoundRings().size() == 1)
        {
            // The only ring
            CbmRichRingLight* v_theRing = v_RingFinderHTimpl->GetFoundRings().at(0);
            // Process this ring
            this->ProcessOneRing(v_theRing, v_eventHits);
        }

        v_eventI++;
    } // loop over events

    ProcessLeadingEdgeDiffMean();

    // Delete ring finder
    delete v_RingFinderHTimpl;

    cout << "Done reconstruction and rings analysis." << endl;
}

void cls_RingsAnalyser::ProcessOneRing(CbmRichRingLight* p_theRing, std::vector<cls_Hit>& p_eventHits)
{
    //cout << p_theRing->GetNofHits() << " hits in the ring." << endl;

    // Statistics before applying the cut
    fhNumOfHitsInTheRing->Fill(p_theRing->GetNofHits());
    fhRingsCenter->Fill(p_theRing->GetCenterX(), p_theRing->GetCenterY());
    fhRingsRadius->Fill(p_theRing->GetRadius());

    //TODO
    // cut on the ring center and radius
    // pos G
    if (p_theRing->GetRadius() < 4. || p_theRing->GetRadius() > 5.5 ||
        p_theRing->GetCenterX() < 15.2 || p_theRing->GetCenterX() > 17.5 ||
        p_theRing->GetCenterY() < 15.5 || p_theRing->GetCenterY() > 17.7) {
        // skipping the ring

    // pos GH
    //if (p_theRing->GetRadius() < 4.45 || p_theRing->GetRadius() > 4.8 ||
    //    p_theRing->GetCenterX() < 15.2 || p_theRing->GetCenterX() > 17.5 ||
    //    p_theRing->GetCenterY() < 13.2 || p_theRing->GetCenterY() > 14.6) {
    //    // skipping the ring
    } else {

        // Statistics after applying the cut
        fhNumOfHitsInTheRingInCut->Fill(p_theRing->GetNofHits());
        fhRingsCenterInCut->Fill(p_theRing->GetCenterX(), p_theRing->GetCenterY());
        fhRingsRadiusInCut->Fill(p_theRing->GetRadius());

        // ---------------------------------------------------------------------------------------------------------------------

        // Map of hits of the ring ordered by leading edge time
        std::map<Double_t, cls_Hit&> v_ringHits;

        // Loop over the hits of the ring
        for (Int_t i=0; i<p_theRing->GetNofHits(); i++) {
            //cout << "X=" << p_theRing->GetHit(i).fX << "\tY=" << p_theRing->GetHit(i).fY << endl;
            //cout << "Hit ID: " << p_theRing->GetHitId(i) << endl;

            UInt_t v_curHitId = p_theRing->GetHitId(i);
            cls_Hit& v_curOrigHit = p_eventHits.at(v_curHitId);

            v_ringHits.insert(std::pair<Double_t, cls_Hit&>(v_curOrigHit.GetLtime(), v_curOrigHit));
        }

        // Here we have the map of hits of the ring ready.
        // The first in the map is the reference hit

        // Interface.... This helps to keep the code in this class and EventsAnalyser class purely the same
        // "event" with ring hits only
        cls_Event v_ringHitsInEvent;

        for (std::map<Double_t, cls_Hit&>::iterator v_ringHitsI = v_ringHits.begin(); v_ringHitsI != v_ringHits.end(); ++v_ringHitsI) {
            v_ringHitsInEvent.AddHit(v_ringHitsI->second);
        }

        std::vector<cls_Event> v_eventsVector;
        v_eventsVector.push_back(v_ringHitsInEvent);
        std::vector<cls_Event>::iterator v_iterEvents=v_eventsVector.begin();

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

        // ---------------------------------------------------------------------------------------------------------------------
    }
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
// One event as input
void cls_RingsAnalyser::ProcessPerTDC(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_TdcId, UInt_t p_histoIndex)
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

            fhLeadingEdgeDiffOnePADIWASymm[p_histoIndex]->Fill(v_diff);

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiffOnePADIWA[p_histoIndex]->Fill(v_diff);
            else
                fhLeadingEdgeDiffOnePADIWA[p_histoIndex]->Fill(-v_diff);
        }
    }
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
// p_PmtId starts from 1
void cls_RingsAnalyser::ProcessPerPMT(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_PmtId, UInt_t p_histoIndex)
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

            fhLeadingEdgeDiffOnePMTSymm[p_histoIndex]->Fill(v_diff);

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiffOnePMT[p_histoIndex]->Fill(v_diff);
            else
                fhLeadingEdgeDiffOnePMT[p_histoIndex]->Fill(-v_diff);
        }
    }
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
void cls_RingsAnalyser::ProcessTRQ(std::vector<cls_Event>::iterator p_iterEvents)
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

            fhLeadingEdgeDiffTopRightQuarterSymm->Fill(v_diff);

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiffTopRightQuarter->Fill(v_diff);
            else
                fhLeadingEdgeDiffTopRightQuarter->Fill(-v_diff);
        }
    }
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
// One event as input
void cls_RingsAnalyser::ProcessPerTDCGood(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_TdcId, UInt_t p_histoIndex)
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

            fhLeadingEdgeDiffOnePADIWAGoodSymm[p_histoIndex]->Fill(v_diff);

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiffOnePADIWAGood[p_histoIndex]->Fill(v_diff);
            else
                fhLeadingEdgeDiffOnePADIWAGood[p_histoIndex]->Fill(-v_diff);
        }
    }
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
// p_PmtId starts from 1
void cls_RingsAnalyser::ProcessPerPMTGood(std::vector<cls_Event>::iterator p_iterEvents, UInt_t p_PmtId, UInt_t p_histoIndex)
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

            fhLeadingEdgeDiffOnePMTGoodSymm[p_histoIndex]->Fill(v_diff);

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiffOnePMTGood[p_histoIndex]->Fill(v_diff);
            else
                fhLeadingEdgeDiffOnePMTGood[p_histoIndex]->Fill(-v_diff);
        }
    }
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
void cls_RingsAnalyser::ProcessTRQGood(std::vector<cls_Event>::iterator p_iterEvents)
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

            fhLeadingEdgeDiffTopRightQuarterGoodSymm->Fill(v_diff);

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiffTopRightQuarterGood->Fill(v_diff);
            else
                fhLeadingEdgeDiffTopRightQuarterGood->Fill(-v_diff);
        }
    }
}

// Only for top-right quarter! PMTs 1, 2, 5, 6.
void cls_RingsAnalyser::ProcessLeadingEdgeDiff(std::vector<cls_Event>::iterator p_iterEvents)
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
                fhLeadingEdgeDiffSymm[v_pixel1uid][v_pixel2uid]->Fill(v_diff);
            else
                fhLeadingEdgeDiffSymm[v_pixel2uid][v_pixel1uid]->Fill(v_diff);

            if (v_pixel2uid > v_pixel1uid)
                fhLeadingEdgeDiff[v_pixel1uid][v_pixel2uid]->Fill(v_diff);
            else
                fhLeadingEdgeDiff[v_pixel2uid][v_pixel1uid]->Fill(-v_diff);
        }
    }
}

void cls_RingsAnalyser::ProcessLeadingEdgeDiffMean(void)
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

UInt_t cls_RingsAnalyser::ExportHistos(TString p_filename)
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

    fhNumOfFoundRings->Write();

    fhNumOfHitsInTheRing->Write();
    fhRingsCenter->Write();
    fhRingsRadius->Write();
    fhNumOfHitsInTheRingInCut->Write();
    fhRingsCenterInCut->Write();
    fhRingsRadiusInCut->Write();

    // Copied from the EventsAnalysis class -----------------------------------

    // ----------------------------------------------------------------------------------------------------------------------------------
    // Without filtering bad channels

    gDirectory->mkdir("NoFiltering");
    gDirectory->cd("NoFiltering");

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

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        fhLeadingEdgeDiffOnePMTSymm[v_pmt]->Write();
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        fhLeadingEdgeDiffOnePADIWASymm[v_padiwa]->Write();
    }

    fhLeadingEdgeDiffTopRightQuarterSymm->Write();

    gDirectory->cd("..");

    // ----------------------------------------------------------------------------------------------------------------------------------
    // With filtering bad channels

    gDirectory->mkdir("WithFiltering");
    gDirectory->cd("WithFiltering");

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

    for (UInt_t v_pmt=0; v_pmt<4; v_pmt++) {
        fhLeadingEdgeDiffOnePMTGoodSymm[v_pmt]->Write();
    }
    for (UInt_t v_padiwa=0; v_padiwa<16; v_padiwa++) {
        fhLeadingEdgeDiffOnePADIWAGoodSymm[v_padiwa]->Write();
    }

    fhLeadingEdgeDiffTopRightQuarterGoodSymm->Write();

    gDirectory->cd("..");

    // ----------------------------------------------------------------------------------------------------------------------------------
    // fhLeadingEdgeDiff for each pair of pixels is here after all the other histograms

    gDirectory->mkdir("LeadingEdgeDiff");
    gDirectory->cd("LeadingEdgeDiff");

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

    gDirectory->cd("..");

    gDirectory->mkdir("LeadingEdgeDiff_Symm");
    gDirectory->cd("LeadingEdgeDiff_Symm");

    for (UInt_t v_pixel1=0; v_pixel1<256-1; v_pixel1++) {
        TString dirName;
        dirName.Form("LeadingEdgeDiffSymm_ID1_%02d", v_pixel1);
        gDirectory->mkdir(dirName);
        gDirectory->cd(dirName);
        for (UInt_t v_pixel2=v_pixel1+1; v_pixel2<256; v_pixel2++) {
            fhLeadingEdgeDiffSymm[v_pixel1][v_pixel2]->Write();
        }
        gDirectory->cd("..");
    }

    gDirectory->cd("..");
    // ------------------------------------------------------------------------

    v_outputFile.Close();

    cout << "Successfully exported analysis histograms into " << p_filename.Data() << "." << endl;

    gDirectory = prevDir;
    gFile = prevFile;
    return 0; // OK
}

/* Just a wrapper */
UInt_t cls_RingsAnalyser::ExportHistos(QString p_qfilename)
{
    char v_cfilename[255];
    strcpy(v_cfilename, p_qfilename.toStdString().c_str());
    TString v_filename(v_cfilename);
    return this->ExportHistos(v_filename);
}
