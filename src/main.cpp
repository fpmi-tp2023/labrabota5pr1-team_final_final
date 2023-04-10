#include <iostream>
#include <vector>
#include "controller.h"
#include <iomanip>
#include <regex>

int main()
{
    bool authorized = false;
    const std::string dbFileName = "RecordStore.db";
    Controller ctrl;

    if (!ctrl.connectDB(dbFileName))
    {
        std::cerr << "Something went wrong when opening the database\n";
        return 1;
    }

    while (true) // main authorization cycle
    {
        std::string role = "";
        std::string login = "";

        if (!authorized)
        {
            std::string response = "";
            std::cout << "You are currently unauthorized. Enter sign-in or sign-up or quit\n";
            std::cin >> response;
            while (response != "quit" && !ctrl.validResponseAuth(response)) // waiting for valid response
            {
                std::cout << "No such option as " << response << ". Enter sign-in or sign-up or quit\n";
                std::cin >> response;
            }

            if (response == "quit") // quit option
            {
                break;
            }

            if (response == "sign-in") // sign-in option
            {
                std::cout << "Login: ";
                std::cin >> login;

                while (login != "cancel" && (login.empty() || !ctrl.existingLogin(login))) // optional quit or waiting for valid login
                {
                    std::cout << "There is no such login\n";
                    std::cout << "Enter cancel or existing login\n";
                    std::cin >> login;
                }

                if (login == "cancel") // option cancel
                {
                    continue; // return to autorization choice stage
                }

                // valid login
                std::string password;
                int wrongPasswordAttempts = 3;

                std::cout << "Password: ";
                std::cin >> password;
                while (wrongPasswordAttempts > 0 && !ctrl.correctPassword(login, password))
                {
                    --wrongPasswordAttempts;
                    std::cout << "Incorrect password. Try again or type cancel\n";
                    std::cout << "Password: ";
                    std::cin >> password;
                    if (password == "cancel")
                    {
                        break;
                    }
                    std::cout << std::endl;
                }

                if (password == "cancel")
                {
                    continue;
                }

                if (wrongPasswordAttempts == 0) // all attempts exhausted but correct password was never received
                {
                    std::cout << "All wrong password attempts exhaused: quiting...\n";

                    /*
                        There should be some kind of block but since it's student's project no block provided.
                    */
                    return 1;
                }

                // correct password was entered
                authorized = true;
            }
            else if (response == "sign-up") // option sign-up
            {
                std::cout << "Enter your desired login or cancel\n";
                std::cout << "Login: ";
                std::cin >> login;
                while (login != "cancel" && ctrl.existingLogin(login)) // wait for cancel or new login
                {
                    std::cout << "Your login is already taken, try another one or enter cancel\n";
                    std::cout << "Login:";
                    std::cin >> login;
                }

                if (login == "cancel") // cancel option
                {
                    continue; // return to main authorization cycle;
                }

                // new login was entered, now asking for password
                std::string password;
                std::string confirmedPassword;
                std::cout << "Enter your password\n";
                std::cout << "Password: ";
                std::cin >> password;
                std::cout << "Confirm: ";
                std::cin >> confirmedPassword;
                while (password != "cancel" && !ctrl.passwordsMatch(password, confirmedPassword)) // wait for cancel or passwords to match
                {
                    std::cout << "Your password doesn't match. Try again on enter cancel\n";
                    std::cout << "Password: ";
                    std::cin >> password;
                    if (password == "cancel")
                    {
                        break;
                    }
                    std::cout << "Confirm: ";
                    std::cin >> confirmedPassword;
                }

                if (password == "cancel") // cancel option
                {
                    continue; // back to main authorization cycle
                }

                // now passwords match
                ctrl.newLogin(login, password); // add new login
                authorized = true;
            }
        }
        if (authorized)
        {
            std::cout << "Welcome: " << login << "\n";
            std::string mainMenuPrompt = "*you have returned to the main menu. 0 to see menu*\n";
            std::string menuAdminMessage =
                "Main menu. To choose a command enter the corresponding number:\n"
                "0. See this message\n"
                "1. Quit\n"
                "2. Logout\n"
                "3. Get information on quantity of sold and left in stock records of every record\n"
                "4. Get information on quantity of sold copies and the cost of given record for given period\n"
                "5. Get all the information of the most sold record\n"
                "6. Get the quantity of sold copies of the most popular artist\n"
                "7. Get the quantity of sold copies of the records and overall sum of money earned for a given artist\n"
                "8. Insert new values into tables\n"
                "9. Update tables\n"
                "10. Delete from tables\n"
                "11. Get information on delivered and sold copies of every record for given period\n"
                "12. Get information on sales of given record for given period\n";
            std::string menuUserMessage =
                "Main menu. To choose a command enter the corresponding number:\n"
                "0. See this message\n"
                "1. Quit\n"
                "2. Logout\n"
                "3. Get all the information of the most sold record\n"
                "4. Get the quantity of sold copies of the most popular artist\n"
                "5. Get information on sales of given record for given period\n";
            std::string requestPrompt = "Enter your request number: ";
            std::string separator = "------------------------------------------------------------------------------------\n";
            std::string intInputWarning =
                "A value of integer type must be entered\n";
            std::string warningAboutQuotations = "Don't forget to enclose string literals into single quotation marks('')\n";
            std::regex checkDate("\\d\\d\\d\\d-\\d\\d-\\d\\d");

            int request = 0;

            role = ctrl.getRole(login);
            std::string roleAdmin = "admin";
            std::string roleUser = "user";

            if (role == roleAdmin)
            {
                std::cout << menuAdminMessage;
            }
            else if (role == roleUser)
            {
                std::cout << menuUserMessage;
            }
            else
            {
                // undefined role = logout
                authorized = false;
                continue;
            }

            while (true) // main menu cycle
            {
                std::cout << requestPrompt;
                std::cin >> request;
                if (!ctrl.intInputGuard(std::cin)) // false if something went wrong when reading int
                {
                    std::cout << separator << intInputWarning << separator;
                    continue;
                }

                if (request == 0)
                {
                    // [admin menu/user menu]
                    // 0. See this message
                    if (role == roleAdmin)
                    {
                        std::cout << menuAdminMessage;
                    }
                    else if (role == roleUser)
                    {
                        std::cout << menuUserMessage;
                    }
                }
                else if (request == 1)
                {
                    // [admin menu/user menu] 1. Quit
                    return 0;
                }
                else if (request == 2)
                {
                    // [admin menu/user menu] 2. Logout
                    authorized = false;
                    break;
                }
                else if (request == 3 && role == roleAdmin)
                {
                    // [admin menu] 3. Get information on quantity of sold and left in stock records of every record
                    std::cout<< separator<< std::setw(40) << std::internal << "Name"<<"|Sold"<<"|Left\n";
                    std::vector<std::vector<std::string>> CurrentQuantityOfCD = ctrl.getCurrentQuantityOfCD();
                    for (int i = 0; i < CurrentQuantityOfCD.size(); i++)
                    {
                        std::cout<< std::setw(40) << std::internal 
                        <<CurrentQuantityOfCD[i][0]<< "|"
                        <<std::setw(4) <<CurrentQuantityOfCD[i][1]<< "|"
                        <<std::setw(4) <<CurrentQuantityOfCD[i][2] << "\n";
                    }
                    std::cout<< separator;
                }
                else if (request == 4 && role == roleAdmin)
                {
                    // [admin menu] 4. Get information on quantity of sold copies and the cost of given record for given period
                    bool data_checked = false;
                    std::string begining;
                    std::string ending;
                    while(!data_checked)
                    {
                        std::cout << "Enter the beginning of time interval in format YYYY-MM-DD: ";
                        std::cin >> begining;
                        if(std::regex_match(begining.c_str(),checkDate))
                        {
                            data_checked = true;
                        }
                        else
                        {
                            std::cout << "\nSomethig wrong,try again\n";
                        }
                    }
                    data_checked = false;
                    while(!data_checked)
                    {
                        std::cout << "Enter the ending of time interval: ";
                        std::cin >> ending;
                        if(std::regex_match(ending.c_str(),checkDate))
                        {
                            data_checked = true;
                        }
                        else
                        {
                            std::cout << "\nSomethig wrong,try again\n";
                        }
                    }
                    std::cout<< separator<< std::setw(40) << std::internal << "Name"<<"|Sold"<<"|Profit\n";
                    std::vector<std::vector<std::string>> QuantityOfCDPeriod = ctrl.getQuantityOfCDPeriod(begining,ending);
                    for (int i = 0; i < QuantityOfCDPeriod.size(); i++)
                    {
                        std::cout<< std::setw(40) << std::internal 
                        <<QuantityOfCDPeriod[i][0]<< "|"
                        <<std::setw(4) <<QuantityOfCDPeriod[i][1]<< "|"
                        <<std::setw(4) <<QuantityOfCDPeriod[i][2]<< "\n";
                    }
                    std::cout<< separator;
                    
                }
                else if ((request == 5 && role == roleAdmin) || (request == 3 && role == roleUser))
                {
                    std::cout << ctrl.getTheMostPopularCD();
                }
                else if ((request == 6 && role == roleAdmin) || (request == 4 && role == roleUser))
                {
                    std::cout << ctrl.getTheMostPopularArtist();
                }
                else if (request == 7 && role == roleAdmin)
                {
                    // [admin menu] 7. Get the quantity of sold copies of the records and overall sum of money for a given artist
                    std::cout << "Enter the artist name ";
                    std::string ArtistName;
                    std::cin.ignore();
                    getline(std::cin,ArtistName);
                    std::cout<< separator<< std::setw(15) << std::internal << "Name"<<"|Sold"<<"|Profit\n";
                    std::vector<std::string>InfoArtist = ctrl.getInfoArtist(ArtistName);
                    if(InfoArtist.size() > 0)
                    {
                    std::cout<< std::setw(15) << std::internal 
                        <<InfoArtist[0]<< "|"
                        <<std::setw(4) <<InfoArtist[1]<< "|"
                        <<std::setw(4) <<InfoArtist[2]<< "\n";
                    std::cout<< separator;
                    }
                    else
                    {
                        std::cout << "No info";
                    }
                }
                else if (request == 8 && role == roleAdmin)
                {
                    // [admin menu] 8. Insert new values into tables

                    std::string tableToInsert;
                    std::vector<std::string> columnsToInsert;
                    std::vector<std::vector<std::string>> valuesToInsert;

                    std::cout << separator << "Insert values into table: \n"
                              << separator;
                    {
                        std::string allTables = "List of all tables:\n";
                        std::string tableFirstInputPrompt = "Enter table where to insert values or enter cancel\n";
                        std::string tableNoSuchTablePrompt = "Unknown table. Try again or enter cancel\n";
                        std::string tableInputPrompt = "Table: ";

                        std::vector<std::string> tables = ctrl.getTables();

                        std::cout << allTables;
                        for (const auto &table : tables)
                        {
                            std::cout << table << ' ';
                        }
                        std::cout << "\n";
                        std::cout << separator;

                        std::cout << tableFirstInputPrompt;
                        std::cout << tableInputPrompt;
                        std::cin >> tableToInsert;
                        while (tableToInsert != "cancel" && !ctrl.validTable(tableToInsert, tables))
                        {
                            std::cout << tableNoSuchTablePrompt;
                            std::cout << tableInputPrompt;
                            std::cin >> tableToInsert;
                        }

                        if (tableToInsert == "cancel") // cancel option
                        {
                            std::cout << separator << mainMenuPrompt;
                            continue;
                        }

                        // now we have valid table
                    }
                    {
                        // fetching columns

                        int columnsNumber = -1;
                        std::string columnsNumberPrompt = "Enter number of columns you want to use. 0 stands for all of them except id column\n";
                        std::string columnsNumberInputPrompt = "Columns number: ";
                        std::string columnsNumberWrongInputPrompt = "Invalid columns number. Try again\n";
                        std::string columnPrompt = "Enter columns where to insert new values\n";
                        std::string columnInputPrompt = "Column: ";
                        std::string columnWrongInputPrompt = "Invalid column. Try again or enter cancel\n";
                        std::string allColumns = "List of all columns:\n";

                        std::vector<std::string> columns = ctrl.getColumns(tableToInsert);

                        std::cout << separator << allColumns;
                        for (const auto &column : columns)
                        {
                            std::cout << column << ' ';
                        }
                        std::cout << "\n";

                        std::cout << separator;
                        {
                            bool tried = false;
                            std::cout << columnsNumberPrompt;
                            while (columnsNumber != 0 && !ctrl.validColumnsCount(columnsNumber, columns))
                            {
                                if (tried)
                                {
                                    std::cout << columnsNumberWrongInputPrompt;
                                }
                                else
                                {
                                    tried = true;
                                }
                                std::cout << columnsNumberInputPrompt;
                                std::cin >> columnsNumber;
                                if (!ctrl.intInputGuard(std::cin))
                                {
                                    std::cout << separator << intInputWarning << separator;
                                    continue;
                                }
                            }
                        }

                        if (columnsNumber == 0)
                        {
                            for (size_t i = 1; i < columns.size(); ++i)
                            {
                                columnsToInsert.push_back(columns[i]);
                            }
                        }
                        else
                        {
                            std::string currentColumn;
                            bool canceled = false;

                            std::cout << separator;
                            std::cout << columnPrompt;
                            for (size_t i = 0; i < columnsNumber; ++i)
                            {
                                std::cout << columnInputPrompt;
                                std::cin >> currentColumn;
                                while (currentColumn != "cancel" && !ctrl.validColumn(currentColumn, columns))
                                {
                                    std::cout << columnWrongInputPrompt;
                                    std::cout << columnInputPrompt;
                                    std::cin >> currentColumn;
                                }

                                if (currentColumn == "cancel") // cancel option
                                {
                                    canceled = true;
                                    break;
                                }

                                // now we have valid column
                                columnsToInsert.push_back(currentColumn);
                            }

                            if (canceled) // cancel option
                            {
                                std::cout << separator << mainMenuPrompt;
                                continue;
                            }
                        }
                        // now we have valid columns to insert
                    }
                    {
                        // fetching values for columnsToInsert
                        int recordNumber = -1;
                        std::string recordPrompt = "Enter values for record ";
                        std::string columnValueInputPrompt = "Enter value for column ";
                        std::string columnValuePrompt = "Enter values for corresponging columns or cancel\n";
                        std::string recordNumberPrompt = "Record number: \n";
                        std::string recordNumberInputPrompt = "Enter number of records\n";
                        std::string recordNumberWrongInput = "Invalid number of records. Try again\n";

                        {
                            bool tried = false;
                            std::cout << separator;
                            std::cout << recordNumberInputPrompt;
                            while (recordNumber <= 0)
                            {
                                if (tried)
                                {
                                    std::cout << recordNumberWrongInput;
                                }
                                else
                                {
                                    tried = true;
                                }

                                std::cout << recordNumberPrompt;
                                std::cin >> recordNumber;
                                if (!ctrl.intInputGuard(std::cin))
                                {
                                    std::cout << separator << intInputWarning << separator;
                                    continue;
                                }
                            }
                        }

                        // now we have valid recordNumber
                        valuesToInsert.resize(recordNumber);
                        std::cout << separator;
                        std::cout << columnValuePrompt;
                        std::cout << warningAboutQuotations;
                        {
                            bool canceled = false;
                            for (size_t i = 0; i < recordNumber; ++i)
                            {
                                if (canceled)
                                {
                                    break;
                                }
                                std::cout << recordPrompt << i << ":\n";
                                for (size_t j = 0; j < columnsToInsert.size(); ++j)
                                {
                                    std::string currentValue;
                                    std::cout << "\t" << columnValueInputPrompt << columnsToInsert[j] << ": ";
                                    std::cin >> currentValue;

                                    if (currentValue == "cancel")
                                    {
                                        canceled = true;
                                        break;
                                    }

                                    valuesToInsert[i].push_back(currentValue);
                                }
                            }

                            if (canceled)
                            {
                                std::cout << separator << mainMenuPrompt;
                                continue;
                            }
                        }

                        // now we have valuesToInsert
                    }

                    std::cout << separator;
                    if (!ctrl.createInsertQuery(tableToInsert, columnsToInsert, valuesToInsert))
                    {
                        std::cout << "Your request failed\n";
                    }
                    else
                    {
                        std::cout << "Your request was successful\n";
                    }

                    std::cout << "Returning to main menu\n";
                    std::cout << separator << mainMenuPrompt;
                }
                else if (request == 9 && role == roleAdmin)
                {
                    // [admin menu] 9. Update tables
                    std::cout << separator;
                    std::cout << "Update table:\n";

                    std::vector<std::string> tables = ctrl.getTables();

                    std::cout << separator;
                    std::cout << "List of all tables: \n";
                    for (const auto &table : tables)
                    {
                        std::cout << table << ' ';
                    }
                    std::cout << "\n";

                    std::string tableToUpdate;

                    std::cout << separator << "Choose a table or enter cancel\n";
                    std::cout << "Table: ";
                    std::cin >> tableToUpdate;

                    while (tableToUpdate != "cancel" && !ctrl.validTable(tableToUpdate, tables))
                    {
                        std::cout << "Unknown table. Try again or enter cancel\n";
                        std::cout << "Table: ";
                        std::cin >> tableToUpdate;
                    }

                    if (tableToUpdate == "cancel")
                    {
                        // return to main admin menu
                        std::cout << separator << mainMenuPrompt;
                        continue;
                    }

                    // valid table received

                    std::cout << separator;
                    std::vector<std::string> columns = ctrl.getColumns(tableToUpdate);

                    std::cout << "List of columns of " + tableToUpdate + ":\n";
                    for (const auto &column : columns)
                    {
                        std::cout << column << " ";
                    }
                    std::cout << "\n";

                    int columnsCount = 0;
                    std::string currentColumn;
                    std::string columnNumberPrompt = "Enter number of columns you want to change: ";

                    std::cout << separator;
                    {
                        bool tried = false; // true if already failed once to input columnsCount
                        while (!ctrl.validColumnsCount(columnsCount, columns))
                        {
                            if (tried)
                            {
                                std::cout << "Number of columns must be in range (1, " << columns.size() << "). Try again\n";
                            }
                            else
                            {
                                tried = true;
                            }
                            std::cout << columnNumberPrompt;
                            std::cin >> columnsCount;
                            if (!ctrl.intInputGuard(std::cin))
                            {
                                columnsCount = 0;
                                std::cout << separator << intInputWarning << separator;
                                continue;
                            }
                        }
                    }

                    // valid columnsCount

                    std::vector<std::string> columnsToUpdate;
                    std::string columnPromptWithCancel = "Enter column for processing or cancel\n";
                    std::string columnPrompt = "Column: ";
                    bool canceled = false;

                    std::cout << separator;
                    for (size_t i = 0; i < columnsCount; ++i)
                    {
                        if (canceled)
                        {
                            break;
                        }

                        std::cout << columnPromptWithCancel;
                        std::cout << columnPrompt;
                        std::cin >> currentColumn;

                        while (currentColumn != "cancel" && !ctrl.validColumn(currentColumn, columns))
                        {
                            std::cout << "No such column. Try again or enter cancel\n";
                            std::cout << columnPrompt;
                            std::cin >> currentColumn;
                        }

                        if (currentColumn == "cancel") // cancel option
                        {
                            canceled = true;
                            break;
                        }

                        // valid column received
                        columnsToUpdate.push_back(currentColumn);
                    }

                    if (canceled) // was canceled inside a cycle
                    {
                        std::cout << separator << mainMenuPrompt;
                        continue;
                    }

                    // valid columnsToUpdate

                    // now trying to fetch new values for selected columns

                    std::vector<std::string> valuesForColumns;
                    std::string valuePromptWithCancel = "Enter values for corresponding columns or cancel\n";
                    std::string valuePrompt = "Value: ";
                    std::string currentValue;

                    std::cout << separator;
                    std::cout << valuePromptWithCancel;
                    std::cout << warningAboutQuotations;
                    for (size_t i = 0; i < columnsCount; ++i)
                    {
                        if (canceled)
                        {
                            break;
                        }
                        std::cout << "Current column: " << columnsToUpdate[i] << "\n";
                        std::cout << valuePrompt;
                        std::cin >> currentValue;

                        if (currentValue == "cancel")
                        {
                            canceled = true;
                            continue;
                        }

                        valuesForColumns.push_back(currentValue);
                    }

                    if (canceled) // was canceled in previous cycle
                    {
                        // returning to main menu
                        std::cout << separator << mainMenuPrompt;
                        continue;
                    }

                    // now we have valid values for corresponding columns

                    // all we need is where clause

                    std::cout << separator;
                    std::cout << "List of all columns: \n";

                    for (const auto &column : columns)
                    {
                        std::cout << column << ' ';
                    }
                    std::cout << "\n";

                    std::string wherePrompt = "Now enter your where condition (without the word \"where\") or cancel\n";
                    std::string whereCondition;

                    std::cout << separator;
                    std::cout << wherePrompt;
                    std::cout << warningAboutQuotations;

                    std::cin.ignore();
                    std::getline(std::cin, whereCondition);
                    if (whereCondition == "cancel")
                    {
                        // returning to main menu
                        std::cout << separator << mainMenuPrompt;
                        continue;
                    }

                    // we are all set, passing data to createUpdateQuery method in controller

                    if (ctrl.createUpdateQuery(tableToUpdate, columnsToUpdate, valuesForColumns, whereCondition))
                    {
                        std::cout << "Your request was successful\n"
                                  << separator << mainMenuPrompt;
                        continue;
                    }
                    else
                    {
                        std::cout << "Something went wrong during with your request. Aborting\n";
                        break;
                    }
                }
                else if (request == 10 && role == roleAdmin)
                {
                    // [admin menu] 10. Delete from tables

                    std::cout << separator;
                    std::cout << "Delete from table:\n";
                    std::string tableToDeleteFrom;
                    std::string whereCondition;
                    {
                        std::string tableInputPrompt = "Table: ";
                        std::string tableWrongInputPrompt = "Unknown table. Try again on enter cancel\n";
                        std::string tableFirstPromt = "Enter table name or cancel\n";

                        std::string allTables = "List of all tables\n";
                        std::vector<std::string> tables = ctrl.getTables();

                        std::cout << separator;
                        std::cout << allTables;
                        for (const auto &table : tables)
                        {
                            std::cout << table << ' ';
                        }
                        std::cout << separator;

                        std::cout << tableFirstPromt;
                        std::cout << tableInputPrompt;
                        std::cin >> tableToDeleteFrom;
                        while (tableToDeleteFrom != "cancel" && !ctrl.validTable(tableToDeleteFrom, tables))
                        {
                            std::cout << tableWrongInputPrompt;
                            std::cout << tableInputPrompt;
                            std::cin >> tableToDeleteFrom;
                        }

                        if (tableToDeleteFrom == "cancel") // cancel option
                        {
                            std::cout << separator << mainMenuPrompt;
                            continue;
                        }

                        // we have valid table name
                    }
                    // as we have valid table, all we need is where condition
                    {
                        std::string whereConditionPrompt = "Enter where condition(without the word \"where\") or cancel\n";
                        std::string warningAboutQuotationMarks = "Don't forget to enclose string literals into single quotation marks('')\n";
                        std::string whereConditionInputPrompt = "Where condition: ";

                        std::cout << whereConditionPrompt;
                        std::cout << warningAboutQuotationMarks;
                        std::cout << whereConditionInputPrompt;
                        std::cin.ignore();
                        getline(std::cin, whereCondition);

                        if (whereCondition == "cancel") // cancel option
                        {
                            std::cout << separator << mainMenuPrompt;
                            continue;
                        }

                        // we are left with valid where condition
                    }

                    if (ctrl.createDeleteQuery(tableToDeleteFrom, whereCondition))
                    {
                        std::cout << "Your query was successful. Returning to main menu\n";
                        std::cout << separator << mainMenuPrompt;
                        continue;
                    }
                    else
                    {
                        std::cout << "Your query failed. Returning to main menu\n";
                        std::cout << separator << mainMenuPrompt;
                        continue;
                    }
                }
                else if (request == 11 && role == roleAdmin)
                {
                    // [admin menu] 11. Get information on delivered and sold copies of every record for given period
                    bool data_checked = false;
                    std::string begining;
                    std::string ending;
                    while(!data_checked)
                    {
                        std::cout << "Enter the beginning of time interval in format YYYY-MM-DD: ";
                        std::cin >> begining;
                        if(std::regex_match(begining.c_str(),checkDate))
                        {
                            data_checked = true;
                        }
                        else
                        {
                            std::cout << "\nSomethig wrong,try again\n";
                        }
                    }
                    data_checked = false;
                    while(!data_checked)
                    {
                        std::cout << "Enter the ending of time interval: ";
                        std::cin >> ending;
                        if(std::regex_match(ending.c_str(),checkDate))
                        {
                            data_checked = true;
                        }
                        else
                        {
                            std::cout << "\nSomethig wrong,try again\n";
                        }
                    }
                    std::cout<< separator<< std::setw(61) << std::internal << "Name"<<"|Sold"<<"|Delivered\n";
                    std::vector<std::vector<std::string>> QuantityDeliveredSoldCDPeriod = ctrl.getQuantityDeliveredSoldCDPeriod(begining,ending);
                    for (int i = 0; i < QuantityDeliveredSoldCDPeriod.size(); i++)
                    {
                        std::cout<< std::setw(61) << std::internal 
                        <<QuantityDeliveredSoldCDPeriod[i][0]<< "|"
                        <<std::setw(4) <<QuantityDeliveredSoldCDPeriod[i][1]<< "|"
                        <<std::setw(4) <<QuantityDeliveredSoldCDPeriod[i][2]<< "\n";
                    }
                    std::cout<< separator;
                }
                else if ((request == 12 && role == roleAdmin) || (request == 5 && role == roleUser))
                {
                    // [admin menu] 12.|[user menu] 5. Get information on sales of given record for given period
                    bool data_checked = false;
                    std::string begining;
                    std::string ending;
                    while(!data_checked)
                    {
                        std::cout << "Enter the beginning of time interval in format YYYY-MM-DD: ";
                        std::cin >> begining;
                        if(std::regex_match(begining.c_str(),checkDate))
                        {
                            data_checked = true;
                        }
                        else
                        {
                            std::cout << "\nSomethig wrong,try again\n";
                        }
                    }
                    data_checked = false;
                    while(!data_checked)
                    {
                        std::cout << "Enter the ending of time interval: ";
                        std::cin >> ending;
                        if(std::regex_match(ending.c_str(),checkDate))
                        {
                            data_checked = true;
                        }
                        else
                        {
                            std::cout << "\nSomethig wrong,try again\n";
                        }
                    }
                    std::cout << "Enter the disk_id ";
                    std::string disk_id;
                    std::cin.ignore();
                    getline(std::cin,disk_id);
                    std::cout<< separator<< std::setw(15) << std::internal << "Name"<<"|Sold"<<"|Profit\n";
                    std::vector InfoCDPeriod = ctrl.getInfoCDPeriod(begining,ending,disk_id);
                    if(InfoCDPeriod.size() > 0)
                    {
                    std::cout<< std::setw(15) << std::internal 
                        <<InfoCDPeriod[0]<< "|"
                        <<std::setw(4) <<InfoCDPeriod[1]<< "|"
                        <<std::setw(4) <<InfoCDPeriod[2]<< "\n";
                    std::cout<< separator;
                    }
                    else
                    {
                        std::cout << "no info";
                    }
                    
                }
                else
                {
                    // [admin menu/user menu] unrecognized option, printing the menu
                    std::cout << "Unrecognized option. Enter 0 to see menu\n";
                    // if (role == roleAdmin)
                    // {
                    //     std::cout << menuAdminMessage;
                    // }
                    // else if (role == roleUser)
                    // {
                    //     std::cout << menuUserMessage;
                    // }
                }
                std::cout << "\n";
            }
        }
    }
    return 0;
}