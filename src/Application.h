//
// Created by Ace Jones on 10/1/26.
//

#ifndef CLIONTEST_APPLICATION_H
#define CLIONTEST_APPLICATION_H

#include "Window.h"



namespace Tester {

class Application {
    Tester::Window SDL;
public:
    Application() = default;
    virtual ~Application() = default;

    void run()
    {

        if (!SDL.Init()) {
            SDL.Quit();
            return;
        }

        bool running = true;

        while (running) {

            SDL_Event event;

            while (SDL_PollEvent(&event)) {

                if (!SDL.Event(event)) {
                    running = false;
                }
            }

            if (running) {
                SDL.Iterate();
            }
        }

        Shutdown();
    }

private:
    virtual void Shutdown()
    {
        SDL.Quit(); //hello???? shutdown right here
    }


};

}

#endif