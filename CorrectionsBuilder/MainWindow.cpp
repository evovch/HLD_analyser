#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <TFile.h>
#include <TDirectory.h>
#include <TString.h>
#include <TH1D.h>

#include <fstream>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

cls_MainWindow::cls_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cls_MainWindow)
{
    ui->setupUi(this);

    /* The effect of this line is AMAZING */
    TH1::AddDirectory(kFALSE);

    for (UInt_t i=0; i<1024; i++) fCorrections[i] = 0.;
}

cls_MainWindow::~cls_MainWindow()
{
    delete ui;
}

void cls_MainWindow::SetInputFilename(QString p_filename)
{
    ui->lineEdit->setText(p_filename);
}

void cls_MainWindow::SetOutputFilename(QString p_filename)
{
    ui->lineEdit_2->setText(p_filename);
}

UInt_t cls_MainWindow::ImportAnalysisRes(void)
{
    TString v_filename(ui->lineEdit->text().toStdString());

    TDirectory* prevDir = gDirectory;
    TFile* prevFile = gFile;

    TFile v_inputFile(v_filename, "READ");

    if (v_inputFile.IsZombie()) {
        cerr << "Error opening file " << v_filename << endl;
        gDirectory = prevDir;
        gFile = prevFile;
        return 1; // FAIL
    }

    // Loop over all histos
    /*
    for (UInt_t v_pixel1=0; v_pixel1<256-1; v_pixel1++) {
        TString v_dirName;
        v_dirName.Form("LeadingEdgeDiff_ID1_%02d", v_pixel1);

        for (UInt_t v_pixel2=v_pixel1+1; v_pixel2<256; v_pixel2++) {

            TString v_histoName;
            v_histoName.Form("LeadingEdgeDiff_ID1_%02d_ID2_%02d", v_pixel1, v_pixel2);
            TString v_objName;
            v_objName.Form("%s/%s", v_dirName.Data(), v_histoName.Data());
            TH1D* v_curHisto = (TH1D*)v_inputFile.Get(v_objName);

            cout << v_objName.Data() << ":\t" << v_curHisto->GetEntries() << endl;
        }
    }
    */

    // Loop over only the first set of hitograms
    UInt_t v_pixel1=0;
    TString v_dirName;
    v_dirName.Form("LeadingEdgeDiff_ID1_%02d", v_pixel1);

    fCorrections[0] = 0.; // overwrite to be on the safe side

    for (UInt_t v_pixel2=v_pixel1+1; v_pixel2<256; v_pixel2++)
    {
        TString v_histoName;
        v_histoName.Form("fhLeadingEdgeDiff_ID1_%02d_ID2_%02d", v_pixel1, v_pixel2);
        TString v_objName;
        v_objName.Form("%s/%s", v_dirName.Data(), v_histoName.Data());
        TH1D* v_curHisto = (TH1D*)v_inputFile.Get(v_objName);

        if (v_pixel2 < 128) fCorrections[v_pixel2] = v_curHisto->GetMean();
        else if (v_pixel2 >= 128) fCorrections[v_pixel2+128] = v_curHisto->GetMean();
    }


    v_inputFile.Close();

    cout << "Successfully imported input analysis results from " << v_filename << "." << endl;

    gDirectory = prevDir;
    gFile = prevFile;
    return 0; // OK
}

UInt_t cls_MainWindow::ExportCorrections(void)
{
    std::ofstream v_corrTableFile;
    v_corrTableFile.open(ui->lineEdit_2->text().toStdString());
    if (v_corrTableFile.is_open())
    {
        for (UInt_t i=0; i<1024; i++) {
            v_corrTableFile << std::fixed << fCorrections[i] << endl;
        }

        v_corrTableFile.close();
        cout << "Successfully exported correction tables into " << ui->lineEdit_2->text().toStdString() << endl;
        return 0; // ok
    } else {
        cout << "Failed to open " << ui->lineEdit_2->text().toStdString() << endl;
        return 1; // fail
    }
}
