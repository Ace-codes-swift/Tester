//
// Created by Ace Jones on 10/6/26.
//

#ifndef TESTER_TESTER_H
#define TESTER_TESTER_H

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"
#include "Result.h"
#include "supabase.h"
#include <SDL3/SDL.h>
#include <string>
#include <random>
#include <chrono>
#include <format>
#include <iostream>

namespace Tester {
    class Test {
    public:
        Test() = default;
        virtual ~Test() = default;

        ImFont* SourceSans = nullptr;

        Tester::Supabase sender;

        int Iterate() {
            // Setup time for recording data
            // Get the current time point from the system clock
            auto now = std::chrono::system_clock::now();
            
            // Format directly as an ISO 8601 UTC string (with fractional seconds)
            // %FT%TZ translates to YYYY-MM-DDTHH:MM:SSZ
            std::string iso_time = std::format("{:%FT%TZ}", now);


            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            ImGuiViewport* viewport = ImGui::GetMainViewport();

            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);

            ImGui::PushStyleVar(
                ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(
                ImGuiStyleVar_WindowBorderSize, 0.0f);

            ImGui::Begin("Reflex Tester", nullptr,
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoTitleBar);

            // The colors that will be tested.
            static const char* Colors[] = {
                "Red", "Orange", "Yellow", "Green",
                "Blue", "Indigo", "Violet", "White"
            };

            static ImVec4 ColorValues[] = {
                ImVec4(1, 0, 0, 1),
                ImVec4(1, 0.4f, 0, 1),
                ImVec4(1, 1, 0, 1),
                ImVec4(0, 0.8f, 0, 1),
                ImVec4(0, 0.3f, 1, 1),
                ImVec4(0.29f, 0, 0.5f, 1),
                ImVec4(0.6f, 0, 0.8f, 1),
                ImVec4(1, 1, 1, 1)
            };

            enum Page {
                START,
                COUNTDOWN,
                TESTING,
                FINISHED
            };

            static Page page = START;
            static int colorNumber = 0;
            static Uint64 countdownStart = 0;
            static Uint64 colorStart = 0;
            static int lastTime = 0;

            // Only one result is needed for the current session.
            static Result currentResult;

            static int order[] = {
                0, 1, 2, 3, 4, 5, 6, 7
            };

            static std::mt19937 random(
                static_cast<unsigned int>(SDL_GetTicks()));

            bool clicked =
                ImGui::IsMouseClicked(ImGuiMouseButton_Left);

            // Change the background when a color is being tested.
            if (page == TESTING) {
                ImGui::PushStyleColor(
                    ImGuiCol_WindowBg,
                    ColorValues[order[colorNumber]]);
            } else {
                ImGui::PushStyleColor(
                    ImGuiCol_WindowBg,
                    ImVec4(0.92f, 0.92f, 0.92f, 1));
            }

            ImGui::PushStyleColor(
                ImGuiCol_Text,
                ImVec4(0, 0, 0, 1));

            // Start page
            if (page == START) {
                ImGui::SetCursorPosY(
                    ImGui::GetWindowHeight() / 2 - 90);

                ImGui::SetCursorPosX(20);

                ImGui::SetWindowFontScale(2.0f);
                ImGui::Text("Reaction Speed Tester");

                ImGui::SetWindowFontScale(1.0f);

                ImGui::Spacing();
                ImGui::Text(
                    "See how fast you can react to colors!");

                ImGui::Spacing();
                ImGui::Text("There are 8 colors to test.");

                ImGui::Spacing();
                ImGui::Text("Program written by Ace Jones");

                ImGui::Text(
                    "Group: Ace Jones, Sam O'Connor, Chevy Blalock, and");

                ImGui::Text("Charlotte Williams");

                ImGui::Spacing();

                if (ImGui::Button(
                    "Tap to Begin", ImVec2(200, 60))) {

                    // Make a new result for this session.
                    currentResult = Result{};

                    currentResult.session_time =
                        "Session " +
                        iso_time;

                    colorNumber = 0;

                    // Randomize the color order.
                    for (int i = 7; i > 0; i--) {
                        std::uniform_int_distribution<int> pick(0, i);

                        int j = pick(random);

                        int temp = order[i];
                        order[i] = order[j];
                        order[j] = temp;
                    }

                    countdownStart = SDL_GetTicks();
                    page = COUNTDOWN;
                }
            }

            // Countdown page
            else if (page == COUNTDOWN) {
                ImGui::SetCursorPosY(
                    ImGui::GetWindowHeight() / 2 - 60);

                ImGui::SetWindowFontScale(1.5f);
                ImGui::Text("Get Ready!");

                ImGui::SetWindowFontScale(1.0f);

                ImGui::Spacing();

                ImGui::Text(
                    "Next color: %s",
                    Colors[order[colorNumber]]);

                int secondsPassed =
                    static_cast<int>(
                        (SDL_GetTicks() - countdownStart) / 1000);

                int secondsLeft = 3 - secondsPassed;

                if (secondsLeft > 0) {
                    ImGui::Text(
                        "Starting in %d...", secondsLeft);
                } else {
                    page = TESTING;
                    colorStart = SDL_GetTicks();
                }
            }

            // Color test page
            else if (page == TESTING) {
                // Record the reaction time when the player clicks.
                if (clicked) {
                    lastTime = static_cast<int>(
                        SDL_GetTicks() - colorStart);

                    switch (order[colorNumber]) {
                        case 0:
                            currentResult.red = lastTime;
                            break;

                        case 1:
                            currentResult.orange = lastTime;
                            break;

                        case 2:
                            currentResult.yellow = lastTime;
                            break;

                        case 3:
                            currentResult.green = lastTime;
                            break;

                        case 4:
                            currentResult.blue = lastTime;
                            break;

                        case 5:
                            currentResult.indigo = lastTime;
                            break;

                        case 6:
                            currentResult.violet = lastTime;
                            break;

                        case 7:
                            currentResult.white = lastTime;
                            break;
                    }

                    colorNumber++;

                    if (colorNumber >= 8) {
                        // Send this completed test session to Supabase.
                        sender.SendData(currentResult);

                        page = FINISHED;
                    } else {
                        countdownStart = SDL_GetTicks();
                        page = COUNTDOWN;
                    }
                }
            }

            // Results page
            else if (page == FINISHED) {
                ImGui::SetWindowFontScale(1.7f);
                ImGui::Text("Test Finished!");

                ImGui::SetWindowFontScale(1.0f);

                ImGui::Spacing();
                ImGui::Text("Your reaction times:");

                ImGui::Text(
                    "Red: %d ms", currentResult.red);

                ImGui::Text(
                    "Orange: %d ms", currentResult.orange);

                ImGui::Text(
                    "Yellow: %d ms", currentResult.yellow);

                ImGui::Text(
                    "Green: %d ms", currentResult.green);

                ImGui::Text(
                    "Blue: %d ms", currentResult.blue);

                ImGui::Text(
                    "Indigo: %d ms", currentResult.indigo);

                ImGui::Text(
                    "Violet: %d ms", currentResult.violet);

                ImGui::Text(
                    "White: %d ms", currentResult.white);

                ImGui::Spacing();

                if (ImGui::Button(
                    "Test Again", ImVec2(200, 60))) {
                    page = START;
                }
            }

            ImGui::PopStyleColor(2);

            ImGui::End();

            ImGui::PopStyleVar(2);

            ImGui::Render();

            return 0;
        }
    };
}

#endif // TESTER_TESTER_H