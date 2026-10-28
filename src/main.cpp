#include <iostream>
#include "Application.h"
#include <SDL3/SDL_main.h>


// Main Loop
int main() {
    // Init Main Application
    Tester::Application app;
    
    // Run the App!
    app.run();

    // Return Success on exit
    return 0;
}