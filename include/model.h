#pragma once
#include <string>
#include <sqlite3.h>

class Model
{
private:
    std::string dbFileName;
    sqlite3 *db = nullptr;

    // invoked by sqlite during existingLogin method
    static int existingLogin_Callback(void* optionalArgument, int numberOfColumns, char** data, char** headers);

    // invoked by sqlite during getPasswordHash method
    static int getPasswordHash_Callback(void* optional, int numberOfColumns, char** data, char** headers);

    //invoked by sqlite during getRole method
    static int getRole_Callback(void* optional, int numberOfColumns, char** data, char** headers);
public:
    Model(const std::string& dbFileName);

    //connects db by dbFileName
    bool connectDB();

    //checks if login exists in db
    bool existingLogin(const std::string& login) const;

    //gets password hash by login
    std::string getPasswordHash(const std::string& login) const;

    //gets role by login
    std::string getRole(const std::string& login) const;

    ~Model();
};