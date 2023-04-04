#pragma once
#include <string>
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
    
    ~Controller();
};