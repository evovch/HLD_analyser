#include "HistoWidget.h"
#include "ui_HistoWidget.h"

#include <TStyle.h>
#include <TPad.h>
#include <TH1D.h>
#include <TRandom3.h>

#include <iostream>
using std::cout;
using std::endl;

UInt_t cls_HistoWidget::mHistoID = 0;

cls_HistoWidget::cls_HistoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cls_HistoWidget),
    mHisto(new TH1D())
{
    ui->setupUi(this);

    // Here we create a default histo with some parameters
    TString v_histoName;
    TString v_histoTitle;
    v_histoName.Form("mHisto_%d", mHistoID);
    v_histoTitle.Form("mHisto_%d", mHistoID);
    mHistoID++;
    std::unique_ptr<TH1D> v_tmpHisto(new TH1D(v_histoName.Data(), v_histoTitle.Data(), 100, 0., 100.));
    mHisto = std::move(v_tmpHisto);

    ui->canvasWidget->cd();
    gStyle->SetOptStat(1111111);
    gPad->SetGrid(1,1);
    mHisto->Draw();
}

cls_HistoWidget::~cls_HistoWidget()
{
    delete ui;
}

void cls_HistoWidget::FillTest(void)
{
    Int_t v_numOfBins = mHisto->GetNbinsX();
    TRandom3 v_randomGenerator;
    v_randomGenerator.SetSeed();
    mHisto->Reset();
    for (Int_t i=0; i<v_numOfBins; i++) {
        mHisto->SetBinContent(i, v_randomGenerator.Rndm());
    }

    ui->canvasWidget->cd();
    ui->canvasWidget->Modified();
    ui->canvasWidget->Update();
}

void cls_HistoWidget::FillFromData(std::vector<Double_t> p_data)
{
    // Maximum element in the data
    Double_t v_maxElem = *std::max_element(p_data.begin(), p_data.end());
    // Minimum element in the data
    Double_t v_minElem = *std::min_element(p_data.begin(), p_data.end());

    Double_t v_diff = v_maxElem - v_minElem;

    TString v_histoName = mHisto->GetName();
    TString v_histoTitle = mHisto->GetTitle();
    std::unique_ptr<TH1D> v_tmpHisto(new TH1D(v_histoName.Data(), v_histoTitle.Data(), (UInt_t)(v_diff+2.), v_minElem - 1., v_maxElem + 1.));
    mHisto = std::move(v_tmpHisto);

    mHisto->Reset();
    for (UInt_t i=0; i<p_data.size(); i++) {
        mHisto->Fill(p_data[i]);
    }

    ui->canvasWidget->cd();
    mHisto->Draw();
    ui->canvasWidget->Modified();
    ui->canvasWidget->Update();
}

void cls_HistoWidget::SetLogY(Int_t p_val)
{
    ui->canvasWidget->cd();
    gPad->SetLogy(p_val);
    ui->canvasWidget->Modified();
    ui->canvasWidget->Update();
}

void cls_HistoWidget::ToggleLogY(void)
{
    ui->canvasWidget->cd();
    gPad->GetLogy()==0 ? gPad->SetLogy(1) : gPad->SetLogy(0);
    ui->canvasWidget->Modified();
    ui->canvasWidget->Update();
}

void cls_HistoWidget::UnZoom(void)
{
    ui->canvasWidget->cd();
    mHisto->GetXaxis()->UnZoom();
    ui->canvasWidget->Modified();
    ui->canvasWidget->Update();
}

void cls_HistoWidget::RebinBy2(void)
{
    ui->canvasWidget->cd();
    mHisto->Rebin(2);
    ui->canvasWidget->Modified();
    ui->canvasWidget->Update();
}
