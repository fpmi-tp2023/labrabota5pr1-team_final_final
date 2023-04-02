#include <iostream>
#include "model.h"

Model::Model(const std::string &dbFileName)
{
    this->dbFileName = dbFileName;
}

bool Model::connectDB()
{
    int openResult;
    openResult = sqlite3_open_v2(dbFileName.c_str(), &db, SQLITE_OPEN_READWRITE, NULL); // open db
    if (openResult)
    {
        std::cerr << sqlite3_errmsg(db) << "\n"; // output last error message from db
        return false;
    }
    return true;
}

int Model::existingLogin_Callback(void *exists, int numberOfColumns, char **data, char **headers)
{
    bool &ok = *(bool*)exists;
    if (data != NULL)
    {
        ok = true;
    }
    else
    {
        ok = false;
    }
    return 0;
}

bool Model::existingLogin(const std::string &login) const
{
    std::string sqlExistingLoginQuery =
        "SELECT login"
        "FROM login"
        "WHERE login = " +
        login;
    bool exists = false;
    int queryResult = sqlite3_exec(db, sqlExistingLoginQuery.c_str(), existingLogin_Callback, &exists, NULL);
    if (queryResult != SQLITE_OK)
    {
        std::cerr << "error on retreiving data from db, fname = existingLogin\n";
        return false;
    }
    else
    {
        return exists;
    }
}

Model::~Model()
{
    sqlite3_close(db); // close db
    db = nullptr;
}