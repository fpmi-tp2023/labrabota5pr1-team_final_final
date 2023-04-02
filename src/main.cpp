#include <iostream>
#include "controller.h"

int main()
{
    const std::string dbFileName = "RecordStore.db";
    Controller ctrl;
    if (ctrl.connectDB(dbFileName))
    {
        std::cout << "Database opened\n";
    }
    else
    {
        std::cout << "Something went wrong\n";
    }
    return 0;
}