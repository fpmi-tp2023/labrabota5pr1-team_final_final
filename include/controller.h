#pragma once
#include <string>
#include <vector>
#include "model.h"

class Controller
{
private:
    std::string dbName;
    Model *dbModel = nullptr;
public:
    Controller();

    // connects a db by given dbName
    bool connectDB(const std::string& dbName);

    // checks if responce is valid. Valid responses are sign-in and sign-up
    bool validResponseAuth(const std::string& response) const;

    // checks if provided login exists
    bool existingLogin(const std::string& login) const;

    // checks if password hashes match for given login
    bool correctPassword(const std::string& login, const std::string& password) const;

    // returns role by login
    std::string getRole(const std::string& login) const;

    // returns hash from password via sha256
    std::string hashPassword(const std::string& password) const;

    // returns true if passwords match
    bool passwordsMatch(const std::string& password, const std::string& confirmedPassword) const;

    // adds new login, password into database
    void newLogin(const std::string& login, const std::string& password) const;

    // deletes login with password from database, used only for tests!!!
    bool deleteLogin(const std::string& login, const std::string& password) const;
    
    // checks if request number is valid. Valid numbers are 0 and 1
    bool validUpdateRequestNumber(int requestNumber) const;

    std::vector<std::string> getTables() const;

    ~Controller();
};