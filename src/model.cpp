#include <iostream>
#include "model.h"

Model::Model(const std::string &dbFileName)
{
    this->dbFileName = dbFileName;
}

bool Model::connectDB()
{
    int openResult;
    openResult = sqlite3_open(dbFileName.c_str(), &db);
    if (openResult)
    {
        std::cerr << sqlite3_errmsg(db);
        return false;
    }
    return true;
}

Model::~Model()
{
    sqlite3_close(db);
    db = nullptr;
}