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

/*int Model::addLogin_Callback(void* option, int numberOfColumns, char** data, char** headers)
{
    return 0;
}*/

void Model::addLogin(const std::string& login, const std::string& hashedPassword) const
{
    std::string sqlAddLoginQuery = 
    "INSERT INTO login(login, hash, role_id) VALUES\n"
    "('" + login + "', '" + hashedPassword + "', 1);";
    //int result = sqlite3_exec(db, sqlAddLoginQuery.c_str(), addLogin_Callback, 0, 0);
    int result = sqlite3_exec(db, sqlAddLoginQuery.c_str(), 0, 0, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on retreiving data from db, fname = addLogin: " << sqlite3_errmsg(db) << "\n";
    }
}

/*int Model::deleteLogin_Callback(void *optional, int numberOfColumns, char** data, char** headers)
{
    return 0;
}*/
bool Model::deleteLogin(const std::string& login) const
{
    std::string sqlDeleteQuery = 
    "DELETE FROM login\n"
    "WHERE login = '" + login + "';";
    //int result = sqlite3_exec(db, sqlDeleteQuery.c_str(), deleteLogin_Callback, 0, 0);
    int result = sqlite3_exec(db, sqlDeleteQuery.c_str(), 0, 0, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on deleting from db, fname = deleteLogin: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    else
    {
        return true;
    }
}

int Model::getTables_Callback(void* optional, int numberOfColumns, char** data, char** headers)
{
    ((std::vector<std::string>*)optional)->push_back(std::string(data[0]));
    return 0;
}

void Model::getTables(std::vector<std::string>* tables) const
{
    std::string sqlGetTablesQuery = 
    "SELECT name\n"
    "FROM sqlite_schema\n"
    "WHERE type = 'table' AND name NOT LIKE 'sqlite_%';";
    int result = sqlite3_exec(db, sqlGetTablesQuery.c_str(), getTables_Callback, tables, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on retreiving tables from db, fname = getTables: " << sqlite3_errmsg(db) << "\n";
    }
}

int Model::getColumns_Callback(void* optional, int numberOfColumns, char** data, char** headers)
{
    ((std::vector<std::string> *)optional)->push_back(data[1]);
    return 0;
}

void Model::getColumns(std::vector<std::string>* columns, const std::string& table) const
{
    std::string sqlGetColumnsQuery = 
    "PRAGMA table_info(" + table + ");";
    int result = sqlite3_exec(db, sqlGetColumnsQuery.c_str(), getColumns_Callback, columns, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on retreiving columns from db, fname = getColumns: " << sqlite3_errmsg(db) << "\n";
    }
}

bool Model::updateQuery(
    const std::string& table,
    const std::vector<std::string>& columnsToUpdate,
    const std::vector<std::string>& valuesForColumns,
    const std::string& whereCondition
) const
{
    std::string sqlUpdateQuery = 
    "UPDATE " + table + "\n";
    for (size_t i = 0; i < columnsToUpdate.size(); ++i)
    {
        if (i != 0)
        {
            sqlUpdateQuery += ",";
        }
        sqlUpdateQuery += "SET " + columnsToUpdate[i] + " = " + valuesForColumns[i];
    }
    sqlUpdateQuery += "\nWHERE " + whereCondition + ";";
    int result = sqlite3_exec(db, sqlUpdateQuery.c_str(), 0, 0, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Something went wrong on updating the database, fname = updateQuery: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    return true;
}


bool Model::deleteQuery(
    const std::string& table,
    const std::string& whereCondition
) const
{
    std::string sqlDeleteQuery =
    "DELETE FROM " + table + "\n"
    "WHERE " + whereCondition + ";";
    int result = sqlite3_exec(db, sqlDeleteQuery.c_str(), 0, 0, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Something went wrong in deleting from database, fname = deleteQuery: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    return true;
}

Model::~Model()
{
    sqlite3_close(db); // close db
    db = nullptr;
}