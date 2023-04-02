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
    bool connectDB(const std::string& dbName);
    bool validResponseAuth(const std::string& response) const;
    ~Controller();
};