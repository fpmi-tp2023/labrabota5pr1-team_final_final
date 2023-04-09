#include <iostream>
#include <vector>
#include "controller.h"

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
            std::string menuAdminMessage =
                "Main menu. To choose a command enter the corresponding number:\n"
                "0. See this message\n"
                "1. Quit\n"
                "2. Logout\n"
                "3. Get information on quantity of sold and left in stock records of every record\n"
                "4. Get information on quantity of sold copies and the cost of given record for given period\n"
                "5. Get all the information of the most sold record\n"
                "6. Get the quantity of sold copies of the most popular artist\n"
                "7. Insert new values into tables\n"
                "8. Update tables\n"
                "9. Delete from tables\n"
                "10. Get information on delivered and sold copies of every record for given period\n"
                "11. Get information on sales of given record for given period\n";
            std::string menuUserMessage =
                "Main menu. To choose a command enter the corresponding number:\n"
                "0. See this message\n"
                "1. Quit\n"
                "2. Logout\n"
                "3. Get all the information of the most sold record\n"
                "4. Get the quantity of sold copies of the most popular artist\n"
                "5. Get information on sales of given record for given period\n";
            std::string requestPrompt = "Enter your request number: ";
            std::string separator = "----------------------------\n";
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
                else if (request == 3)
                {
                    if (role == roleAdmin)
                    {
                        // [admin menu] 3. Get information on quantity of sold and left in stock records of every record
                    }
                    else if (role == roleUser)
                    {
                        // [user menu] 3. Get all the information of the most sold record
                    }
                }
                else if (request == 4)
                {
                    if (role == roleAdmin)
                    {
                        // [admin menu] 4. Get information on quantity of sold copies and the cost of given record for given period
                    }
                    else if (role == roleUser)
                    {
                        // [user menu] 4. Get the quantity of sold copies of the most popular artist
                    }
                }
                else if (request == 5)
                {
                    if (role == roleAdmin)
                    {
                        // [admin menu] 5. Get all the information of the most sold record
                    }
                    else if (role == roleUser)
                    {
                        // [user menu] 5. Get information on sales of given record for given period
                    }
                }
                else if (request == 6)
                {
                    // [admin menu] 6. Get the quantity of sold copies of the most popular artist
                }
                else if (request == 7)
                {
                    // [admin menu] 7. Insert new values into tables
                }
                else if (request == 8)
                {
                    // [admin menu] 8. Update tables
                    std::string response;
                    int requestNumber = -1;
                    std::string requestPrompt =
                        "Enter corresponding request number:\n"
                        "0: return to menu\n"
                        "1: default sql update query (with input prompts)\n";
                    std::string requestInputPrompt = 
                        "Request: ";
                    std::string mainMenuPrompt =
                        "*you have returned to the main menu. 0 to see menu*\n";

                    std::cout << separator;
                    std::cout << "Update table:\n"; 
                    std::cout << requestPrompt << requestInputPrompt;

                    std::cin >> requestNumber;
                    while (!ctrl.validUpdateRequestNumber(requestNumber))
                    {
                        std::cout << "invalid request number. Try again: ";
                        std::cin >> requestNumber;
                    }

                    // valid request number received

                    if (requestNumber == 0)
                    {
                        // return to main admin menu
                        std::cout << separator << mainMenuPrompt;
                        continue;
                    }
                    else if (requestNumber == 1)
                    {
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
                        std::cout << columnNumberPrompt;
                        std::cin >> columnsCount;

                        while (!ctrl.validColumnsCount(columnsCount, columns))
                        {
                            std::cout << "Number of columns must be in range (1, " << columns.size() << "). Try again\n";
                            std::cout << columnNumberPrompt;
                            std::cin >> columnsCount;
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
                        std::string warningAboutQuotations = "Don't forget to enclose string literals into single quotation marks('')\n";
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

                        std::cin >> whereCondition;
                         if (whereCondition == "cancel")
                        {
                            // returning to main menu
                            std::cout << separator << mainMenuPrompt;
                            continue;
                        }

                        // we are all set, passing data to createUpdateQuery method in controller

                        ctrl.createUpdateQuery(tableToUpdate, columnsToUpdate, valuesForColumns, whereCondition);
                    }
                }
                else if (request == 9)
                {
                    // [admin menu] 9. Delete from tables
                }
                else if (request == 10)
                {
                    // [admin menu] 10. Get information on delivered and sold copies of every record for given period
                }
                else if (request == 11)
                {
                    // [admin menu] 11. Get information on sales of given record for given period
                }
                else
                {
                    // [admin menu/user menu] unrecognized option, printing the menu
                    if (role == roleAdmin)
                    {
                        std::cout << menuAdminMessage;
                    }
                    else if (role == roleUser)
                    {
                        std::cout << menuUserMessage;
                    }
                }
                std::cout << "\n";
            }
        }
    }
    return 0;
}