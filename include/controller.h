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
    ~Controller();
};