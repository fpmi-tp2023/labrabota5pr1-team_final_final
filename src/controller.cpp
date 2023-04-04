#include <openssl/evp.h>
#include <sstream>
#include <iomanip>
#include "controller.h"
#include <iostream>

Controller::Controller()
{

}

bool Controller::connectDB(const std::string& dbName)
{
    dbModel = new Model(dbName);
    bool res = dbModel->connectDB();
    return res;
}

bool Controller::validResponseAuth(const std::string& response) const
{
    if (response == "sign-up" or response == "sign-in")
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Controller::existingLogin(const std::string& login) const
{
    return dbModel->existingLogin(login);
}

bool Controller::correctPassword(const std::string& login, const std::string& password) const
{
    std::string dbPasswordHash = dbModel->getPasswordHash(login);
    std::string providedPasswordHash = hashPassword(password);
    return (providedPasswordHash == dbPasswordHash);
}

std::string Controller::getRole(const std::string& login) const
{
    return dbModel->getRole(login);
}

std::string Controller::hashPassword(const std::string& password) const
{
    /* 
        EVP is openssl high-level interface i use for digest(hash) a password
        It requires EVP_DigestInit, EVP_DigestUpdate and EVP_DigestFinal
        The result is put into hashed variable
    */
    EVP_MD_CTX* context = EVP_MD_CTX_new(); 

    std::string hashed;

    if(context != NULL)
    {
        if(EVP_DigestInit_ex(context, EVP_sha256(), NULL))
        {
            if(EVP_DigestUpdate(context, password.c_str(), password.length()))
            {
                unsigned char hash[EVP_MAX_MD_SIZE];
                unsigned int lengthOfHash = 0;

                if(EVP_DigestFinal_ex(context, hash, &lengthOfHash))
                {
                    std::stringstream ss;
                    for(unsigned int i = 0; i < lengthOfHash; ++i)
                    {
                        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
                    }

                    hashed = ss.str();
                }
            }
        }

        EVP_MD_CTX_free(context);
    }
    return hashed;
}

bool Controller::passwordsMatch(const std::string& password, const std::string& confirmedPassword) const
{
    return (password == confirmedPassword);
}

void Controller::newLogin(const std::string& login, const std::string& password) const
{
    std::string hashedPassword = hashPassword(password);
    dbModel->addLogin(login, hashedPassword);
}


bool Controller::deleteLogin(const std::string& login, const std::string& password) const
{
    // if login exists, passwod match and it's not admin
    if (login != "admin" && existingLogin(login) && correctPassword(login, password)) 
    {
        return dbModel->deleteLogin(login);
    }
    else
    {
        return false;
    }
}

Controller::~Controller()
{
    if (dbModel)  //check if pointer is not null
    {
        delete dbModel;
        dbModel = nullptr;
    }
}