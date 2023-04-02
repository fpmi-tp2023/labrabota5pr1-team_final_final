#include <iostream>
#include "controller.h"

int main()
{
    bool authorized = false;
    const std::string dbFileName = "RecordStore.db";
    Controller ctrl;
    std::string response;
    std::string login;

    if (!ctrl.connectDB(dbFileName))
    {
        std::cerr << "Something went wrong when opening the database\n";
        return 1;
    }

    while (true)
    {
        std::cout << "You are currently unauthorized. Enter sign-in or sign-up\n";
        std::cin >> response;
        while (!ctrl.validResponseAuth(response)) // waiting for valid response
        {
            std::cout << "No such option as " << response << ". Enter sign-in or sign-up\n";
            std::cin >> response;
        }

        if (response == "sign-in")
        {
            std::cout << "Login: ";
            std::cin >> login;
            std::cout << std::endl;

            while (login != "quit" && (login.empty() || !ctrl.existingLogin(login))) // optional quit or waiting for valid login
            {
                std::cout << "There is no such login\n";
                std::cout << "Enter quit or existing login\n";
                std::cin >> login;
            }

            if (login == "quit") // option quit
            {
                continue; // return to autorization choice stage
            } 

            // valid login
            std::string password;
            int wrongPasswordAttempts = 3;

            std::cout << "Password: ";
            std::cin >> password;
            while(!ctrl.correctPassword(login, password))
            {
                --wrongPasswordAttempts;
                std::cout << "Incorrect password. Try again\n";
                std::cout << "Password: ";
                std::cin >> password;
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
            
            
        }
        else if (response == "sign-up")
        {
        }
    }
    return 0;
}