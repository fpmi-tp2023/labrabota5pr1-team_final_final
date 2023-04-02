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
    bool &ok = *(bool *)exists;
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
        "SELECT login\n"
        "FROM login\n"
        "WHERE login = '" +
        login + "';";
    bool exists = false; // callback function will put here the result
    int queryResult = sqlite3_exec(db, sqlExistingLoginQuery.c_str(), existingLogin_Callback, &exists, NULL);
    if (queryResult != SQLITE_OK)
    {
        std::cerr << "Error on retreiving data from db, fname = existingLogin: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    else
    {
        return exists;
    }
}

int Model::getPasswordHash_Callback(void *first_arg, int numberOfColumns, char **data, char **headers)
{
    std::string &passwordHash = *(std::string*)first_arg; // by assigning this variable c-style string we can pass it outside
    passwordHash = data[0];
    return 0;
}

std::string Model::getPasswordHash(const std::string &login) const
{
    std::string sqlGetPasswordHashQuery =
        "SELECT hash\n"
        "FROM login\n"
        "WHERE login = '" +
        login + "';";
    std::string passwordHash; // callback function will place here the result
    int queryResult = sqlite3_exec(db, sqlGetPasswordHashQuery.c_str(), getPasswordHash_Callback, &passwordHash, NULL);
    if (queryResult != SQLITE_OK)
    {
        std::cerr << "Error on retreiving data from db, fname = getPasswordHash: " << sqlite3_errmsg(db) << "\n";
        return std::string();
    }
    else
    {
        return passwordHash;
    }
}

int Model::getRole_Callback(void* first_arg, int numberOfColumns, char** data, char** headers)
{
    std::string &role = *(std::string *)first_arg; // by assigning this variable some value we can pass it outside
    role = data[0];
    return 0;
}
std::string Model::getRole(const std::string &login) const
{
    std::string sqlGetRoleQuery =
        "SELECT type\n"
        "FROM login INNER JOIN role USING(role_id)\n"
        "WHERE login = '" +
        login + "';";
    std::string role; // callback function will place answer here
    int result = sqlite3_exec(db, sqlGetRoleQuery.c_str(), getRole_Callback, &role, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on retreiving data from db, fname = getRole: " << sqlite3_errmsg(db) << "\n";
        return std::string();
    }
    else
    {
        return role;
    }
}
Model::~Model()
{
    sqlite3_close(db); // close db
    db = nullptr;
}