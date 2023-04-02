#include "controller.h"

Controller::Controller()
{

}

bool Controller::connectDB(const std::string& dbName)
{
    dbModel = new Model(dbName);
    bool res = dbModel->connectDB();
    return res;
}

Controller::~Controller()
{
    if (dbModel)
    {
        delete dbModel;
        dbModel = nullptr;
    }
}