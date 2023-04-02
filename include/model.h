#pragma once
#include <string>
#include <sqlite3.h>

class Model
{
private:
    std::string dbFileName;
    sqlite3 *db = nullptr;
public:
    Model(const std::string&);
    bool connectDB();
    ~Model();
};