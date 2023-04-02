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

    while (!ctrl.validResponseAuth(response)) // waiting for valid response
    {
        std::cout << "You are currently unauthorized. Enter sign-in or sign-up\n";
        std::cin >> response;
    }

    if (response == "sign-in")
    {
        while(login.empty() || !ctrl.existingLogin(login))
        {
            std::cout << "There is no such login";
            std::cin >> login;
        }

    }
    else if (response == "sign-up")
    {

    }
    return 0;
}