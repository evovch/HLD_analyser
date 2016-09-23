#ifndef RINGVISU_H
#define RINGVISU_H

#include <map>

#include <QWidget>

#include "Event.h"
#include "PixelMapEntry.h"

namespace Ui {
class cls_RingVisu;
}

class cls_RingVisu : public QWidget
{
    Q_OBJECT

public:
    explicit cls_RingVisu(QWidget *parent = 0);
    cls_RingVisu(cls_Event& p_event, QWidget *parent = 0);
    ~cls_RingVisu();

    static std::map<UInt_t, cls_PixelMapEntry> ImportPixelMap(QString p_filename="../pixel-coord_channel-register.ascii");

private:
    Ui::cls_RingVisu *ui;

    static std::map<UInt_t, cls_PixelMapEntry> fPixelMap;

    static const Double_t fWidth;
    static const Double_t fHeight;
};

#endif // RINGVISU_H
