#ifndef HLD_FILE_H
#define HLD_FILE_H

#include <TString.h>

#include <QString>

#include "Calibrator.h"
#include "EdgeMatcher.h"
#include "EventsAnalyser.h"
#include "Unpacking_info.h"
#include "RingsAnalyser.h"
#include "DirectTDCanalyser.h"

class cls_HLD_file
{
public:
    cls_HLD_file();
    ~cls_HLD_file();

    UInt_t Import(TString p_filename, Bool_t p_doCalibration);
    UInt_t Import(QString p_qfilename, Bool_t p_doCalibration);
    UInt_t ReadRawData(TString p_filename);
    void Dump(UInt_t p_amount = 512);
    UInt_t Process(void);

    void SetRunDirectTDCanalysis(void);
    void SetCalibrator(cls_Calibrator* p_calibrator);
    void SetTrigger(enu_triggerType p_type) { mEdgeMatcher->SetTrigger(p_type); }

    void RunDirectTDCpostAnalysis(void) { mDirectTDCanalyser->RunPostAnalysis(); }
    void RunEdgeMatcher(void) { mEdgeMatcher->Process(); }
    void RunEventBuilder(void) { mEdgeMatcher->BuildEvents(); }
    void RunAnalyser(void) { mAnalyser->RunAnalysis(); }
    void RunRingsAnalyser(void) { mRingsAnalyser->RunAnalysis(); }

    void FitAllCalibration() { mCalibrator.FitAll(); }
    void ExportCalibration(QString p_filename) { mCalibrator.Export(p_filename); }
    void ImportCalibration(QString p_filename) { mCalibrator.Import(p_filename); }
    //void ResetCalibration() { mCalibrator.Reset(); }
    void SetPseudoCalibration(void) { mCalibrator.SetNoCalibration(); }
    void ExportMatchedEdges(QString p_filename) { mEdgeMatcher->ExportMatchedEdges(p_filename); }
    void ImportMatchedEdges(QString p_filename) { mEdgeMatcher->ImportMatchedEdges(p_filename); }

    void ExportUnpackInfo(QString p_filename) { mUnpackingInfo->Export(p_filename); }
    void ExportEdgeMatcherInfo(QString p_filename) { mEdgeMatcher->ExportHistos(p_filename); }
    void ExportEventBuildingInfo(QString p_filename) { mEdgeMatcher->ExportEventBuildingHistos(p_filename); }
    void ExportAnalysisInfo(QString p_filename) { mAnalyser->ExportHistos(p_filename); }
    void ExportCorrections(QString p_filename) { mAnalyser->ExportCorrections(p_filename); }
    void ImportCorrections(QString p_filename) { mCalibrator.ImportCorrections(p_filename); }
    void ExportRingsAnalysisInfo(QString p_filename) { mRingsAnalyser->ExportHistos(p_filename); }
    void ExportDirectTDCresults(QString p_filename) { mDirectTDCanalyser->ExportHistos(p_filename); }

private:
    void ProcessEvent(unsigned char* p_data, UInt_t p_eventSize);
    void ProcessSubEvent(unsigned char* p_data, UInt_t p_subEventSize);
    void ProcessSubSubEvent(unsigned char* p_data, UInt_t p_subSubEventSize);
    UInt_t ProcessSSSevent(unsigned char* p_data);
    UInt_t ProcessSSSeventSkip(unsigned char* p_data);
    UInt_t ProcessTimeMessage(UInt_t tdcData, UInt_t p_tdcId, UInt_t currentEpoch);

    UInt_t DecodeBuf(unsigned char* p_buf);
    UInt_t DecodeBuf2(unsigned char* p_buf);

private:
    char* mRawData;
    std::streampos mSize;

    UInt_t mCurEpochCounter;

    cls_Calibrator mCalibrator;
    cls_EdgeMatcher* mEdgeMatcher;
    cls_EventsAnalyser* mAnalyser;
    cls_Unpacking_info* mUnpackingInfo;
    cls_RingsAnalyser* mRingsAnalyser;
    cls_DirectTDCanalyser* mDirectTDCanalyser;

    UInt_t mRawEventsCounter;

    Bool_t mRunDirectTDCanalysis;

    static UInt_t mVerbosityLevel;
};

#endif // HLD_FILE_H
