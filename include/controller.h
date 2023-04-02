#pragma once
#include <string>

class Controller
{
private:
    std::string dbName;
public:
    Controller();
    bool connectDB(const std::string& dbName);
};