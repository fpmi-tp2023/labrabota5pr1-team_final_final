#include <iostream>
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
        std::string response = "";
        std::string role = "";
        std::string login = "";

        if (!authorized)
        {
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
                "9. Delete tables\n"
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
            role = ctrl.getRole(login);
            if (role == "admin")
            {
                int request = 0;

                std::cout << menuAdminMessage;
                while (true) // main admin menu cycle
                {
                    std::cout << requestPrompt;
                    std::cin >> request;
                    if (request == 0)
                    {
                        // [admin menu] 0. See this message
                        std::cout << menuAdminMessage;
                    }
                    else if (request == 1)
                    {
                        // [admin menu] 1. Quit
                        break;
                    }
                    else if (request == 2)
                    {
                        // [admin menu] 2. Logout
                        authorized = false;
                        continue;
                    }
                    else if (request == 3)
                    {
                        // [admin menu] 3. Get information on quantity of sold and left in stock records of every record
                    }
                    else if (request == 4)
                    {
                        // [admin menu] 4. Get information on quantity of sold copies and the cost of given record for given period
                    }
                    else if (request == 5)
                    {
                        // [admin menu] 5. Get all the information of the most sold record
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
                    }
                    else if (request == 9)
                    {
                        // [admin menu] 9. Delete tables
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
                        // [admin menu] unrecognized option, printing the menu
                        std::cout << menuAdminMessage;
                    }
                    std::cout << "\n";
                }
            }
            else if (role == "user")
            {
                int request = 0;

                std::cout << menuUserMessage;
                while (true) // main user menu cycle
                {
                    std::cout << requestPrompt;
                    std::cin >> request;
                    if (request == 0)
                    {
                        // [user menu] 0. See this message
                        std::cout << menuAdminMessage;
                    }
                    else if (request == 1)
                    {
                        // [user menu] 1. Quit
                        break;
                    }
                    else if (request == 2)
                    {
                        // [user menu] 2. Logout
                        authorized = false;
                        continue;
                    }
                    else if (request == 3)
                    {
                        // [user menu] 3. Get all the information of the most sold record
                    }
                    else if (request == 4)
                    {
                        // [user menu] 4. Get the quantity of sold copies of the most popular artist
                    }
                    else if (request == 5)
                    {
                        // [user menu] 5. Get information on sales of given record for given period
                    }
                    else
                    {
                        // unrecognized option, printing the menu
                        std::cout << menuUserMessage;
                    }
                    std::cout << "\n";
                }
            }
            else
            {
                // undefined role = logout
                authorized = false;
            }
        }
    }
    return 0;
}