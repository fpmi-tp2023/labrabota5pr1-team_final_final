#pragma once
#include <string>
#include <sqlite3.h>
#include <vector>

class Model
{
private:
    std::string dbFileName;
    sqlite3 *db = nullptr;

    // invoked by sqlite during existingLogin method
    static int existingLogin_Callback(void* optionalArgument, int numberOfColumns, char** data, char** headers);

    // invoked by sqlite during getPasswordHash method
    static int getPasswordHash_Callback(void* optional, int numberOfColumns, char** data, char** headers);

    // invoked by sqlite during getRole method
    static int getRole_Callback(void* optional, int numberOfColumns, char** data, char** headers);

    // invoked by sqlite during addLogin method
    // it appears it doesn't get invoked at all so it can be commented
    //static int addLogin_Callback(void* optional, int numberOfColumns, char** data, char** headers);

    // invoked by sqlite during deleteLogin method
    // it appears it doesn't get invoked at all so it can be commented
    //static int deleteLogin_Callback(void* optional, int numberOfColumns, char** data, char** headers);

    //invoked by sqlite during getTheMostPopularCD method
    static int TheMostPopularCD_Callback(void* optional, int numberOfColumns, char** data, char** headers);

    //invoked by sqlite during getTheMostPopularArtist method
    static int TheMostPopularArtist_Callback(void* optional, int numberOfColumns, char** data, char** headers);

    //invoked by sqlite during getCurrentQuantityOfCD method
    static int CurrentQuantityOfCD_Callback(void* optional, int numberOfColumns, char** data, char** headers);

public:
    Model(const std::string& dbFileName);

    // connects db by dbFileName
    bool connectDB();

    // checks if login exists in db
    bool existingLogin(const std::string& login) const;

    // gets password hash by login
    std::string getPasswordHash(const std::string& login) const;

    // gets role by login
    std::string getRole(const std::string& login) const;

    // adds new login into database
    void addLogin(const std::string& login, const std::string& hashedPassword) const;

    // deletes login from database
    bool deleteLogin(const std::string& login) const;

    //get the most popular cd
    std::string getTheMostPopularCD()const;

    //get the most popular artist
    std::string getTheMostPopularArtist()const;

    //Get information on quantity of sold and left in stock records of every record
    std::string getCurrentQuantityOfCD()const;

    //Get information on quantity of sold copies and the cost of given record for given period
    std::string getQuantityOfCDPeriod()const;

    ~Model();
};