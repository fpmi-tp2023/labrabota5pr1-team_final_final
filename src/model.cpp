#include <iostream>
#include "model.h"

Model::Model(const std::string &dbFileName)
{
    this->dbFileName = dbFileName;
}

bool Model::connectDB()
{
    int openResult;
    openResult = sqlite3_open_v2(dbFileName.c_str(), &db, SQLITE_OPEN_READWRITE, NULL);
    if (openResult)
    {
        std::cerr << sqlite3_errmsg(db) << "\n";
        return false;
    }
    return true;
}

Model::~Model()
{
    sqlite3_close(db);
    db = nullptr;
}