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

bool Controller::existingLogin(const std::string& login) const
{
    return dbModel->existingLogin(login);
}

bool Controller::correctPassword(const std::string& login, const std::string& password) const
{
    std::string dbPasswordHash = dbModel->getPasswordHash(login);
    std::string providedPasswordHash = "admin";//hashSha256(password);
    return (providedPasswordHash == dbPasswordHash);
}

std::string Controller::getRole(const std::string& login) const
{
    return dbModel->getRole(login);
}

Controller::~Controller()
{
    if (dbModel)  //check if pointer is not null
    {
        delete dbModel;
        dbModel = nullptr;
    }
}