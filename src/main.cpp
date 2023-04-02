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
    std::string response;
    while (!ctrl.validResponseAuth(response))
    {
        std::cout << "You are currently unauthorized. Options are sign-in or sign-up";
        std::cin >> response;
    }
    return 0;
}