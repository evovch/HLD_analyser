#include "RingVisu.h"
#include "ui_RingVisu.h"

#include <vector>
#include <fstream>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <TMarker.h>
#include <TStyle.h>

#include "Hit.h"
#include "global_defines.h"

std::map<UInt_t, cls_PixelMapEntry> cls_RingVisu::fPixelMap = cls_RingVisu::ImportPixelMap();

const Double_t cls_RingVisu::fWidth = 220.;
const Double_t cls_RingVisu::fHeight = 220.;

cls_RingVisu::cls_RingVisu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cls_RingVisu)
{
    ui->setupUi(this);
}

cls_RingVisu::cls_RingVisu(cls_Event& p_event, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cls_RingVisu)
{
    ui->setupUi(this);

    // Get hits of the current event
    std::vector<cls_Hit>& v_eventHits = p_event.mHits;

    // Total number of hits in the event
    //UInt_t v_numHits = v_eventHits.size();

    // Hit ID
    UInt_t v_ID=0;

    ui->canvWidget->cd();

    gStyle->SetMarkerColor(kBlack);
    gStyle->SetMarkerSize(2);

    // Loop over the hits of the events
    for (std::vector<cls_Hit>::iterator v_iterHits=v_eventHits.begin(); v_iterHits!=v_eventHits.end(); ++v_iterHits) {

        // Get the current hit and its parameters from the pixel map
        cls_Hit& curHit = (*v_iterHits);
        UInt_t v_pixelUID = PixelUID(curHit.GetTDC(), curHit.GetLch());
        Double_t v_hitX = fPixelMap.at(v_pixelUID).mX;
        Double_t v_hitY = fPixelMap.at(v_pixelUID).mY;

        //cout << v_ID << ":\tX=" << v_hitX << "\t\tY=" << v_hitY << endl;

        TMarker* mark = new TMarker(v_hitX/fWidth, v_hitY/fHeight, 20);
        mark->SetUniqueID(v_ID);

        mark->Draw();

        v_ID++;
    }

    // delete markers?

}

cls_RingVisu::~cls_RingVisu()
{
    delete ui;
}

std::map<UInt_t, cls_PixelMapEntry> cls_RingVisu::ImportPixelMap(QString p_filename)
{
    std::map<UInt_t, cls_PixelMapEntry> v_pixelMap;

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
            v_pixelMap.insert(std::pair<UInt_t, cls_PixelMapEntry>(v_pixelUID, v_obj));
        }

        v_mapFile.close();
        cout << "Successfully read pixel map file " << p_filename.toStdString() << endl;
        return v_pixelMap;
    } else {
        cout << "Failed to read pixel map file " << p_filename.toStdString() << endl;
        return v_pixelMap;
    }

}
