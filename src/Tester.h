//
// Created by Ace Jones on 10/6/26.
//

#ifndef TESTER_TESTER_H
#define TESTER_TESTER_H

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>

namespace Tester {
    class Test {
        public:
            Test() = default;
            virtual ~Test() = default;

            ImFont* SourceSans = nullptr;

            int Iterate() {
                ImGui_ImplSDLRenderer3_NewFrame();
                ImGui_ImplSDL3_NewFrame();
                ImGui::NewFrame();

                // Styles
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.0f));
                ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImColor(255, 255, 255, 255).Value);
                ImGui::PushStyleColor(ImGuiCol_Text, ImColor(0, 0, 0, 255).Value);

                // Draw UI
                // Draw Main App Window
                int CurrentColor = 0; // 0 = white, 1 = black, 2 = Blue, 3 = Red, 4 = Orange, 5 = Yellow, 6 = Green, 7 = Indigo, 8 = Violet
                ImGui::PushFont(SourceSans);
                ImGui::Begin("Reflex Tester", nullptr,
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse); // ImGuiWindowFlags_NoDecoration |
                ImGui::PopStyleVar(3);
                ImGui::PopStyleColor(2);

                // Content
                ImGui::Text("Test Your Reflexes");





                ImGui::End();
                ImGui::PopFont();

                ImGui::Render();

                return 0;
            }

    };
}

#endif //TESTER_TESTER_H
