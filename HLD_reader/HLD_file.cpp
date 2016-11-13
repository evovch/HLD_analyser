#include "HLD_file.h"

#include <iostream>
#include <fstream>
using std::cout;
using std::cerr;
using std::endl;

#include <cstdio>

#include "Rtypes.h"

#include "ui_MainWindow.h"

#include "Unpacking_info.h"
#include "RawMessage.h"

UInt_t cls_HLD_file::mVerbosityLevel = 0;

cls_HLD_file::cls_HLD_file() :
    mRawData(nullptr),
    mCurEpochCounter(0),
    mEdgeMatcher(new cls_EdgeMatcher()),
    mAnalyser(new cls_EventsAnalyser()),
    mUnpackingInfo(new cls_Unpacking_info()),
    mRingsAnalyser(new cls_RingsAnalyser()),
    mDirectTDCanalyser(nullptr),
    mRawEventsCounter(0),
    mRunDirectTDCanalysis(kFALSE)
{
}

cls_HLD_file::~cls_HLD_file()
{
    if (mRawData) { delete [] mRawData; mRawData = nullptr; }
    delete mEdgeMatcher;    mEdgeMatcher = nullptr;
    delete mAnalyser;       mAnalyser = nullptr;
    delete mUnpackingInfo;  mUnpackingInfo = nullptr;
    delete mRingsAnalyser;  mRingsAnalyser = nullptr;
    if (mDirectTDCanalyser) { delete mDirectTDCanalyser; mDirectTDCanalyser = nullptr; }
}

UInt_t cls_HLD_file::Import(TString p_filename, Bool_t p_doCalibration)
{
    cout << "Importing file " << p_filename << endl;

    if (!mCalibrator.fAllocated) mCalibrator.Allocate();

    if (this->ReadRawData(p_filename) == 1)
        return 1; // fail

    //this->Dump(10240);

    cout << "Unpacking input data..." << endl;
    this->Process();

    if (p_doCalibration)
        mCalibrator.CalibrateAll();

    //TODO implement further
    //cls_Unpacking_info* infoWin = new cls_Unpacking_info();
    //infoWin->show();

    return 0; // ok
}

/* Just a wrapper */
UInt_t cls_HLD_file::Import(QString p_qfilename, Bool_t p_doCalibration)
{
    char v_cfilename[255];
    strcpy(v_cfilename, p_qfilename.toStdString().c_str());
    TString v_filename(v_cfilename);
    return this->Import(v_filename, p_doCalibration);
}

UInt_t cls_HLD_file::ReadRawData(TString p_filename)
{
    std::ifstream file (p_filename.Data(), std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open()) {
        mSize = file.tellg();
        mRawData = new char [mSize];
        file.seekg (0, std::ios::beg);
        file.read (mRawData, mSize);
        file.close();
        cout << "The entire file " << p_filename << " is in memory" << endl;
        return 0; // ok
    } else {
        cerr << "Unable to open file";
        return 1; // fail
    }
}

void cls_HLD_file::Dump(UInt_t p_amount)
{
    UInt_t v_cursor;
    unsigned char* v_rawData = (unsigned char*)mRawData;
    cout << std::setfill('0');
    for (v_cursor=2; v_cursor<=p_amount; v_cursor+=2) {                 // to mSize
        printf ("%02X%02X ", v_rawData[v_cursor-2], v_rawData[v_cursor-1]);
        if (v_cursor % 32 == 0) printf ("\n");
    }
    printf ("\n");
}

void cls_HLD_file::SetRunDirectTDCanalysis(void)
{
    mRunDirectTDCanalysis = kTRUE;
    mDirectTDCanalyser = new cls_DirectTDCanalyser();
}

//TODO check
// seems to work...
void cls_HLD_file::SetCalibrator(cls_Calibrator* p_calibrator)
{
    mCalibrator = *p_calibrator;
}

UInt_t cls_HLD_file::Process(void)
{
    unsigned char* v_rawData = (unsigned char*)mRawData;

    UInt_t v_cursor=0;

    do {
        //UInt_t v_dataWord = DecodeBuf2(&v_rawData[v_cursor]);
        //printf ("%08x - EVENT #%d\n", v_dataWord, mRawEventsCounter );

        if (mRawEventsCounter % 1000 == 0) {
            cout << "Raw event #" << mRawEventsCounter << endl;
        }

        //FIXME - temporary debug hack
        //if (mRawEventsCounter == 89000) mVerbosityLevel = 2;
        //----------------------------

        mRawEventsCounter++;

        //TODO check!
        UInt_t v_eventSize = ((v_rawData[v_cursor+1] <<  8) & 0x0000ff00) |
                             ((v_rawData[v_cursor+0] <<  0) & 0x000000ff);

        this->ProcessEvent(&v_rawData[v_cursor], v_eventSize);
        v_cursor+=v_eventSize;

        //FIXME - cheating!
        // Padding
        if (v_cursor % 8 != 0) {
            v_cursor+=4;
        }

    }
    while(v_cursor < mSize); //1024 or mSize

    cout << "Unpacking finished succesfully" << endl;
    return 0;
}

void cls_HLD_file::ProcessEvent(unsigned char* p_data, UInt_t p_eventSize)
{
    if (mVerbosityLevel > 1) printf ("EVENT\n");

    UInt_t v_cursor=0;

    UInt_t v_eventSize = DecodeBuf2(&p_data[v_cursor]);
    if (mVerbosityLevel > 1) printf ("\t\t\t\teventSize=0x%08x\t", v_eventSize);
    v_cursor += 4;

    UInt_t v_decoding = DecodeBuf2(&p_data[v_cursor]);
    if (mVerbosityLevel > 1) printf ("decoding=0x%08x\t", v_decoding);
    v_cursor += 4;

    UInt_t v_id = DecodeBuf2(&p_data[v_cursor]);
    if (mVerbosityLevel > 1) printf ("id=0x%08x\t", v_id);
    v_cursor += 4;

    UInt_t v_seqNr = DecodeBuf2(&p_data[v_cursor]);
    if (mVerbosityLevel > 1) printf ("seqNr=0x%08x\n", v_seqNr);
    v_cursor += 4;

/*
    //TODO here date, time, runNr and expId are coded
    UInt_t v_sec, v_min, v_hour;
    UInt_t v_day, v_month, v_year;

    v_year = p_data[v_cursor];  v_cursor++;
    v_month = p_data[v_cursor]; v_cursor++;
    v_day = p_data[v_cursor];   v_cursor++; v_cursor++;

    v_hour = p_data[v_cursor];  v_cursor++;
    v_min = p_data[v_cursor];   v_cursor++;
    v_sec = p_data[v_cursor];   v_cursor++; v_cursor++;

    v_cursor += 2*4;

    printf ("%02d.%02d.%02d %02d:%02d:%02d\n", v_day, v_month, v_year, v_hour, v_min, v_sec);
*/

    v_cursor += 4*4;
    if (mVerbosityLevel > 1) printf ("\t\t\t\tdate=\ttime=\trunNr=\texpId=\n");

    if (v_cursor == p_eventSize) return;

    do {
        UInt_t v_subEventSize = DecodeBuf(&p_data[v_cursor]);
        this->ProcessSubEvent(&p_data[v_cursor], v_subEventSize);
        v_cursor+=v_subEventSize;
        //cout << "v_cursor=" << v_cursor << "\t\tp_eventSize=" << p_eventSize << endl;
        if (v_cursor % 8 != 0) {
            //cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
            v_cursor += 4;
        }
    }
    while(v_cursor < p_eventSize);

    if (mVerbosityLevel > 1) printf ("\n");
}

void cls_HLD_file::ProcessSubEvent(unsigned char* p_data, UInt_t p_subEventSize)
{
    if (mVerbosityLevel > 1) printf ("SUBEVENT\tp_subEventSize=%d\n", p_subEventSize);

    UInt_t v_cursor=0;

    UInt_t v_subEventSize = DecodeBuf(&p_data[v_cursor]);
    if (mVerbosityLevel > 1) printf ("\t\t\t\tsubEventSize=0x%08x\t", v_subEventSize);
    v_cursor += 4;

    UInt_t v_decoding = DecodeBuf(&p_data[v_cursor]);
    if (mVerbosityLevel > 1) printf ("decoding=0x%08x\t", v_decoding);
    v_cursor += 4;

    UInt_t v_id = DecodeBuf(&p_data[v_cursor]);
    if (mVerbosityLevel > 1) printf ("id=0x%08x\t", v_id);
    v_cursor += 4;

    UInt_t v_trigNr = DecodeBuf(&p_data[v_cursor]);
    if (mVerbosityLevel > 1) printf ("trigNr=0x%08x\n", v_trigNr);
    v_cursor += 4;

    //TODO implement
    // trigTypeTRB3

    if (v_cursor == p_subEventSize) return;

    do {
        UInt_t v_subSubEventHeader = DecodeBuf(&p_data[v_cursor]);
        UInt_t v_tdcNofWords = (v_subSubEventHeader >> 16) & 0xffff;
        this->ProcessSubSubEvent(&p_data[v_cursor], (1+v_tdcNofWords)*4);
        v_cursor+=(1+v_tdcNofWords)*4;
    }
    while(v_cursor < p_subEventSize);

    if (mVerbosityLevel > 1) printf ("\n");
}

void cls_HLD_file::ProcessSubSubEvent(unsigned char* p_data, UInt_t p_subSubEventSize)
{
    if (mVerbosityLevel > 1) printf ("SUBSUBEVENT\tp_subSubEventSize=%d\n", p_subSubEventSize);

    UInt_t v_cursor = 0;

    UInt_t tdcData = DecodeBuf(&p_data[v_cursor]);
    UInt_t tdcNofWords = (tdcData >> 16) & 0xffff;
    UInt_t subSubEventId = tdcData & 0xffff;

    if (mVerbosityLevel > 1) {
        printf ("0x%08x - SUBSUBEVENT HEADER\ttdcNofWords=%d\t\tsubSubEventId=0x%04x\n", tdcData, tdcNofWords, subSubEventId);
    }

    if (tdcNofWords > 0) {
        do {
            tdcData = DecodeBuf(&p_data[v_cursor]);
            subSubEventId = tdcData & 0xffff;

            UInt_t v_cursorAddend;

            if (subSubEventId == 0x7005 || subSubEventId == 0x5555 || subSubEventId == 0x0112 || subSubEventId == 0xc000) { // 0xc000 - only for lab tests
                v_cursorAddend = this->ProcessSSSeventSkip(&p_data[v_cursor]);
            } else {
                v_cursorAddend = this->ProcessSSSevent(&p_data[v_cursor]);
            }
            v_cursor += v_cursorAddend;
        }
        while(v_cursor < p_subSubEventSize);
    }

}

UInt_t cls_HLD_file::ProcessSSSevent(unsigned char* p_data)
{
    if (mVerbosityLevel > 1) printf ("SSSEVENT\n");

    UInt_t v_cursor=0;
    UInt_t v_headerAddned=1; // either 1 or 2 - cheating

    UInt_t tdcData = DecodeBuf(&p_data[v_cursor]);
    UInt_t tdcNofWords = (tdcData >> 16) & 0xffff;
    UInt_t subSubEventId = tdcData & 0xffff;
    if (mVerbosityLevel > 1) printf ("0x%08x - SUBSUBEVENT HEADER\ttdcNofWords=%d\t\tsubSubEventId=0x%04x\n", tdcData, tdcNofWords, subSubEventId);
    v_cursor += 4;
    if (subSubEventId == 0x7000 || subSubEventId == 0x7001 || subSubEventId == 0x7002 || subSubEventId == 0x7003)
    {
        tdcData = DecodeBuf(&p_data[v_cursor]);
        tdcNofWords = (tdcData >> 16) & 0xffff;
        subSubEventId = tdcData & 0xffff;
        if (mVerbosityLevel > 1) printf ("0x%08x - SUBSUBEVENT HEADER\ttdcNofWords=%d\t\tsubSubEventId=0x%04x\n", tdcData, tdcNofWords, subSubEventId);
        v_cursor += 4;
        v_headerAddned++;
    }
    UInt_t tdcId = subSubEventId;

    do {
        tdcData = DecodeBuf(&p_data[v_cursor]);
        //printf ("%08x\n", tdcData);
        v_cursor += 4;

        UInt_t tdcTimeDataMarker = (tdcData >> 31) & 0x1;
        if (tdcTimeDataMarker == 0x1)  // TIME DATA
        {
            this->ProcessTimeMessage(tdcData, tdcId, mCurEpochCounter);
        } else {
            UInt_t tdcMarker = (tdcData >> 29) & 0x7;       // 3 bits
            if (tdcMarker == 0x1) {                         // TDC header
                UInt_t randomCode = (tdcData >> 16) & 0xff; // 8 bits
                UInt_t errorBits = (tdcData) & 0xffff;      // 16 bits
                if (mVerbosityLevel > 1) printf("0x%08x - TDC HEADER randomCode:0x%02x, errorBits:0x%04x\n", tdcData, randomCode, errorBits);
            } else if (tdcMarker == 0x2) {                  // DEBUG
                UInt_t debugMode = (tdcData >> 24) & 0x1f;  // 5 bits
                UInt_t debugBits = (tdcData) & 0xffffff;    // 24 bits
                if (mVerbosityLevel > 1) printf("\t0x%08x - DEBUG debugMode:%i, debugBits:0x%06x\n", tdcData, debugMode, debugBits);
            } else if (tdcMarker == 0x3) {                  // EPOCH counter
                mCurEpochCounter = (tdcData) & 0xfffffff;    // 28 bits
                if (mVerbosityLevel > 1) printf("\t0x%08x - EPOCH COUNTER epochCounter:0x%07x\n", tdcData, mCurEpochCounter);
            } else {                                        // UNKNOWN
                if (mVerbosityLevel > 1) printf("\t0x%08x - UNKNOWN\n", tdcData);
            }
        }
    }
    while (v_cursor < (v_headerAddned+tdcNofWords)*4);

    return (v_headerAddned+tdcNofWords)*4;
}

UInt_t cls_HLD_file::ProcessSSSeventSkip(unsigned char* p_data)
{
    //if (mVerbosityLevel > 1) printf ("SSSEVENT\n");

    UInt_t v_cursor=0;

    UInt_t tdcData = DecodeBuf(&p_data[v_cursor]);
    UInt_t tdcNofWords = (tdcData >> 16) & 0xffff;
    UInt_t subSubEventId = tdcData & 0xffff;
    if (mVerbosityLevel > 1) printf ("0x%08x - SUBSUBEVENT HEADER\ttdcNofWords=%d\t\tsubSubEventId=0x%04x\n", tdcData, tdcNofWords, subSubEventId);
    v_cursor += 4;

    do {
        tdcData = DecodeBuf(&p_data[v_cursor]);
        if (mVerbosityLevel > 1) printf ("\t0x%08x - skipping\n", tdcData);
        v_cursor += 4;
    }
    while (v_cursor < (1+tdcNofWords)*4);

    return (1+tdcNofWords)*4;
}

// Return number of succesfully processed messages
UInt_t cls_HLD_file::ProcessTimeMessage(UInt_t tdcData, UInt_t p_tdcId, UInt_t currentEpoch)
{
    UInt_t channel = (tdcData >> 22) & 0x7f;
    UInt_t fine = (tdcData >> 12) & 0x3ff;
    UInt_t edge = (tdcData >> 11) & 0x1;
    UInt_t coarse = (tdcData) & 0x7ff;
    UInt_t epoch = currentEpoch;

    if (mVerbosityLevel > 1) printf("\t0x%08x - TIMESTAMP", tdcData);

    if (fine == 0x3ff) {
        if (mVerbosityLevel > 0) printf(" 0x3ff fine time counter value detected, skipping timestamp. tdc 0x%04x  ch %d\n", p_tdcId, channel);
        mUnpackingInfo->AddFaultyMessage(TDCidToInteger(p_tdcId), channel);
        return 0;
    }

    //TODO implement calibration and corrections
    //TODO check
    Double_t fullTime = mCalibrator.GetFullTime(p_tdcId, channel, epoch, coarse, fine);

    if (channel == 0) {
        if (mVerbosityLevel > 1) printf("   --- SYNC  - tdc 0x%04x ch %02d edge %d epoch %08x coarse %08x fine %08x = %f\n",
                                            p_tdcId, channel, edge, epoch, coarse, fine, fullTime);
    } else if (channel % 2 == 1) {
        if (mVerbosityLevel > 1) printf("   --- LEAD  - tdc 0x%04x ch %02d edge %d epoch %08x coarse %08x fine %08x = %f\n",
                                            p_tdcId, channel, edge, epoch, coarse, fine, fullTime);
        mCalibrator.AddFineTime(p_tdcId, channel, fine);
    } else {
        if (mVerbosityLevel > 1) printf("   --- TRAIL - tdc 0x%04x ch %02d edge %d epoch %08x coarse %08x fine %08x = %f\n",
                                            p_tdcId, channel, edge, epoch, coarse, fine, fullTime);
        mCalibrator.AddFineTime(p_tdcId, channel, fine);
    }
    UInt_t v_tdcUID = TDCidToInteger(p_tdcId);
    cls_RawMessage v_mess(v_tdcUID, channel, epoch, coarse, fine, fullTime);
    mEdgeMatcher->AddInputMessage(v_mess);
    mUnpackingInfo->AddMessage(v_mess);

    if (mRunDirectTDCanalysis) {
        mDirectTDCanalyser->AddMessage(v_mess);
    }

    return 1; // Processed succesfully 1 message
}

UInt_t cls_HLD_file::DecodeBuf(unsigned char* p_buf)
{
    return ((p_buf[0] << 24) & 0xff000000) |
           ((p_buf[1] << 16) & 0x00ff0000) |
           ((p_buf[2] <<  8) & 0x0000ff00) |
           ((p_buf[3] <<  0) & 0x000000ff);
}

UInt_t cls_HLD_file::DecodeBuf2(unsigned char* p_buf)
{
    return ((p_buf[3] << 24) & 0xff000000) |
           ((p_buf[2] << 16) & 0x00ff0000) |
           ((p_buf[1] <<  8) & 0x0000ff00) |
           ((p_buf[0] <<  0) & 0x000000ff);
}
