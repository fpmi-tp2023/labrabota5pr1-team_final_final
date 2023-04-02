#include "controller.h"

Controller::Controller()
{

}
Controller::~Controller()
{
    if (dbModel)
    {
        delete dbModel;
        dbModel = nullptr;
    }
}