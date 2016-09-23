#ifndef UNPACKING_INFO_H
#define UNPACKING_INFO_H

#include <QMainWindow>
#include <QString>

#include <TString.h>

class TH1D;
class cls_RawMessage;

namespace Ui {
class cls_Unpacking_info;
}

class cls_Unpacking_info : public QMainWindow
{
    Q_OBJECT

public:
    explicit cls_Unpacking_info(QWidget *parent = 0);
    ~cls_Unpacking_info();

    void AddMessage(cls_RawMessage p_mess);
    void AddFaultyMessage(UInt_t p_tdcUid, UInt_t p_channel);

    UInt_t Export(TString p_filename);
    UInt_t Export(QString p_qfilename);

    TH1D* mhEdgesPerTDC;
    TH1D* mhLaserPeriodL;
    TH1D* mhLaserPeriodT;

    TH1D* mGoodMessagesPerChannel;
    TH1D* mFaultyMessagesPerChannel;

    TH1D* mCounterPerChannel;

private:
    Ui::cls_Unpacking_info *ui;

    void ProcessBeamDetectorMessage(cls_RawMessage p_mess);

    Double_t mLastLaserTimestampL;
    Bool_t mLastLaserSetL;

    Double_t mLastLaserTimestampT;
    Bool_t mLastLaserSetT;
};

#endif // UNPACKING_INFO_H
