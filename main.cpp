#include "simcontrol.h"

#include <memory>

int main()
{
    std::unique_ptr<simcontrol> controller;

    controller = std::make_unique<simcontrol>(0.0, 4000.0, 0.0); 

    
    return 0;
}