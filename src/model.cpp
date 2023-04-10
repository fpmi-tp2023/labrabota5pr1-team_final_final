#include <iostream>
#include "model.h"

Model::Model(const std::string &dbFileName)
{
    this->dbFileName = dbFileName;
}

bool Model::connectDB()
{
    int openResult;
    openResult = sqlite3_open_v2(dbFileName.c_str(), &db, SQLITE_OPEN_READWRITE, NULL); // open db
    if (openResult)
    {
        std::cerr << sqlite3_errmsg(db) << "\n"; // output last error message from db
        return false;
    }
    return true;
}

int Model::existingLogin_Callback(void *exists, int numberOfColumns, char **data, char **headers)
{
    bool &ok = *(bool *)exists;
    if (data != NULL)
    {
        ok = true;
    }
    else
    {
        ok = false;
    }
    return 0;
}

bool Model::existingLogin(const std::string &login) const
{
    std::string sqlExistingLoginQuery =
        "SELECT login\n"
        "FROM login\n"
        "WHERE login = '" +
        login + "';";
    bool exists = false; // callback function will put here the result
    int queryResult = sqlite3_exec(db, sqlExistingLoginQuery.c_str(), existingLogin_Callback, &exists, NULL);
    if (queryResult != SQLITE_OK)
    {
        std::cerr << "Error on retreiving data from db, fname = existingLogin: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    else
    {
        return exists;
    }
}

int Model::getPasswordHash_Callback(void *first_arg, int numberOfColumns, char **data, char **headers)
{
    std::string &passwordHash = *(std::string *)first_arg; // by assigning this variable c-style string we can pass it outside
    passwordHash = data[0];
    return 0;
}

std::string Model::getPasswordHash(const std::string &login) const
{
    std::string sqlGetPasswordHashQuery =
        "SELECT hash\n"
        "FROM login\n"
        "WHERE login = '" +
        login + "';";
    std::string passwordHash; // callback function will place here the result
    int queryResult = sqlite3_exec(db, sqlGetPasswordHashQuery.c_str(), getPasswordHash_Callback, &passwordHash, NULL);
    if (queryResult != SQLITE_OK)
    {
        std::cerr << "Error on retreiving data from db, fname = getPasswordHash: " << sqlite3_errmsg(db) << "\n";
        return std::string();
    }
    else
    {
        return passwordHash;
    }
}

int Model::getRole_Callback(void *first_arg, int numberOfColumns, char **data, char **headers)
{
    std::string &role = *(std::string *)first_arg; // by assigning this variable some value we can pass it outside
    role = data[0];
    return 0;
}
std::string Model::getRole(const std::string &login) const
{
    std::string sqlGetRoleQuery =
        "SELECT type\n"
        "FROM login INNER JOIN role USING(role_id)\n"
        "WHERE login = '" +
        login + "';";
    std::string role; // callback function will place answer here
    int result = sqlite3_exec(db, sqlGetRoleQuery.c_str(), getRole_Callback, &role, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on retreiving data from db, fname = getRole: " << sqlite3_errmsg(db) << "\n";
        return std::string();
    }
    else
    {
        return role;
    }
}

/*int Model::addLogin_Callback(void* option, int numberOfColumns, char** data, char** headers)
{
    return 0;
}*/

void Model::addLogin(const std::string &login, const std::string &hashedPassword) const
{
    std::string sqlAddLoginQuery =
        "INSERT INTO login(login, hash, role_id) VALUES\n"
        "('" +
        login + "', '" + hashedPassword + "', 1);";
    // int result = sqlite3_exec(db, sqlAddLoginQuery.c_str(), addLogin_Callback, 0, 0);
    int result = sqlite3_exec(db, sqlAddLoginQuery.c_str(), 0, 0, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on retreiving data from db, fname = addLogin: " << sqlite3_errmsg(db) << "\n";
    }
}

/*int Model::deleteLogin_Callback(void *optional, int numberOfColumns, char** data, char** headers)
{
    return 0;
}*/
bool Model::deleteLogin(const std::string &login) const
{
    std::string sqlDeleteQuery =
        "DELETE FROM login\n"
        "WHERE login = '" +
        login + "';";
    // int result = sqlite3_exec(db, sqlDeleteQuery.c_str(), deleteLogin_Callback, 0, 0);
    int result = sqlite3_exec(db, sqlDeleteQuery.c_str(), 0, 0, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on deleting from db, fname = deleteLogin: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    else
    {
        return true;
    }
}

int Model::getTables_Callback(void *optional, int numberOfColumns, char **data, char **headers)
{
    ((std::vector<std::string> *)optional)->push_back(std::string(data[0]));
    return 0;
}

void Model::getTables(std::vector<std::string> *tables) const
{
    std::string sqlGetTablesQuery =
        "SELECT name\n"
        "FROM sqlite_schema\n"
        "WHERE type = 'table' AND name NOT LIKE 'sqlite_%';";
    int result = sqlite3_exec(db, sqlGetTablesQuery.c_str(), getTables_Callback, tables, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on retreiving tables from db, fname = getTables: " << sqlite3_errmsg(db) << "\n";
    }
}

int Model::getColumns_Callback(void *optional, int numberOfColumns, char **data, char **headers)
{
    ((std::vector<std::string> *)optional)->push_back(data[1]);
    return 0;
}


void Model::getColumns(std::vector<std::string> *columns, const std::string &table) const
{
    std::string sqlGetColumnsQuery =
        "PRAGMA table_info(" + table + ");";
    int result = sqlite3_exec(db, sqlGetColumnsQuery.c_str(), getColumns_Callback, columns, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on retreiving columns from db, fname = getColumns: " << sqlite3_errmsg(db) << "\n";
    }
}

bool Model::updateQuery(
    const std::string &table,
    const std::vector<std::string> &columnsToUpdate,
    const std::vector<std::string> &valuesForColumns,
    const std::string &whereCondition) const
{
    std::string sqlUpdateQuery =
        "UPDATE " + table + "\nSET ";
    for (size_t i = 0; i < columnsToUpdate.size(); ++i)
    {
        if (i != 0)
        {
            sqlUpdateQuery += ", ";
        }
        sqlUpdateQuery += columnsToUpdate[i] + " = " + valuesForColumns[i];
    }
    sqlUpdateQuery += "\nWHERE " + whereCondition + ";";
    int result = sqlite3_exec(db, sqlUpdateQuery.c_str(), 0, 0, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Something went wrong on updating the database, fname = updateQuery: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    return true;
}

bool Model::deleteQuery(
    const std::string &table,
    const std::string &whereCondition) const
{
    std::string sqlDeleteQuery =
        "DELETE FROM " + table + "\n"
                                 "WHERE " +
        whereCondition + ";";
    int result = sqlite3_exec(db, sqlDeleteQuery.c_str(), 0, 0, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Something went wrong in deleting from database, fname = deleteQuery: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    return true;
}

bool Model::insertQuery(
    const std::string &table,
    const std::vector<std::string> &columns,
    const std::vector<std::vector<std::string>> &values) const
{
    std::string sqlInsertQuery = "INSERT INTO " + table + "(";
    for (size_t i = 0; i < columns.size(); ++i)
    {
        if (i != 0)
        {
            sqlInsertQuery += ", ";
        }
        sqlInsertQuery += columns[i];
    }

    sqlInsertQuery += ") VALUES\n";

    for (size_t i = 0; i < values.size(); ++i)
    {
        if (i != 0)
        {
            sqlInsertQuery += ",\n";
        }
        sqlInsertQuery += "(";
        for (size_t j = 0; j < values[i].size(); ++j)
        {
            if (j != 0)
            {
                sqlInsertQuery += ", ";
            }
            sqlInsertQuery += values[i][j];
        }
        sqlInsertQuery += ")";
    }
    sqlInsertQuery += ";";
    int result = sqlite3_exec(db, sqlInsertQuery.c_str(), 0, 0, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Something went wrong on inserting into database, fname = insertQuery: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    return true;
}

Model::~Model()
{
    sqlite3_close(db); // close db
    db = nullptr;
}

int Model::TheMostPopularCD_Callback(void *first_arg, int numberOfColumns, char **data, char **headers)
{
    std::string &TheMostPopularCD = *(std::string *)first_arg;
    TheMostPopularCD = data[0];
    TheMostPopularCD += ".\nCurrent amount of sold disks:";
    TheMostPopularCD += data[1];
    return 0;
}

int Model::TheMostPopularArtist_Callback(void *first_arg, int numberOfColumns, char **data, char **headers)
{
    std::string &TheMostPopularArtist = *(std::string *)first_arg;
    TheMostPopularArtist = data[0];
    TheMostPopularArtist += ".\nCurrent amount of sold disks:";
    TheMostPopularArtist += data[1];
    return 0;
}

int Model::CurrentQuantityOfCD_Callback(void *first_arg, int numberOfColumns, char **data, char **headers)
{
    std::string &CurrentQuantityOfCD = *(std::string *)first_arg;
    CurrentQuantityOfCD += data[0];
    CurrentQuantityOfCD += " | ";
    CurrentQuantityOfCD += data[1];
    CurrentQuantityOfCD += " | ";
    CurrentQuantityOfCD += data[2];
    CurrentQuantityOfCD += "\n";
    return 0;
}

int Model::QuantityOfCDPeriod_Callback(void *first_arg, int numberOfColumns, char **data, char **headers)
{
    std::string &CurrentQuantityOfCD = *(std::string *)first_arg;
    CurrentQuantityOfCD += data[0];
    CurrentQuantityOfCD += " | ";
    CurrentQuantityOfCD += data[1];
    CurrentQuantityOfCD += " | ";
    CurrentQuantityOfCD += data[2];
    CurrentQuantityOfCD += "\n";
    return 0;
}

int Model::InfoArtist_Callback(void *first_arg, int numberOfColumns, char **data, char **headers)
{
    std::string &InfoArtist = *(std::string *)first_arg;
    InfoArtist += data[0];
    InfoArtist += " | ";
    InfoArtist += data[1];
    InfoArtist += " | ";
    InfoArtist += data[2];
    InfoArtist += "\n";
    return 0;
}

std::string Model::getTheMostPopularCD() const
{
    std::string TheMostPopularCDQuery =
        "SELECT discs.name,sum(operation_details.quantity)\n"
        "FROM operation_details\n"
        "INNER JOIN discs ON discs.discs_id=operation_details.discs_id\n"
        "INNER JOIN operation ON operation.operation_id=operation_details.operation_id\n"
        "INNER JOIN operation_type ON operation_type.operation_type_id=operation.operation_type_id\n"
        "WHERE operation_type.operation_type = 'Sell'\n"
        "GROUP BY operation_details.discs_id\n"
        "ORDER BY operation_details.quantity DESC\n"
        "LIMIT 1;";
    std::string TheMostPopularCD;
    int result = sqlite3_exec(db, TheMostPopularCDQuery.c_str(), TheMostPopularCD_Callback, &TheMostPopularCD, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on retreiving data from db, fname = getTheMostPopularCD: " << sqlite3_errmsg(db) << "\n";
        return std::string();
    }
    else
    {
        return TheMostPopularCD;
    }
}

std::string Model::getTheMostPopularArtist() const
{
    std::string TheMostPopularArtistQuery =
        "SELECT artist.name,sum(operation_details.quantity)\n"
        "FROM operation_details\n"
        "INNER JOIN discs ON discs.discs_id=operation_details.discs_id\n"
        "INNER JOIN operation ON operation.operation_id=operation_details.operation_id\n"
        "INNER JOIN operation_type ON operation_type.operation_type_id=operation.operation_type_id\n"
        "INNER JOIN artist ON discs.artist_id=artist.artist_id\n"
        "WHERE operation_type.operation_type = 'Sell'\n"
        "GROUP BY discs.artist_id\n"
        "ORDER BY operation_details.quantity DESC\n"
        "LIMIT 1;";
    std::string TheMostPopularArtist;
    int result = sqlite3_exec(db, TheMostPopularArtistQuery.c_str(), TheMostPopularArtist_Callback, &TheMostPopularArtist, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on retreiving data from db, fname = getTheMostPopularArtist: " << sqlite3_errmsg(db) << "\n";
        return std::string();
    }
    else
    {
        return TheMostPopularArtist;
    }
}

std::string Model::getCurrentQuantityOfCD() const
{
    std::string CurrentQuantityOfCDQuery =
        "SELECT discs.name,sum(operation_details.quantity),discs.amount_in_stock\n"
        "FROM operation_details\n"
        "INNER JOIN discs ON discs.discs_id=operation_details.discs_id\n"
        "INNER JOIN operation ON operation.operation_id=operation_details.operation_id\n"
        "INNER JOIN operation_type ON operation_type.operation_type_id=operation.operation_type_id\n"
        "WHERE operation_type.operation_type = 'Sell'\n"
        "GROUP BY discs.discs_id\n"
        "ORDER BY discs.amount_in_stock DESC;";
    std::string CurrentQuantityOfCD;
    int result = sqlite3_exec(db, CurrentQuantityOfCDQuery.c_str(), CurrentQuantityOfCD_Callback, &CurrentQuantityOfCD, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on retreiving data from db, fname = getCurrentQuantityOfCD: " << sqlite3_errmsg(db) << "\n";
        return std::string();
    }
    else
    {
        return CurrentQuantityOfCD;
    }
}

std::string Model::getQuantityOfCDPeriod(std::string beginin, std::string ending) const
{
    std::string QuantityOfCDPeriodQuery =
        "SELECT discs.name,sum(operation_details.quantity),discs.price * sum(operation_details.quantity)\n"
        "FROM operation_details\n"
        "INNER JOIN discs ON discs.discs_id=operation_details.discs_id\n"
        "INNER JOIN operation ON operation.operation_id=operation_details.operation_id\n"
        "INNER JOIN operation_type ON operation_type.operation_type_id=operation.operation_type_id\n"
        "WHERE operation_type.operation_type = 'Sell' and '" +
        beginin + "' < operation.date and operation.date < '" + ending + "'\n"
        "GROUP BY operation_details.discs_id\n"
        "ORDER BY operation_details.quantity DESC\n";
    std::string QuantityOfCDPeriod;
    int result = sqlite3_exec(db, QuantityOfCDPeriodQuery.c_str(),QuantityOfCDPeriod_Callback, &QuantityOfCDPeriod, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on retreiving data from db, fname = getQuantityOfCDPeriod: " << sqlite3_errmsg(db) << "\n";
        return std::string();
    }
    else
    {
        return QuantityOfCDPeriod;
    }
}

std::string Model::getInfoArtist(std::string name)
{
    std::string InfoArtistQuery =
        "SELECT artist.name,sum(operation_details.quantity),discs.price * sum(operation_details.quantity)\n"
        "FROM operation_details\n"
        "INNER JOIN discs ON discs.discs_id=operation_details.discs_id\n"
        "INNER JOIN operation ON operation.operation_id=operation_details.operation_id\n"
        "INNER JOIN operation_type ON operation_type.operation_type_id=operation.operation_type_id\n"
        "INNER JOIN artist ON discs.artist_id=artist.artist_id\n"
        "WHERE operation_type.operation_type = 'Sell' and artist.name = '" + name + "'\n"
        "GROUP BY discs.artist_id\n"
        "ORDER BY operation_details.quantity DESC\n";
    std::string InfoArtist;
    int result = sqlite3_exec(db, InfoArtistQuery.c_str(),InfoArtist_Callback, &InfoArtist, 0);
    if (result != SQLITE_OK)
    {
        std::cerr << "Error on retreiving data from db, fname = getInfoArtist: " << sqlite3_errmsg(db) << "\n";
        return std::string();
    }
    else
    {
        return InfoArtist;
    }
}
