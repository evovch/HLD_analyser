#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <QFileDialog>
#include <QStringList>

#include <TString.h>
#include <TFile.h>
#include <TH1D.h>

UInt_t cls_MainWindow::fMinimumEntries = 100;

cls_MainWindow::cls_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cls_MainWindow)
{
    ui->setupUi(this);
}

cls_MainWindow::~cls_MainWindow()
{
    delete ui;
}

void cls_MainWindow::SelectInputFiles(void)
{
    QFileDialog v_dial;
    v_dial.setFileMode(QFileDialog::ExistingFiles);
    v_dial.setNameFilter(tr("Root files (*.root)"));
    v_dial.setDirectory("/home/evovch/DATA/");

    QStringList v_fileNames;
    if (v_dial.exec()) {
        v_fileNames = v_dial.selectedFiles();
        ui->listWidget->addItems(v_fileNames);
    }
}

void cls_MainWindow::SelectOutputFile(void)
{
    QFileDialog v_dial;
    v_dial.setFileMode(QFileDialog::AnyFile);
    v_dial.setNameFilter(tr("Root files (*.root)"));
    v_dial.setDirectory("/home/evovch/DATA/");

    QStringList v_fileNames;
    if (v_dial.exec()) {
        v_fileNames = v_dial.selectedFiles();
        QString v_selectedFile = v_fileNames.at(0);
        if (!v_selectedFile.endsWith(".root")) v_selectedFile.append(".root");
        ui->lineEdit->setText(v_selectedFile);
    }
}

void cls_MainWindow::InitHistos(void)
{
    /* The effect of this line is AMAZING */
    TH1::AddDirectory(kFALSE);

    // Init histograms
    TString histoName;
    TString histoTitle;
    for (UInt_t tdc=0; tdc<NUMTDCs; tdc++) {
        UInt_t v_tdcId = IntegerToTDCid(tdc);

        // Reset 'calibration done' flags
        histoName.Form("CalDone_%04x", v_tdcId);
        histoTitle.Form("Calibration done for TDC %04x", v_tdcId);
        fCalibDone[tdc] = new TH1C(histoName.Data(), histoTitle.Data(), 33, 0., 33.);

//        histoName.Form("CalEntries_%04x", v_tdcId);
//        histoTitle.Form("Number of entries used for calibration for TDC %04x", v_tdcId);
//        fCalibEntries[tdc] = new TH1I(histoName.Data(), histoTitle.Data(), 33, 0., 33.);

        for (UInt_t ch=0; ch<NUMCHs; ch++) {
//            histoName.Form("FineBuffer_%04x_%02d", v_tdcId, ch);
//            histoTitle.Form("Fine time buffer for TDC %04x ch %02d", v_tdcId, ch);
//            fFineBuffer[tdc][ch] = new TH1D(histoName.Data(), histoTitle.Data(), 1024, 0., 1024.);

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
}

void cls_MainWindow::Merge(void)
{
    this->InitHistos();

    TString histoName;

    for (int i=0; i<ui->listWidget->count(); i++) {
        QListWidgetItem* v_item = ui->listWidget->item(i);
        QString v_curLine = (QString)v_item->text();

        TFile v_curFile(v_curLine.toStdString().c_str(), "READ");

        for (unsigned int v_pmt = 1; v_pmt < 18; v_pmt++) {
            for (unsigned int v_tdc = 0; v_tdc < 4; v_tdc++) {
                unsigned int v_index = v_pmt*16 + v_tdc;

                UInt_t v_tdcId = IntegerToTDCid((v_pmt-1)*4+v_tdc);
                histoName.Form("CalEntries_%04x", v_tdcId);
                TH1I* curHisto2;
                curHisto2 = (TH1I*)v_curFile.Get(histoName);
                if (i==0) {
                    fCalibEntries[(v_pmt-1)*4+v_tdc] = (TH1I*)v_curFile.Get(histoName);
                    fCalibEntries[(v_pmt-1)*4+v_tdc]->SetDirectory(0);
                } else {
                    fCalibEntries[(v_pmt-1)*4+v_tdc]->Add(curHisto2);
                }


                TString folderName;
                folderName.Form("TDC%04x", v_index);
                for (unsigned int v_channel=0; v_channel<NUMCHs; v_channel++) {
                    histoName.Form("%s/FineBuffer_%04x_%02d", folderName.Data(), v_index, v_channel);

                    TH1D* curHisto;
                    curHisto = (TH1D*)v_curFile.Get(histoName);
                    //cout << curHisto->GetName() << "\t\t" << curHisto->GetEntries() << endl;

                    if (i==0) {
                        fFineBuffer[(v_pmt-1)*4+v_tdc][v_channel] = (TH1D*)curHisto->Clone();
                        fFineBuffer[(v_pmt-1)*4+v_tdc][v_channel]->SetDirectory(0);
                    } else {
                        fFineBuffer[(v_pmt-1)*4+v_tdc][v_channel]->Add(curHisto);
                    }

                }
            }
        }

        v_curFile.Close();

    }

    this->CalibrateAll();

    this->Export();
}

UInt_t cls_MainWindow::Export(void)
{
    TDirectory* prevDir = gDirectory;
    TFile* prevFile = gFile;

    TFile v_outputFile(ui->lineEdit->text().toStdString().c_str(), "RECREATE");

    if (v_outputFile.IsZombie()) {
        cerr << "Error opening file " << ui->lineEdit->text().toStdString() << endl;
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

    cout << "Successfully exported calibration tables into " << ui->lineEdit->text().toStdString() << "." << endl;

    gDirectory = prevDir;
    gFile = prevFile;
    return 0; // OK
}

/*
void cls_MainWindow::Export(void)
{
    TFile v_outFile(ui->lineEdit->text().toStdString().c_str(), "RECREATE");

    for (unsigned int v_tdc=0; v_tdc<NUMTDCs; v_tdc++) {
        UInt_t v_tdcId = IntegerToTDCid(v_tdc);
        TString dirName;
        dirName.Form("TDC%04x", v_tdcId);
        gDirectory->mkdir(dirName);
        gDirectory->cd(dirName);
        for (unsigned int j=0; j<NUMCHs; j++) {
            fFineBuffer[v_tdc][j]->Write();
        }
        gDirectory->cd("..");
    }

    v_outFile.Close();
}
*/

/* Perform calibration of one channel */
UInt_t cls_MainWindow::CalibrateOneChannel(UInt_t p_tdcId, UInt_t p_ch)
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
void cls_MainWindow::CalibrateOneTDC(UInt_t p_tdcId)
{
    for (UInt_t ch=0; ch<NUMCHs; ch++) {
        this->CalibrateOneChannel(p_tdcId, ch);
    }
}

/* Simply run calibration procedure for each TDC */
void cls_MainWindow::CalibrateAll(void)
{
    for (UInt_t tdc=0; tdc<NUMTDCs; tdc++) {
        this->CalibrateOneTDC(tdc);
    }
}
