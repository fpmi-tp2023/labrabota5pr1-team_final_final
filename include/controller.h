#pragma once
#include <string>
#include "model.h"

class Controller
{
private:
    std::string dbName;
    Model *dbModel = nullptr;
public:
    Controller();
    //connects a db by given dbName
    bool connectDB(const std::string& dbName);
    //valid responses are sign-in and sign-up
    bool validResponseAuth(const std::string& response) const;
    //checks if provided login exists
    bool existingLogin(const std::string& login) const;
    //checks if password hashes match for given login
    bool correctPassword(const std::string& login, const std::string& password) const;
    ~Controller();
};