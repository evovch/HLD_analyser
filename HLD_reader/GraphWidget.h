#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>

#include <Rtypes.h>
#include <memory>
#include "RawMessage.h"
class TGraph;

namespace Ui {
class cls_GraphWidget;
}

class cls_GraphWidget : public QWidget
{
    Q_OBJECT

public:
    explicit cls_GraphWidget(QWidget *parent = 0);
    cls_GraphWidget(char* p_name, QWidget *parent = 0);
    ~cls_GraphWidget();

    void FillFromData(std::vector<Double_t> p_data);
    void FillFromData(std::vector<cls_RawMessage> p_data);

    void SetDrawOptions(const char* p_opt) { mDrawOptions = QString(p_opt); }

public slots:
    void FillTest(void);
    void SetLogY(Int_t p_val = 1);
    void ToggleLogY(void);
    void UnZoom(void);


private:
    Ui::cls_GraphWidget *ui;

    std::unique_ptr<TGraph> mGraph;

    QString mDrawOptions;

    static UInt_t mGraphID;
};

#endif // GRAPHWIDGET_H
