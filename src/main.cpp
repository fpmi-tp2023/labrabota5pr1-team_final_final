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
                while(login != "cancel" && ctrl.existingLogin(login)) // wait for cancel or new login
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
                while(password != "cancel" && !ctrl.passwordsMatch(password, confirmedPassword)) // wait for cancel or passwords to match
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
            std::string menuMessage;
            role = ctrl.getRole(login);
            if (role == "admin")
            {
                while (true) // main admin menu cycle
                {
                    authorized = false;
                    break;
                }
            }
            else if (role == "user")
            {
                while (true) // main user menu cycle
                {
                    authorized = false;
                    break;
                }
            }
        }
    }
    return 0;
}