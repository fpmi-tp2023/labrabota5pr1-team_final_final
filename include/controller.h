#pragma once
#include <string>
#include <vector>
#include <limits>
#include "model.h"

class Controller
{
private:
    std::string dbName;
    Model *dbModel = nullptr;
public:
    static constexpr auto maxStreamSize = std::numeric_limits<std::streamsize>::max(); // constant for maximum stream size

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

    //get the most popular cd
    std::string getTheMostPopularCD()const;

    //get the most popular artist
    std::string getTheMostPopularArtist()const;

    //Get information on quantity of sold and left in stock records of every record
    std::vector<std::vector<std::string>> getCurrentQuantityOfCD()const;

    //Get information on quantity of sold copies and the cost of given record for given period
    std::vector<std::vector<std::string>> getQuantityOfCDPeriod(std::string begining,std::string ending)const;
    
    //Get the quantity of sold copies of the records and overall sum of money for a given artist
    std::vector<std::string> getInfoArtist(std::string name)const;

    //Get information on sales of given record for given period
    std::vector<std::string> getInfoCDPeriod(std::string begining,std::string ending,std::string disk_id)const;

    //Get information on delivered and sold copies of every record for given period
    std::vector<std::vector<std::string>>getQuantityDeliveredSoldCDPeriod(std::string begining,std::string ending)const;

    // checks if request number is valid. Valid numbers are 0 and 1
    bool validUpdateRequestNumber(int requestNumber) const;

    // returns vector of all tables from database
    std::vector<std::string> getTables() const;

    // returns true if table is in tableList
    bool validTable(const std::string& table, const std::vector<std::string>& tableList) const;

    // return columns from given table
    std::vector<std::string> getColumns(const std::string& table) const;

    // checks if columnsCount is valid. Valid values are in range (1, columns.size())
    bool validColumnsCount(int columnsCount, const std::vector<std::string>& columns) const;

    // checks if column is present in columnList
    bool validColumn(const std::string& column, const std::vector<std::string>& columnList) const;

    // creates update query and performs it. Returns true when successful
    bool createUpdateQuery(
        const std::string& table, 
        const std::vector<std::string>& columnsToUpdate, 
        const std::vector<std::string>& valuesForColumns,
        const std::string& whereCondition) const;
    
    // protects stream in from blocking on wrong input. Returns true if everything ok
    bool intInputGuard(std::istream& in) const;

    // creates delete query and performs it. Returns true when successful
    bool createDeleteQuery(
        const std::string& table,
        const std::string& whereCondition
    ) const;

    // creates insert query and performs it. Returns true when successful
    bool createInsertQuery(
        const std::string& table,
        const std::vector<std::string>& columns,
        const std::vector<std::vector<std::string>>& values
    ) const;

    //get all artists
    std::vector<std::vector<std::string>> getAllArtist()const;

    //get all cd
    std::vector<std::vector<std::string>> getAllCd()const;


    ~Controller();
};