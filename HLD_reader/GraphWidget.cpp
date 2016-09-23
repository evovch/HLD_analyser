#include "GraphWidget.h"
#include "ui_GraphWidget.h"

#include <QByteArray>

#include <TStyle.h>
#include <TPad.h>
#include <TGraph.h>
#include <TRandom3.h>
#include <TAxis.h>

#include <iostream>
using std::cout;
using std::endl;

UInt_t cls_GraphWidget::mGraphID = 0;

cls_GraphWidget::cls_GraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cls_GraphWidget),
    mGraph(new TGraph(100)),
    mDrawOptions("")
{
    ui->setupUi(this);

    mGraphID++;

    char opts[32];
    QByteArray ba = mDrawOptions.toLocal8Bit();
    strcpy(opts, ba.constData());

    ui->canvasWidget->cd();
    gStyle->SetOptStat(1111111);
    gPad->SetGrid(1,1);
    mGraph->Draw(opts);
}

cls_GraphWidget::cls_GraphWidget(char* p_name, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cls_GraphWidget),
    mGraph(new TGraph(100)),
    mDrawOptions("")
{
    ui->setupUi(this);

    mGraphID++;

    char opts[32];
    QByteArray ba = mDrawOptions.toLocal8Bit();
    strcpy(opts, ba.constData());

    mGraph->SetTitle(p_name);

    ui->canvasWidget->cd();
    gStyle->SetOptStat(1111111);
    gPad->SetGrid(1,1);
    mGraph->Draw(opts);
}

cls_GraphWidget::~cls_GraphWidget()
{
    delete ui;
}

void cls_GraphWidget::FillTest(void)
{
    Int_t v_numOfPoints = mGraph->GetN();
    TRandom3 v_randomGenerator;
    v_randomGenerator.SetSeed();

    for (Int_t i=0; i<v_numOfPoints; i++) {
        mGraph->SetPoint(i, i, v_randomGenerator.Rndm());
    }

    ui->canvasWidget->cd();
    ui->canvasWidget->Modified();
    ui->canvasWidget->Update();
}

void cls_GraphWidget::FillFromData(std::vector<Double_t> p_data)
{
    mGraph->Set(p_data.size());
    for (UInt_t i=0; i<p_data.size(); i++) {
        //cout << i << "\t\t" << p_data[i] << endl;
        mGraph->SetPoint(i, (Double_t)(i), p_data[i]);
    }

    char opts[32];
    QByteArray ba = mDrawOptions.toLocal8Bit();
    strcpy(opts, ba.constData());

    ui->canvasWidget->cd();
    mGraph->Draw(opts);
    ui->canvasWidget->Modified();
    ui->canvasWidget->Update();
}

void cls_GraphWidget::FillFromData(std::vector<cls_RawMessage> p_data)
{
    mGraph->Set(p_data.size());
    for (UInt_t i=0; i<p_data.size(); i++) {
        //cout << i << "\t\t" << p_data[i].mFullTime << endl;
        mGraph->SetPoint(i, (Double_t)(i), p_data[i].mFullTime);
    }

    char opts[32];
    QByteArray ba = mDrawOptions.toLocal8Bit();
    strcpy(opts, ba.constData());

    ui->canvasWidget->cd();
    mGraph->Draw(opts);
    ui->canvasWidget->Modified();
    ui->canvasWidget->Update();
}

void cls_GraphWidget::SetLogY(Int_t p_val)
{
    ui->canvasWidget->cd();
    gPad->SetLogy(p_val);
    ui->canvasWidget->Modified();
    ui->canvasWidget->Update();
}

void cls_GraphWidget::ToggleLogY(void)
{
    ui->canvasWidget->cd();
    gPad->GetLogy()==0 ? gPad->SetLogy(1) : gPad->SetLogy(0);
    ui->canvasWidget->Modified();
    ui->canvasWidget->Update();
}

void cls_GraphWidget::UnZoom(void)
{
    ui->canvasWidget->cd();
    mGraph->GetXaxis()->UnZoom();
    ui->canvasWidget->Modified();
    ui->canvasWidget->Update();
}
