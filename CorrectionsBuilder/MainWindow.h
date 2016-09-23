#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Rtypes.h"

namespace Ui {
class cls_MainWindow;
}

class cls_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit cls_MainWindow(QWidget *parent = 0);
    ~cls_MainWindow();

    void SetInputFilename(QString p_filename);
    void SetOutputFilename(QString p_filename);

public slots:
    UInt_t ImportAnalysisRes(void);
    UInt_t ExportCorrections(void);

private:
    Ui::cls_MainWindow *ui;

    Double_t fCorrections[1024];
};

#endif // MAINWINDOW_H
