#pragma once
#include <string>
#include <sqlite3.h>

class Model
{
private:
    std::string dbFileName;
    sqlite3 *db = nullptr;
public:
    Model(const std::string& dbFileName);

    //connects db by dbFileName
    bool connectDB();

    //checks if login exists in db
    bool existingLogin(const std::string& login) const;

    //gets passwordHash by login
    std::string getPasswordHash(const std::string& login) const;

    //gets role by login
    std::string getRole(const std::string& login) const;
    
    ~Model();
};