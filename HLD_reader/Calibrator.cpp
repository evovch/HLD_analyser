#include "Calibrator.h"

#include <TH1D.h>
#include <TFile.h>
#include <TDirectory.h>

#include <cstdio>

#include <fstream>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

UInt_t cls_Calibrator::fMinimumEntries = 100;

cls_Calibrator::cls_Calibrator() :
    fAllocated(kFALSE)
{
    /* The effect of this line is AMAZING */
    TH1::AddDirectory(kFALSE);

    // Clear correction table
    for (UInt_t i=0; i<1024; i++) fCorrections[i] = 0.;
}

cls_Calibrator::~cls_Calibrator()
{
    this->Deallocate();
}

void cls_Calibrator::Allocate(void)
{
    if (fAllocated) this->Deallocate();

    // Init histograms
    TString histoName;
    TString histoTitle;
    for (UInt_t tdc=0; tdc<NUMTDCs; tdc++) {
        UInt_t v_tdcId = IntegerToTDCid(tdc);

        // Reset 'calibration done' flags
        histoName.Form("CalDone_%04x", v_tdcId);
        histoTitle.Form("Calibration done for TDC %04x", v_tdcId);
        fCalibDone[tdc] = new TH1C(histoName.Data(), histoTitle.Data(), 33, 0., 33.);

        histoName.Form("CalEntries_%04x", v_tdcId);
        histoTitle.Form("Number of entries used for calibration for TDC %04x", v_tdcId);
        fCalibEntries[tdc] = new TH1I(histoName.Data(), histoTitle.Data(), 33, 0., 33.);

        for (UInt_t ch=0; ch<NUMCHs; ch++) {
            histoName.Form("FineBuffer_%04x_%02d", v_tdcId, ch);
            histoTitle.Form("Fine time buffer for TDC %04x ch %02d", v_tdcId, ch);
            fFineBuffer[tdc][ch] = new TH1D(histoName.Data(), histoTitle.Data(), 1024, 0., 1024.);

            histoName.Form("CalcBinWidth_%04x_%02d", v_tdcId, ch);
            histoTitle.Form("Calculated bin width for TDC %04x ch %02d", v_tdcId, ch);
            fCalcBinWidth[tdc][ch] = new TH1D(histoName.Data(), histoTitle.Data(), 1024, 0., 1024.);

            histoName.Form("CalTable_%04x_%02d", v_tdcId, ch);
            histoTitle.Form("Calibration table for TDC %04x ch %02d", v_tdcId, ch);
            fCalTable[tdc][ch] = new TH1D(histoName.Data(), histoTitle.Data(), 1024, 0., 1024.);

            histoName.Form("CalTableMinusLinear_%04x_%02d", v_tdcId, ch);
            histoTitle.Form("Calibration table for TDC %04x ch %02d minus linear function", v_tdcId, ch);
            fCalTableMinusLinear[tdc][ch] = new TH1D(histoName.Data(), histoTitle.Data(), 1024, 0., 1024.);

            // initialize with 1 (horisontal constant function)
            for (UInt_t ibin=1; ibin<=1024; ibin++) {
                fCalcBinWidth[tdc][ch]->SetBinContent(ibin, 1.);
            }
            // initialize with linear function
            for (UInt_t ibin=1; ibin<=512; ibin++) {
                fCalTable[tdc][ch]->SetBinContent(ibin, 5.*(Double_t)ibin/512.);
            }
            for (UInt_t ibin=513; ibin<=1024; ibin++) {
                fCalTable[tdc][ch]->SetBinContent(ibin, 5.);
            }
        }
    }

    fAllocated = kTRUE;
}

void cls_Calibrator::Deallocate(void)
{
    if (!fAllocated) return;

    for (UInt_t tdc=0; tdc<NUMTDCs; tdc++) {
        delete fCalibDone[tdc];                     fCalibDone[tdc] = nullptr;
        delete fCalibEntries[tdc];                  fCalibEntries[tdc] = nullptr;
        for (UInt_t ch=0; ch<NUMCHs; ch++) {
            delete fFineBuffer[tdc][ch];            fFineBuffer[tdc][ch] = nullptr;
            delete fCalcBinWidth[tdc][ch];          fCalcBinWidth[tdc][ch] = nullptr;
            delete fCalTable[tdc][ch];              fCalTable[tdc][ch] = nullptr;
            delete fCalTableMinusLinear[tdc][ch];   fCalTableMinusLinear[tdc][ch] = nullptr;
        }
    }

    fAllocated = kFALSE;
}

/*void cls_Calibrator::Reset(void)
{
    if (!fAllocated) return;

    for (UInt_t v_tdc=0; v_tdc<NUMTDCs; v_tdc++) {
        fCalibDone[v_tdc]->Reset();
        fCalibEntries[v_tdc]->Reset();

        for (UInt_t v_ch=0; v_ch<NUMCHs; v_ch++) {
            fFineBuffer[v_tdc][v_ch]->Reset();
            fCalcBinWidth[v_tdc][v_ch]->Reset();
            fCalTable[v_tdc][v_ch]->Reset();
            fCalTableMinusLinear[v_tdc][v_ch]->Reset();

            // initialize with 1 (horisontal constant function)
            for (UInt_t ibin=1; ibin<=1024; ibin++) {
                fCalcBinWidth[v_tdc][v_ch]->SetBinContent(ibin, 1.);
            }

            // initialize with linear function
            for (UInt_t ibin=1; ibin<=512; ibin++) {
                fCalTable[v_tdc][v_ch]->SetBinContent(ibin, 5.*(Double_t)ibin/512.);
            }
            for (UInt_t ibin=513; ibin<=1024; ibin++) {
                fCalTable[v_tdc][v_ch]->SetBinContent(ibin, 5.);
            }
        }
    }
}*/

//TODO check
cls_Calibrator& cls_Calibrator::operator=(const cls_Calibrator& other)
{
    if (this != &other) {

        if (!other.fAllocated) return *this;

        if (fAllocated) this->Deallocate();

        for (UInt_t v_tdc=0; v_tdc<NUMTDCs; v_tdc++) {
            fCalibDone[v_tdc] = (TH1C*)other.fCalibDone[v_tdc]->Clone();
            fCalibEntries[v_tdc] = (TH1I*)other.fCalibEntries[v_tdc]->Clone();
            for (UInt_t v_ch=0; v_ch<NUMCHs; v_ch++) {
                fFineBuffer[v_tdc][v_ch] = (TH1D*)other.fFineBuffer[v_tdc][v_ch]->Clone();
                fCalcBinWidth[v_tdc][v_ch] = (TH1D*)other.fCalcBinWidth[v_tdc][v_ch]->Clone();
                fCalTable[v_tdc][v_ch] = (TH1D*)other.fCalTable[v_tdc][v_ch]->Clone();
                fCalTableMinusLinear[v_tdc][v_ch] = (TH1D*)other.fCalTableMinusLinear[v_tdc][v_ch]->Clone();
            }
        }

        for (UInt_t i=0; i<1024; i++) {
            fCorrections[i] = other.fCorrections[i];
        }

        fAllocated = kTRUE;
    }
    return *this;
}

UInt_t cls_Calibrator::Import(TString p_filename)
{
    TDirectory* prevDir = gDirectory;
    TFile* prevFile = gFile;

    TFile v_inputFile(p_filename, "READ");

    if (v_inputFile.IsZombie()) {
        cerr << "Error opening file " << p_filename << endl;
        gDirectory = prevDir;
        gFile = prevFile;
        return 1; // FAIL
    }

    if (fAllocated) this->Deallocate();


    TString histoName;
    TH1D* curHisto;

    for (UInt_t v_tdc=0; v_tdc<NUMTDCs; v_tdc++) {
        UInt_t v_tdcId = IntegerToTDCid(v_tdc);

        TH1C* curHistoC;
        TH1I* curHistoI;

        histoName.Form("CalDone_%04x", v_tdcId);
        curHistoC = (TH1C*)v_inputFile.Get(histoName);
        fCalibDone[v_tdc] = (TH1C*)curHistoC->Clone();

        histoName.Form("CalEntries_%04x", v_tdcId);
        curHistoI = (TH1I*)v_inputFile.Get(histoName);
        fCalibEntries[v_tdc] = (TH1I*)curHistoI->Clone();
    }

    for (UInt_t v_tdc=0; v_tdc<NUMTDCs; v_tdc++) {
        UInt_t v_tdcId = IntegerToTDCid(v_tdc);

        TString dirName;

        dirName.Form("TDC%04x", v_tdcId);

        for (UInt_t v_ch=0; v_ch<NUMCHs; v_ch++) {
            histoName.Form("%s/FineBuffer_%04x_%02d", dirName.Data(), v_tdcId, v_ch);
            curHisto = (TH1D*)v_inputFile.Get(histoName);
            fFineBuffer[v_tdc][v_ch] = (TH1D*)curHisto->Clone();

            histoName.Form("%s/CalcBinWidth_%04x_%02d", dirName.Data(), v_tdcId, v_ch);
            curHisto = (TH1D*)v_inputFile.Get(histoName);
            fCalcBinWidth[v_tdc][v_ch] = (TH1D*)curHisto->Clone();

            histoName.Form("%s/CalTable_%04x_%02d", dirName.Data(), v_tdcId, v_ch);
            curHisto = (TH1D*)v_inputFile.Get(histoName);
            fCalTable[v_tdc][v_ch] = (TH1D*)curHisto->Clone();

            histoName.Form("%s/CalTableMinusLinear_%04x_%02d", dirName.Data(), v_tdcId, v_ch);
            curHisto = (TH1D*)v_inputFile.Get(histoName);
            fCalTableMinusLinear[v_tdc][v_ch] = (TH1D*)curHisto->Clone();
        }
    }

    v_inputFile.Close();

    cout << "Successfully imported calibration tables from " << p_filename << "." << endl;
    fAllocated = kTRUE;

    gDirectory = prevDir;
    gFile = prevFile;
    return 0; // OK
}

UInt_t cls_Calibrator::Export(TString p_filename)
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

    for (UInt_t v_tdc=0; v_tdc<NUMTDCs; v_tdc++) {
        fCalibDone[v_tdc]->Write();
        fCalibEntries[v_tdc]->Write();
    }

    for (UInt_t v_tdc=0; v_tdc<NUMTDCs; v_tdc++) {
        UInt_t v_tdcId = IntegerToTDCid(v_tdc);
        TString dirName;
        dirName.Form("TDC%04x", v_tdcId);
        gDirectory->mkdir(dirName);
        gDirectory->cd(dirName);
        for (UInt_t v_ch=0; v_ch<NUMCHs; v_ch++) {
            fFineBuffer[v_tdc][v_ch]->Write();
            fCalcBinWidth[v_tdc][v_ch]->Write();
            fCalTable[v_tdc][v_ch]->Write();
            fCalTableMinusLinear[v_tdc][v_ch]->Write();
        }
        gDirectory->cd("..");
    }

    v_outputFile.Close();

    cout << "Successfully exported calibration tables into " << p_filename << "." << endl;

    gDirectory = prevDir;
    gFile = prevFile;
    return 0; // OK
}

/* When a message with time information is unpacked fine time counter value is sent here */
void cls_Calibrator::AddFineTime(UInt_t p_tdcId, UInt_t p_ch, UInt_t p_fine)
{
    UInt_t v_tdcUID = TDCidToInteger(p_tdcId);
    fFineBuffer[v_tdcUID][p_ch]->Fill(p_fine);
    fCalibEntries[v_tdcUID]->Fill((Double_t)p_ch+0.5);
}

/* Return full time taking calibration into account */
Double_t cls_Calibrator::GetFullTime(UInt_t p_tdcId, UInt_t p_ch, UInt_t p_epoch, UInt_t p_coarse, UInt_t p_fine)
{
    UInt_t v_tdcUID = TDCidToInteger(p_tdcId);
    Double_t v_fineTime = 0.;
    if (fCalibDone[v_tdcUID]->GetBinContent(p_ch+1) == 1) {                  // +1 because 0-th bin is underflow bin
        v_fineTime = fCalTable[v_tdcUID][p_ch]->GetBinContent(p_fine+1);
    }
    Double_t v_correction = 0.;
    if (p_ch > 0) v_correction = fCorrections[v_tdcUID*16+((p_ch-1)/2)];
    return ((Double_t)p_epoch*5.*2048. + (Double_t)p_coarse*5. - v_fineTime - v_correction);
}

/* Perform calibration of one channel */
UInt_t cls_Calibrator::CalibrateOneChannel(UInt_t p_tdcId, UInt_t p_ch)
{
    // Do the calibration
    UInt_t sum = fFineBuffer[p_tdcId][p_ch]->GetEntries();

    if (sum < fMinimumEntries) {
        return 1; // FAIL
    }

    for (UInt_t ibin=0; ibin<1024; ibin++)
    {
        UInt_t binHits = fFineBuffer[p_tdcId][p_ch]->GetBinContent(ibin+1);

        Double_t binWidth = 1.;
        if (sum) {
            binWidth = 5. * binHits / sum;
        }

        fCalcBinWidth[p_tdcId][p_ch]->SetBinContent(ibin+1, binWidth);

        Double_t calbintime = 1.;
        if (ibin == 0)
            calbintime = fCalcBinWidth[p_tdcId][p_ch]->GetBinContent(ibin + 1) / 2;
        else
            calbintime = fCalTable[p_tdcId][p_ch]->GetBinContent(ibin) +
                        (fCalcBinWidth[p_tdcId][p_ch]->GetBinContent(ibin) + fCalcBinWidth[p_tdcId][p_ch]->GetBinContent(ibin + 1)) / 2;

        fCalTable[p_tdcId][p_ch]->SetBinContent(ibin + 1, calbintime);
    }

    fCalibDone[p_tdcId]->SetBinContent(p_ch+1, 1);      // +1 because 0-th bin is underflow bin

    // Find the linear function to subtract
    Bool_t firstFound = kFALSE;
    Bool_t lastFound = kFALSE;
    UInt_t firstIbin = 0;
    UInt_t lastIbin = 1024;

    for (UInt_t ibin=0; ibin<1024; ibin++) {
        UInt_t binHits = fFineBuffer[p_tdcId][p_ch]->GetBinContent(ibin+1);
        if (!firstFound && binHits > 0) {
            firstIbin = ibin+1;
            firstFound = kTRUE;
            break;
        }
    }

    for (UInt_t ibin=1024; ibin>0; ibin--) {
        UInt_t binHits = fFineBuffer[p_tdcId][p_ch]->GetBinContent(ibin);
        if (!lastFound && binHits > 0) {
            lastIbin = ibin;
            lastFound = kTRUE;
            break;
        }
    }

    // If found - subtract
    if (firstFound && lastFound) {
        for (UInt_t ibin=firstIbin; ibin<=lastIbin; ibin++) {
            Double_t g = 5.*(Double_t)(ibin-firstIbin)/(Double_t)(lastIbin-firstIbin+1);    // In principle one can make not +1 but +2
            fCalTableMinusLinear[p_tdcId][p_ch]->SetBinContent(ibin, fCalTable[p_tdcId][p_ch]->GetBinContent(ibin) - g);
        }
    }

    return 0; // OK
}

/* Simply run calibration procedure for each channel of one TDC */
void cls_Calibrator::CalibrateOneTDC(UInt_t p_tdcId)
{
    for (UInt_t ch=0; ch<NUMCHs; ch++) {
        this->CalibrateOneChannel(p_tdcId, ch);
    }
}

/* Simply run calibration procedure for each TDC */
void cls_Calibrator::CalibrateAll(void)
{
    for (UInt_t tdc=0; tdc<NUMTDCs; tdc++) {
        this->CalibrateOneTDC(tdc);
    }
}

/* Set flags to all channels that calibration has been done
   Together with correction initialization of calibration tables
   with linear function with results in having fine time with calibration */
void cls_Calibrator::SetNoCalibration(void)
{
    if (!fAllocated) this->Allocate();

    for (UInt_t i=0; i<NUMTDCs; i++) {
        for (UInt_t v_ch=0; v_ch<33; v_ch++) {
            fCalibDone[i]->SetBinContent(v_ch+1, 1);
        }
    }
}

UInt_t cls_Calibrator::ImportCorrections(TString p_filename)
{
    std::ifstream v_inFile;
    v_inFile.open(p_filename.Data());

    if (!v_inFile.is_open()) {
        cout << "Failed to open file " << p_filename.Data() << endl;
        return 1; // FAIL
    }

    std::string v_line;
    UInt_t v_counter=0;
    while (getline(v_inFile, v_line)) {
        fCorrections[v_counter] = std::stod(v_line);
        v_counter++;
    }

    if (v_counter != 1024) cout << "Imported not 1024 correction values. Something is wrong." << endl;

    v_inFile.close();

    cout << "Successfully imported corrections table from " << p_filename.Data() << endl;

    return 0; // ok
}

/* Just a wrapper */
UInt_t cls_Calibrator::ImportCorrections(QString p_qfilename)
{
    char v_cfilename[255];
    strcpy(v_cfilename, p_qfilename.toStdString().c_str());
    TString v_filename(v_cfilename);
    return this->ImportCorrections(v_filename);
}

/* Just a wrapper */
UInt_t cls_Calibrator::Import(QString p_qfilename)
{
    char v_cfilename[255];
    strcpy(v_cfilename, p_qfilename.toStdString().c_str());
    TString v_filename(v_cfilename);
    return this->Import(v_filename);
}

/* Just a wrapper */
UInt_t cls_Calibrator::Export(QString p_qfilename)
{
    char v_cfilename[255];
    strcpy(v_cfilename, p_qfilename.toStdString().c_str());
    TString v_filename(v_cfilename);
    return this->Export(v_filename);
}
