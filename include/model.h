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

    // invoked by sqlite during addLogin methid
    // it appears it doesn't get invoked at all so it can be commented
    //static int addLogin_Callback(void* optional, int numberOfColumns, char** data, char** headers);

    // invoked by sqlite during deleteLogin method
    // it appears it doesn't get invoked at all so it can be commented
    //static int deleteLogin_Callback(void* optional, int numberOfColumns, char** data, char** headers);

    // invoked by sqlite during getTables method
    static int getTables_Callback(void *optional, int numberOfColumns, char** data, char** headers);

    // invoked by sqlite during getColumns method
    static int getColumns_Callback(void* optional, int numberOfColumns, char** data, char** headers);

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

    // retreives all table names from db and puts them into tables vector
    void getTables(std::vector<std::string>* tables) const;

    // retreives all columns from a given table and puts them into columns vector
    void getColumns(std::vector<std::string>* columns, const std::string& table) const;

    // create and performs update query with given params. Returns true when successful
    bool updateQuery(
        const std::string& table,
        const std::vector<std::string>& columnsToUpdate,
        const std::vector<std::string>& valuesForColumns,
        const std::string& whereCondition
    ) const;

    // creates and performs delete query with given params. Returns true when successful
    bool deleteQuery(
        const std::string& table,
        const std::string& whereCondition
    ) const;

    // creates and performs insert query with given params. Returns true when successful 
    bool insertQuery(
        const std::string& table,
        const std::vector<std::string>& columns,
        const std::vector<std::vector<std::string>>& values
    ) const;

    ~Model();
};