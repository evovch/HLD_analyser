#include "Unpacking_info.h"
#include "ui_Unpacking_info.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <TH1D.h>
#include <TFile.h>

#include "global_defines.h"
#include "RawMessage.h"

cls_Unpacking_info::cls_Unpacking_info(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cls_Unpacking_info),
    mLastLaserTimestampL(0.),
    mLastLaserSetL(kFALSE),
    mLastLaserTimestampT(0.),
    mLastLaserSetT(kFALSE)
{
    ui->setupUi(this);

    mhEdgesPerTDC = new TH1D("mhEdgesPerTDC", "mhEdgesPerTDC", NUMTDCs, 0., (Double_t)NUMTDCs);
    mhLaserPeriodL = new TH1D("mhLaserPeriodL", "mhLaserPeriodL", 10000, 0., 1.e8);
    mhLaserPeriodT = new TH1D("mhLaserPeriodT", "mhLaserPeriodT", 10000, 0., 1.e8);

    mGoodMessagesPerChannel = new TH1D("mGoodMessagesPerChannel", "mGoodMessagesPerChannel", NUMTDCs*NUMCHs, 0., (Double_t)(NUMTDCs*NUMCHs));
    mFaultyMessagesPerChannel = new TH1D("mFaultyMessagesPerChannel", "mFaultyMessagesPerChannel", NUMTDCs*NUMCHs, 0., (Double_t)(NUMTDCs*NUMCHs));

    mCounterPerChannel = new TH1D("mCounterPerChannel", "mCounterPerChannel", 4*NUMCHs, 0., (Double_t)(4*NUMCHs));

}

cls_Unpacking_info::~cls_Unpacking_info()
{
    delete ui;
}

void cls_Unpacking_info::AddMessage(cls_RawMessage p_mess)
{
    mhEdgesPerTDC->Fill(p_mess.mTDC);
    mGoodMessagesPerChannel->Fill(p_mess.mTDC*NUMCHs + p_mess.mChannel);

    //TODO implement diagnostics of the unpacking
    this->ProcessBeamDetectorMessage(p_mess);
}

void cls_Unpacking_info::AddFaultyMessage(UInt_t p_tdcUid, UInt_t p_channel)
{
    mFaultyMessagesPerChannel->Fill(p_tdcUid*NUMCHs + p_channel);
}

void cls_Unpacking_info::ProcessBeamDetectorMessage(cls_RawMessage p_mess)
{
    // Only for TDCs >= 0x0110, i.e. 0x0110, 0x0111, 0x0112, 0x0113
    if (p_mess.mTDC < 64) return;

    mCounterPerChannel->Fill((p_mess.mTDC-64)*NUMCHs+p_mess.mChannel);

    // Laser message processing - leading edge
    if (p_mess.mTDC == 64 && p_mess.mChannel == 15) {
        Double_t curTimestamp = p_mess.mFullTime;
        if (mLastLaserSetL) {
            mhLaserPeriodL->Fill(curTimestamp - mLastLaserTimestampL);
            mLastLaserTimestampL = curTimestamp;
        } else {
            mLastLaserTimestampL = curTimestamp;
            mLastLaserSetL = kTRUE;
        }
    }

    // Laser message processing - trailing edge
    if (p_mess.mTDC == 64 && p_mess.mChannel == 16) {
        Double_t curTimestamp = p_mess.mFullTime;
        if (mLastLaserSetT) {
            mhLaserPeriodT->Fill(curTimestamp - mLastLaserTimestampT);
            mLastLaserTimestampT = curTimestamp;
        } else {
            mLastLaserTimestampT = curTimestamp;
            mLastLaserSetT = kTRUE;
        }
    }
}

UInt_t cls_Unpacking_info::Export(TString p_filename)
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

    mhEdgesPerTDC->Write();
    mhLaserPeriodL->Write();
    mhLaserPeriodT->Write();
    mGoodMessagesPerChannel->Write();
    mFaultyMessagesPerChannel->Write();
    mCounterPerChannel->Write();

    v_outputFile.Close();

    cout << "Successfully exported unpacking info into " << p_filename << "." << endl;

    gDirectory = prevDir;
    gFile = prevFile;
    return 0; // OK
}

/* Just a wrapper */
UInt_t cls_Unpacking_info::Export(QString p_qfilename)
{
    char v_cfilename[255];
    strcpy(v_cfilename, p_qfilename.toStdString().c_str());
    TString v_filename(v_cfilename);
    return this->Export(v_filename);
}
