#include <openssl/evp.h>
#include <sstream>
#include <iomanip>
#include "controller.h"
#include <iostream>

Controller::Controller()
{
}

bool Controller::connectDB(const std::string &dbName)
{
    dbModel = new Model(dbName);
    bool res = dbModel->connectDB();
    return res;
}

bool Controller::validResponseAuth(const std::string &response) const
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

bool Controller::existingLogin(const std::string &login) const
{
    return dbModel->existingLogin(login);
}

bool Controller::correctPassword(const std::string &login, const std::string &password) const
{
    std::string dbPasswordHash = dbModel->getPasswordHash(login);
    std::string providedPasswordHash = hashPassword(password);
    return (providedPasswordHash == dbPasswordHash);
}

std::string Controller::getRole(const std::string &login) const
{
    return dbModel->getRole(login);
}

std::string Controller::hashPassword(const std::string &password) const
{
    /*
        EVP is openssl high-level interface i use for digest(hash) a password
        It requires EVP_DigestInit, EVP_DigestUpdate and EVP_DigestFinal
        The result is put into hashed variable
    */
    EVP_MD_CTX *context = EVP_MD_CTX_new();

    std::string hashed;

    if (context != NULL)
    {
        if (EVP_DigestInit_ex(context, EVP_sha256(), NULL))
        {
            if (EVP_DigestUpdate(context, password.c_str(), password.length()))
            {
                unsigned char hash[EVP_MAX_MD_SIZE];
                unsigned int lengthOfHash = 0;

                if (EVP_DigestFinal_ex(context, hash, &lengthOfHash))
                {
                    std::stringstream ss;
                    for (unsigned int i = 0; i < lengthOfHash; ++i)
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

bool Controller::passwordsMatch(const std::string &password, const std::string &confirmedPassword) const
{
    return (password == confirmedPassword);
}

void Controller::newLogin(const std::string &login, const std::string &password) const
{
    std::string hashedPassword = hashPassword(password);
    dbModel->addLogin(login, hashedPassword);
}

bool Controller::deleteLogin(const std::string &login, const std::string &password) const
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

std::string Controller::getTheMostPopularCD()const
{
    return dbModel->getTheMostPopularCD();
}

std::string Controller::getTheMostPopularArtist() const
{
    return dbModel->getTheMostPopularArtist();
}

std::vector<std::vector<std::string>> Controller::getCurrentQuantityOfCD() const
{
    return dbModel->getCurrentQuantityOfCD();
}

std::vector<std::vector<std::string>> Controller::getQuantityOfCDPeriod(std::string begining,std::string ending) const
{
    return dbModel->getQuantityOfCDPeriod(begining,ending);
}

std::vector<std::string> Controller::getInfoArtist(std::string name) const
{
    return dbModel->getInfoArtist(name);
}

std::vector<std::string> Controller::getInfoCDPeriod(std::string begining, std::string ending, std::string disk_id) const
{
    return dbModel->getInfoCDPeriod(begining,ending,disk_id);
}

std::vector<std::vector<std::string>> Controller::getQuantityDeliveredSoldCDPeriod(std::string begining, std::string ending) const
{
    return dbModel->getQuantityDeliveredSoldCDPeriod(begining,ending);
}

bool Controller::validUpdateRequestNumber(int requestNumber) const
{
    return (requestNumber == 0 || requestNumber == 1);
}

std::vector<std::string> Controller::getTables() const
{
    std::vector<std::string> tables;
    dbModel->getTables(&tables);
    return tables;
}

bool Controller::validTable(const std::string &table, const std::vector<std::string> &tableList) const
{
    bool ans = false;
    for (const auto &tableFromList : tableList)
    {
        if (table == tableFromList)
        {
            ans = true;
            break;
        }
    }
    return ans;
}

std::vector<std::string> Controller::getColumns(const std::string &table) const
{
    std::vector<std::string> columns;
    dbModel->getColumns(&columns, table);
    return columns;
}

bool Controller::validColumnsCount(int columnsNumber, const std::vector<std::string> &columns) const
{
    return ((columnsNumber >= 1) && (columnsNumber <= columns.size()));
}

bool Controller::validColumn(const std::string &column, const std::vector<std::string> &columnList) const
{
    bool ans = false;
    for (const auto &columnFromList : columnList)
    {
        if (column == columnFromList)
        {
            ans = true;
            break;
        }
    }
    return ans;
}

bool Controller::createUpdateQuery(
    const std::string &table,
    const std::vector<std::string> &columnsToUpdate,
    const std::vector<std::string> &valuesForColumns,
    const std::string &whereCondition) const
{
    return dbModel->updateQuery(table, columnsToUpdate, valuesForColumns, whereCondition);
}

bool Controller::intInputGuard(std::istream& in) const
{
    if (in.fail()) // int can't be read as input is not integer
    {
        in.clear(); // reset in to the good state
        in.ignore(Controller::maxStreamSize, '\n'); // discard all bad characters before a new \n
        return false;
    }
    return true;
}

bool Controller::createDeleteQuery(
    const std::string& table,
    const std::string& whereCondition
) const
{
    return dbModel->deleteQuery(table, whereCondition);
}

bool Controller::createInsertQuery(
    const std::string& table,
    const std::vector<std::string>& columns,
    const std::vector<std::vector<std::string>>& values
) const
{
    return dbModel->insertQuery(table, columns, values);
}

Controller::~Controller()
{
    if (dbModel) // check if pointer is not null
    {
        delete dbModel;
        dbModel = nullptr;
    }
}

