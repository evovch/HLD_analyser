#include "DataHandler.h"

cls_DataHandler* cls_DataHandler::fInstance = nullptr;

cls_DataHandler::cls_DataHandler()
{
}

cls_DataHandler::~cls_DataHandler()
{
}

cls_DataHandler* cls_DataHandler::Instance()
{
    if (fInstance == nullptr) {
        fInstance = new cls_DataHandler();
        return fInstance;
    }
    else return fInstance;
}
