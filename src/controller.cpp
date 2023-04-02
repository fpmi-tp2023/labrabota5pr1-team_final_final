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

bool Controller::validResponseAuth(const std::string& response) const
{
    if (response == "sign-up" or response == "sign-in")
    {
        return true;
    }
    else
    {
        return false;
    }
}
Controller::~Controller()
{
    if (dbModel)  //check if pointer is not null
    {
        delete dbModel;
        dbModel = nullptr;
    }
}