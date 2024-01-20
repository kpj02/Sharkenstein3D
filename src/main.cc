#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <stack>
#include <memory>
#include "Game.h"
#include "State.h"
#include "Program.h"

int main()
{   
    // The entiere program is represented by an instance of the Program class
    Program prog_example {};

    // Run the main program loop
    prog_example.start();
    return 0;
}
