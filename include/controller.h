#pragma once
#include <string>
#include "model.h"

class Controller
{
private:
    std::string dbName;
    Model *dbModel;
public:
    Controller();
    bool connectDB(const std::string& dbName);
    ~Controller();
};