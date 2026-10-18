//
// Created by Ace Jones on 10/2/26.
//

#ifndef TESTER_WINDOW_H
#define TESTER_WINDOW_H


#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>

#include "Tester.h"

namespace Tester {
    class Window {
        Tester::Test app;

        public:
        Window() = default;
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        ~Window()
        {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            renderer = nullptr;
            window = nullptr;
            SDL_Quit();
        }



        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        


        bool Init()
        {
            SDL_SetAppMetadata(
                "Tester Application",
                nullptr,
                nullptr
            );

            if (!SDL_Init(SDL_INIT_VIDEO)) {
                SDL_Log(
                    "Couldn't initialize SDL: %s",
                    SDL_GetError()
                );

                return false;
            }

            if (!SDL_CreateWindowAndRenderer(
                "Tester Application",
                1920,
                1080,
                SDL_WINDOW_RESIZABLE,
                &window,
                &renderer
            )) {
                SDL_Log(
                    "Couldn't create window/renderer: %s",
                    SDL_GetError()
                );
                return false;
            }
            SDL_SetWindowFullscreen(window, true);


            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            //ImGui::StyleColorsLight();

            // Setup scaling

            ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
            ImGui_ImplSDLRenderer3_Init(renderer);
            ImGuiFreeTypeBuilderSettings freetype_settings;
            freetype_settings.Weight = 700.0f; // Adjust 'wght' axis (e.g., 100 to 900)



            // 2. Configure ImFontConfig
            ImFontConfig cfg;
            cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_LightHinting; // Choose flags as needed
            cfg.FontBuilderSettings = &freetype_settings;
            app.SourceSans = io.Fonts->AddFontFromFileTTF("assets/fonts/SourceSans.ttf", 28.0f, &cfg);

            return true;

        }

        bool Event(const SDL_Event& event)
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                return false;
            }

            return true;
        }

        void Iterate()
        {
            app.Iterate();
            SDL_SetRenderDrawColorFloat(
                renderer,
                0,
                0,
                0,
                SDL_ALPHA_OPAQUE_FLOAT
            );

            SDL_RenderClear(renderer);
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
            SDL_RenderPresent(renderer);
        }

        void Quit()
        {
            ImGui_ImplSDLRenderer3_Shutdown();
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();

            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);

            renderer = nullptr;
            window = nullptr;

            SDL_Quit();
        }

    };
}

#endif //TESTER_WINDOW_H
