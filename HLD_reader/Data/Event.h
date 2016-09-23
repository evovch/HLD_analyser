#ifndef CLS_EVENT_H
#define CLS_EVENT_H

#include <vector>

#include "Hit.h"

class cls_Event
{
public:
    cls_Event();
    ~cls_Event();

    void AddHit(cls_Hit p_hit) { mHits.push_back(p_hit); }

    UInt_t GetSize() { return mHits.size(); }

    std::vector<cls_Hit> mHits;
};

#endif // CLS_EVENT_H
