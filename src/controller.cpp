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

Controller::~Controller()
{
    if (dbModel)  //check if pointer is not null
    {
        delete dbModel;
        dbModel = nullptr;
    }
}