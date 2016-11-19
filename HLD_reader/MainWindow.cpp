#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <fstream>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <QFileDialog>

#include "HLD_file.h"
#include "Calibrator.h"
#include "global_defines.h"

cls_MainWindow::cls_MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::cls_MainWindow),
    mFile(nullptr),
    mCalibrator(nullptr)
{
    ui->setupUi(this);
}

cls_MainWindow::~cls_MainWindow()
{
    delete ui;

    delete mFile;       mFile = nullptr;
    delete mCalibrator; mCalibrator = nullptr;
}

void cls_MainWindow::SetConfigFile(QString p_filename)
{
    ui->leConfigFile->setText(p_filename);
}

void cls_MainWindow::SelectConfigFile(void)
{
    QFileDialog v_dial;
    v_dial.setFileMode(QFileDialog::ExistingFile);
    v_dial.setNameFilter(tr("Config files (*.cfg)"));
    v_dial.setDirectory("/home/evovch/DATA");

    QStringList v_fileNames;
    if (v_dial.exec()) {
        v_fileNames = v_dial.selectedFiles();
        ui->leConfigFile->setText(v_fileNames.at(0));
    }
}

UInt_t cls_MainWindow::ImportConfig(void)
{
    std::ifstream v_configFile(ui->leConfigFile->text().toStdString());

    if (v_configFile.is_open()) {

        std::string v_line;
        std::string v_delims("=");
        while (getline(v_configFile, v_line))
        {
            if (v_line.size() == 0) continue;
            if (v_line.substr(0, 1) == std::string("#")) continue;

            std::vector<std::string> v_parsedLine = tokenize(v_line, v_delims);

            if (v_parsedLine.size() < 2) continue;

            if (v_parsedLine[0] == std::string("HLDFILE")) {
                cout << "HLDFILE=" << v_parsedLine[1] << endl;
                ui->leHLDfile->setText(QString::fromStdString(v_parsedLine[1]));
            }
            if (v_parsedLine[0] == std::string("CALIBFILE")) {
                cout << "CALIBFILE=" << v_parsedLine[1] << endl;
                ui->leCalibFile->setText(QString::fromStdString(v_parsedLine[1]));
                ui->leHLDfileCalibFile->setText(QString::fromStdString(v_parsedLine[1]));
            }
            if (v_parsedLine[0] == std::string("UNPACKINFOFILE")) {
                cout << "UNPACKINFOFILE=" << v_parsedLine[1] << endl;
                ui->leUnpackInfoFile->setText(QString::fromStdString(v_parsedLine[1]));
            }
            if (v_parsedLine[0] == std::string("EDGEMATCHINFOFILE")) {
                cout << "EDGEMATCHINFOFILE=" << v_parsedLine[1] << endl;
                ui->leEdgeMatcherInfoFile->setText(QString::fromStdString(v_parsedLine[1]));
            }
            if (v_parsedLine[0] == std::string("MATCHEDEDGESFILE")) {
                cout << "MATCHEDEDGESFILE=" << v_parsedLine[1] << endl;
                ui->leMatchedEdgesFile->setText(QString::fromStdString(v_parsedLine[1]));
            }
            if (v_parsedLine[0] == std::string("EVBLDINFOFILE")) {
                cout << "EVBLDINFOFILE=" << v_parsedLine[1] << endl;
                ui->leEventBuildingInfoFile->setText(QString::fromStdString(v_parsedLine[1]));
            }
            if (v_parsedLine[0] == std::string("ANALYSISINFOFILE")) {
                cout << "ANALYSISINFOFILE=" << v_parsedLine[1] << endl;
                ui->leAnalysisInfoFile->setText(QString::fromStdString(v_parsedLine[1]));
            }
            if (v_parsedLine[0] == std::string("CORRECTIONSFILE")) {
                cout << "CORRECTIONSFILE=" << v_parsedLine[1] << endl;
                ui->leCorrectionsFile->setText(QString::fromStdString(v_parsedLine[1]));
            }
            if (v_parsedLine[0] == std::string("RINGSANALYSISINFOFILE")) {
                cout << "RINGSANALYSISINFOFILE=" << v_parsedLine[1] << endl;
                ui->leRingsAnalysisInfoFile->setText(QString::fromStdString(v_parsedLine[1]));
            }
        }

        this->PostprocessConfig();

        v_configFile.close();
        cout << "Successfully imported config file " << ui->leConfigFile->text().toStdString() << endl;
        return 0; // ok
    } else {
        cout << "Failed to read config file " << ui->leConfigFile->text().toStdString() << endl;
        return 1; // fail
    }

}

void cls_MainWindow::PostprocessConfig(void)
{
    if (ui->leHLDfile->text().size() > 0) {

        if (ui->leHLDfile->text().endsWith(".hld")) {

            QStringList v_tokens = ui->leHLDfile->text().split('/');

            QStringList::const_reverse_iterator v_token;
            v_token = v_tokens.rbegin();
            QString v_basename = (*v_token);

            if (ui->leCalibFile->text().endsWith("/")) {
                QString v_origtext = ui->leCalibFile->text();
                v_origtext += "calib_";
                v_origtext += v_basename;
                v_origtext += ".root";
                ui->leCalibFile->setText(v_origtext);
            }
            if (ui->leHLDfileCalibFile->text().endsWith("/")) {
                QString v_origtext = ui->leHLDfileCalibFile->text();
                v_origtext += "calib_";
                v_origtext += v_basename;
                v_origtext += ".root";
                ui->leHLDfileCalibFile->setText(v_origtext);
            }
            if (ui->leMatchedEdgesFile->text().endsWith("/")) {
                QString v_origtext = ui->leMatchedEdgesFile->text();
                v_origtext += "hits_";
                v_origtext += v_basename;
                v_origtext += ".hits";
                ui->leMatchedEdgesFile->setText(v_origtext);
            }
            if (ui->leUnpackInfoFile->text().endsWith("/")) {
                QString v_origtext = ui->leUnpackInfoFile->text();
                v_origtext += "unpack_info_";
                v_origtext += v_basename;
                v_origtext += ".root";
                ui->leUnpackInfoFile->setText(v_origtext);
            }
            if (ui->leEdgeMatcherInfoFile->text().endsWith("/")) {
                QString v_origtext = ui->leEdgeMatcherInfoFile->text();
                v_origtext += "edgematch_info_";
                v_origtext += v_basename;
                v_origtext += ".root";
                ui->leEdgeMatcherInfoFile->setText(v_origtext);
            }
            if (ui->leEventBuildingInfoFile->text().endsWith("/")) {
                QString v_origtext = ui->leEventBuildingInfoFile->text();
                v_origtext += "evbld_info_";
                v_origtext += v_basename;
                v_origtext += ".root";
                ui->leEventBuildingInfoFile->setText(v_origtext);
            }
            if (ui->leAnalysisInfoFile->text().endsWith("/")) {
                QString v_origtext = ui->leAnalysisInfoFile->text();
                v_origtext += "analysis_info_";
                v_origtext += v_basename;
                v_origtext += ".root";
                ui->leAnalysisInfoFile->setText(v_origtext);
            }
            if (ui->leCorrectionsFile->text().endsWith("/")) {
                QString v_origtext = ui->leCorrectionsFile->text();
                v_origtext += "corrections_";
                v_origtext += v_basename;
                v_origtext += ".txt";
                ui->leCorrectionsFile->setText(v_origtext);
            }
            if (ui->leRingsAnalysisInfoFile->text().endsWith("/")) {
                QString v_origtext = ui->leRingsAnalysisInfoFile->text();
                v_origtext += "analysis_info_";
                v_origtext += v_basename;
                v_origtext += ".rings.root";
                ui->leRingsAnalysisInfoFile->setText(v_origtext);
            }
        }

    }
}

UInt_t cls_MainWindow::ExportConfig(void)
{
    std::ofstream v_configFile(ui->leConfigFile->text().toStdString());

    if (v_configFile.is_open()) {

        v_configFile << "HLDFILE=" << ui->leHLDfile->text().toStdString() << endl;
        v_configFile << "CALIBFILE=" << ui->leCalibFile->text().toStdString() << endl;
        v_configFile << "MATCHEDEDGESFILE=" << ui->leMatchedEdgesFile->text().toStdString() << endl;
        v_configFile << "UNPACKINFOFILE=" << ui->leUnpackInfoFile->text().toStdString() << endl;
        v_configFile << "EDGEMATCHINFOFILE=" << ui->leEdgeMatcherInfoFile->text().toStdString() << endl;
        v_configFile << "EVBLDINFOFILE=" << ui->leEventBuildingInfoFile->text().toStdString() << endl;
        v_configFile << "ANALYSISINFOFILE=" << ui->leAnalysisInfoFile->text().toStdString() << endl;
        v_configFile << "CORRECTIONSFILE=" << ui->leCorrectionsFile->text().toStdString() << endl;
        v_configFile << "RINGSANALYSISINFOFILE=" << ui->leRingsAnalysisInfoFile->text().toStdString() << endl;

        cout << "Successfully exported config file " << ui->leConfigFile->text().toStdString() << endl;
        v_configFile.close();
        return 0; // ok
    } else {
        cout << "Failed to write config file " << ui->leConfigFile->text().toStdString() << endl;
        return 1; // fail
    }
}

void cls_MainWindow::GenFilenames(void)
{
    if (ui->leHLDfile->text().size() == 0) {
        cout << "Please, specify input file." << endl;
        return;
    }
    QStringList v_tokens = ui->leHLDfile->text().split('/');

    QStringList::const_reverse_iterator v_token;
    v_token = v_tokens.rbegin();
    QString v_basename = (*v_token);

    QString v_calibPath("/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/calibration_files/calib_");
    QString v_hitsPath("/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/hits_files/hits_");
    QString v_unpackingInfoPath("/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/analysis_results/unpack_info_");
    QString v_edgeMatchingInfoPath("/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/analysis_results/edgematch_info_");
    QString v_evbldInfoPath("/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/analysis_results/evbld_info_");
    QString v_analysisInfoPath("/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/analysis_results/analysis_info_");
    QString v_correctionsPath("/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/corrections/corrections_");
    QString v_ringsAnalysisInfoPath("/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/analysis_results/analysis_info_");

    if (v_basename.endsWith(".hld")) {

        v_calibPath += "sum";                  v_calibPath += ".root";
        v_hitsPath += v_basename;                   v_hitsPath += ".hits";
        v_unpackingInfoPath += v_basename;          v_unpackingInfoPath += ".root";
        v_edgeMatchingInfoPath += v_basename;       v_edgeMatchingInfoPath += ".root";
        v_evbldInfoPath += v_basename;              v_evbldInfoPath += ".root";
        v_analysisInfoPath += v_basename;           v_analysisInfoPath += ".root";
        v_correctionsPath += v_basename;            v_correctionsPath += ".txt";
        v_ringsAnalysisInfoPath += v_basename;      v_ringsAnalysisInfoPath += ".rings.root";

        ui->leHLDfileCalibFile->setText(v_calibPath);
        ui->leCalibFile->setText(v_calibPath);
        ui->leMatchedEdgesFile->setText(v_hitsPath);
        ui->leUnpackInfoFile->setText(v_unpackingInfoPath);
        ui->leEdgeMatcherInfoFile->setText(v_edgeMatchingInfoPath);
        ui->leEventBuildingInfoFile->setText(v_evbldInfoPath);
        ui->leAnalysisInfoFile->setText(v_analysisInfoPath);
        ui->leCorrectionsFile->setText(v_correctionsPath);
        ui->leRingsAnalysisInfoFile->setText(v_ringsAnalysisInfoPath);

    } else {
        cout << "Incorect file name. It has to end with '.hld'" << endl;
        return;
    }
}

void cls_MainWindow::ImportFile(void)
{
    if (ui->checkBox->isChecked()) {
        mFile = new cls_HLD_file();
        mFile->SetCalibrator(mCalibrator);
    } else {
        mFile = new cls_HLD_file();
    }

    ui->pbImportHLDfile->setEnabled(false);
    ui->pbExportHLDfileCalib->setEnabled(true);
    mFile->Import(ui->leHLDfile->text(), !(ui->checkBox->isChecked()));
}

void cls_MainWindow::RunDirectTDCanalysis(void)
{
    if (ui->checkBox->isChecked()) {
        mFile = new cls_HLD_file();
        mFile->SetCalibrator(mCalibrator);
    } else {
        mFile = new cls_HLD_file();
        mFile->SetPseudoCalibration();
    }

    ui->pbImportHLDfile->setEnabled(false);
    ui->pbExportHLDfileCalib->setEnabled(true);
    mFile->SetRunDirectTDCanalysis();
    mFile->Import(ui->leHLDfile->text(), !(ui->checkBox->isChecked()));
    mFile->RunDirectTDCpostAnalysis();
    mFile->ExportDirectTDCresults(ui->leAnalysisInfoFile->text());
    this->ExportUnpackInfo();
}

void cls_MainWindow::ExportCalibrationOfFile(void)
{
    mFile->ExportCalibration(ui->leHLDfileCalibFile->text());
}

void cls_MainWindow::ImportCalibration(void)
{
    mCalibrator = new cls_Calibrator();
    mCalibrator->Import(ui->leCalibFile->text());
    ui->checkBox->setEnabled(true);
    ui->checkBox->setChecked(true);
}

void cls_MainWindow::SetNoCalibration(void)
{
    mCalibrator = new cls_Calibrator();
    mCalibrator->SetNoCalibration();
    ui->checkBox->setEnabled(true);
    ui->checkBox->setChecked(true);
}

/*void cls_MainWindow::ResetCalibration(void)
{
    mCalibrator->Reset();
}*/

void cls_MainWindow::RunEdgeMatcher(void)
{
    if (!mFile){
        cout << "Aborting mFile is NULL." << endl;
        return;
    }

    mFile->RunEdgeMatcher();
}

void cls_MainWindow::ExportMatchedEdges(void)
{
    mFile->ExportMatchedEdges(ui->leMatchedEdgesFile->text());
}

void cls_MainWindow::ImportMatchedEdges(void)
{
    mFile = new cls_HLD_file();
    mFile->ImportMatchedEdges(ui->leMatchedEdgesFile->text());
}

void cls_MainWindow::RunEventBuilder(void)
{
    // Beamtime
    if (ui->rbCh1->isChecked()) mFile->SetTrigger(kCh1disc);
    if (ui->rbCh2->isChecked()) mFile->SetTrigger(kCh2disc);
    if (ui->rbLaser->isChecked()) mFile->SetTrigger(kLaser);
    if (ui->rbLED->isChecked()) mFile->SetTrigger(kLED);
    if (ui->rbHodo->isChecked()) mFile->SetTrigger(kHodoCoinc);

    // Lab
    if (ui->rbTDC10sync->isChecked()) mFile->SetTrigger(kTDC0010sync);
    if (ui->rbTDC11sync->isChecked()) mFile->SetTrigger(kTDC0011sync);
    if (ui->rbTDC12sync->isChecked()) mFile->SetTrigger(kTDC0012sync);
    if (ui->rbTDC13sync->isChecked()) mFile->SetTrigger(kTDC0013sync);

    if (ui->rbChannel->isChecked()) mFile->SetTrigger((enu_triggerType)(ui->leChannelNum->text().toInt()));

    mFile->RunEventBuilder();
}

void cls_MainWindow::RunLaserAnalyser(void)
{
    mFile->RunAnalyser();
}

void cls_MainWindow::RunRingsAnalyser(void)
{
    mFile->RunRingsAnalyser();
}

void cls_MainWindow::ExportUnpackInfo(void)
{
    mFile->ExportUnpackInfo(ui->leUnpackInfoFile->text());
}

void cls_MainWindow::ExportEdgeMatcherInfo(void)
{
    mFile->ExportEdgeMatcherInfo(ui->leEdgeMatcherInfoFile->text());
}

void cls_MainWindow::ExportEventBuildingInfo(void)
{
    mFile->ExportEventBuildingInfo(ui->leEventBuildingInfoFile->text());
}

void cls_MainWindow::ExportAnalysisInfo(void)
{
    mFile->ExportAnalysisInfo(ui->leAnalysisInfoFile->text());
}

void cls_MainWindow::ExportCorrections(void)
{
    mFile->ExportCorrections(ui->leCorrectionsFile->text());
}

void cls_MainWindow::ImportCorrections(void)
{
    mCalibrator->ImportCorrections(ui->leCorrectionsFile->text());
}

void cls_MainWindow::ExportRingsAnalysisInfo(void)
{
    mFile->ExportRingsAnalysisInfo(ui->leRingsAnalysisInfoFile->text());
}

// ------------------------------------------------------------------------------------------------------------------------

void cls_MainWindow::BatchDirectTDCnocalib(void)
{
    this->ImportConfig();
    //this->GenFilenames();

    this->RunDirectTDCanalysis();
    mFile->FitAllCalibration();
    this->ExportCalibrationOfFile();

    cout << "FINISHED BATCH" << endl;
}

void cls_MainWindow::BatchDirectTDCcalib(void)
{
    this->ImportConfig();
    //this->GenFilenames();

    this->ImportCalibration();
    this->RunDirectTDCanalysis();

    cout << "FINISHED BATCH" << endl;
}

void cls_MainWindow::BatchDirectTDCindividPseudocalib(void)
{
    this->ImportConfig();
    //this->GenFilenames();

    this->ImportCalibration();
    mCalibrator->FitAll();
    mCalibrator->SetUseFittedCalibration();
    this->RunDirectTDCanalysis();

    cout << "FINISHED BATCH" << endl;
}

//TODO implement
void cls_MainWindow::BatchDirectTDCglobalPseudocalib(void)
{
    this->ImportConfig();
    //this->GenFilenames();

    this->SetNoCalibration();
    mCalibrator->SetUseFittedCalibration();
    this->RunDirectTDCanalysis();

    cout << "FINISHED BATCH" << endl;
}

// ------------------------------------------------------------------------------------------------------------------------

void cls_MainWindow::BatchGenCalib(void)
{
    this->ImportConfig();
    //this->GenFilenames();
    this->ImportFile();
    this->ExportCalibrationOfFile();
    cout << "FINISHED BATCH" << endl;
}

// LAB -----------------------------------------------------

void cls_MainWindow::BatchLabAnalysisCalibNoCorr(void)
{
    ui->rbTDC10sync->setChecked(true);

    this->ImportConfig();
    this->GenFilenames();
    this->ImportCalibration();

    this->ImportFile();
    this->ExportUnpackInfo();
    this->RunEdgeMatcher();
    this->ExportMatchedEdges();
    this->ExportEdgeMatcherInfo();
    this->RunEventBuilder();
    this->ExportEventBuildingInfo();

    //TODO
    this->RunLaserAnalyser();
    this->ExportAnalysisInfo();
    this->ExportCorrections();
    cout << "FINISHED BATCH" << endl;
}

// LASER ---------------------------------------------------

void cls_MainWindow::BatchLaserAnalysisNoCalibNoCorr(void)
{
    ui->rbLaser->setChecked(true);

    this->ImportConfig();
    this->GenFilenames();
    this->SetNoCalibration();

    this->ImportFile();
    this->ExportUnpackInfo();
    this->RunEdgeMatcher();
    this->ExportMatchedEdges();
    this->ExportEdgeMatcherInfo();
    this->RunEventBuilder();
    this->ExportEventBuildingInfo();
    this->RunLaserAnalyser();
    this->ExportAnalysisInfo();
    this->ExportCorrections();
    cout << "FINISHED BATCH" << endl;
}

void cls_MainWindow::BatchLaserAnalysisCalibNoCorr(void)
{
    ui->rbLaser->setChecked(true);

    this->ImportConfig();
    this->GenFilenames();
    this->ImportCalibration();

    this->ImportFile();
    this->ExportUnpackInfo();
    this->RunEdgeMatcher();
    this->ExportMatchedEdges();
    this->ExportEdgeMatcherInfo();
    this->RunEventBuilder();
    this->ExportEventBuildingInfo();
    this->RunLaserAnalyser();
    this->ExportAnalysisInfo();
    this->ExportCorrections();
    cout << "FINISHED BATCH" << endl;
}

void cls_MainWindow::BatchLaserAnalysisNoCalibCorr(void)
{
    ui->rbLaser->setChecked(true);

    this->ImportConfig();
    this->GenFilenames();
    this->SetNoCalibration();

    QString v_oldCorrTableFile(ui->leCorrectionsFile->text());
    ui->leCorrectionsFile->setText("/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/correction_to_use/corrections.txt");
    this->ImportCorrections();
    ui->leCorrectionsFile->setText(v_oldCorrTableFile);

    this->ImportFile();
    this->ExportUnpackInfo();
    this->RunEdgeMatcher();
    this->ExportMatchedEdges();
    this->ExportEdgeMatcherInfo();
    this->RunEventBuilder();
    this->ExportEventBuildingInfo();
    this->RunLaserAnalyser();
    this->ExportAnalysisInfo();
    this->ExportCorrections();
    cout << "FINISHED BATCH" << endl;
}

void cls_MainWindow::BatchLaserAnalysisCalibCorr(void)
{
    ui->rbLaser->setChecked(true);

    this->ImportConfig();
    this->GenFilenames();
    this->ImportCalibration();

    QString v_oldCorrTableFile(ui->leCorrectionsFile->text());
    ui->leCorrectionsFile->setText("/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/correction_to_use/corrections.txt");
    this->ImportCorrections();
    ui->leCorrectionsFile->setText(v_oldCorrTableFile);

    this->ImportFile();
    this->ExportUnpackInfo();
    this->RunEdgeMatcher();
    this->ExportMatchedEdges();
    this->ExportEdgeMatcherInfo();
    this->RunEventBuilder();
    this->ExportEventBuildingInfo();
    this->RunLaserAnalyser();
    this->ExportAnalysisInfo();
    this->ExportCorrections();
    cout << "FINISHED BATCH" << endl;
}

// RINGS ---------------------------------------------------

void cls_MainWindow::BatchRingsAnalysisNoCalibNoCorr(void)
{
    this->ImportConfig();
    this->GenFilenames();
    this->SetNoCalibration();

    this->ImportMatchedEdges();

    ui->rbHodo->setChecked(true);

    this->RunEventBuilder();
    this->ExportEventBuildingInfo();
    this->RunRingsAnalyser();
    this->ExportRingsAnalysisInfo();
    cout << "FINISHED BATCH" << endl;
}

void cls_MainWindow::BatchRingsAnalysisCalibNoCorr(void)
{
    this->ImportConfig();
    this->GenFilenames();
    this->ImportCalibration();

    this->ImportMatchedEdges();

    ui->rbHodo->setChecked(true);

    this->RunEventBuilder();
    this->ExportEventBuildingInfo();
    this->RunRingsAnalyser();
    this->ExportRingsAnalysisInfo();
    cout << "FINISHED BATCH" << endl;
}

void cls_MainWindow::BatchRingsAnalysisNoCalibCorr(void)
{
    this->ImportConfig();
    this->GenFilenames();
    this->SetNoCalibration();

    QString v_oldCorrTableFile(ui->leCorrectionsFile->text());
    ui->leCorrectionsFile->setText("/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/correction_to_use/corrections.txt");
    this->ImportCorrections();
    ui->leCorrectionsFile->setText(v_oldCorrTableFile);

    this->ImportMatchedEdges();

    ui->rbHodo->setChecked(true);

    this->RunEventBuilder();
    this->ExportEventBuildingInfo();
    this->RunRingsAnalyser();
    this->ExportRingsAnalysisInfo();
    cout << "FINISHED BATCH" << endl;
}

void cls_MainWindow::BatchRingsAnalysisCalibCorr(void)
{
    this->ImportConfig();
    this->GenFilenames();
    this->ImportCalibration();

    QString v_oldCorrTableFile(ui->leCorrectionsFile->text());
    ui->leCorrectionsFile->setText("/home/evovch/Documents/Analysis_Sep2016/time_precision_evolution/correction_to_use/corrections.txt");
    this->ImportCorrections();
    ui->leCorrectionsFile->setText(v_oldCorrTableFile);

    this->ImportMatchedEdges();

    ui->rbHodo->setChecked(true);

    this->RunEventBuilder();
    this->ExportEventBuildingInfo();
    this->RunRingsAnalyser();
    this->ExportRingsAnalysisInfo();
    cout << "FINISHED BATCH" << endl;
}
