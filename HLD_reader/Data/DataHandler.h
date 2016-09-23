#ifndef CLS_DATAHANDLER_H
#define CLS_DATAHANDLER_H

#include <map>
#include <vector>
#include "Event.h"

class cls_DataHandler
{
private:
    cls_DataHandler();
    ~cls_DataHandler();

public:
    static cls_DataHandler* Instance();

private:
    static cls_DataHandler* fInstance;

public:
    std::multimap<Double_t, cls_Hit> fMapOfHits;

    std::vector<cls_Event> mEvents;
};

#endif // CLS_DATAHANDLER_H
