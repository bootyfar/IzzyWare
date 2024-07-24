#include "main.h"
#include "settings.h"
#include "font_awesome.h"
#include "fonts.h"
#include "images.h"
#include "particles.h"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <windows.h> // Include for Windows API functions

#include "auth.hpp"
#include "skStr.h"

Framework::CUSTOM c;
Framework::GUI g;

using namespace KeyAuth;

// Fill this up with your credentials
std::string name = skCrypt("IzzyWare").decrypt(); // Application Name
std::string ownerid = skCrypt("72zjngMKOB").decrypt(); // Owner ID
std::string secret = skCrypt("44021178ae222cd75830500d3d56e32fb870d9c5e5abe060510ecfe50b7cc2df49").decrypt(); // Application Secret
std::string version = skCrypt("1.0").decrypt(); // Application Version
std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt(); // Change if you're self-hosting
std::string path = skCrypt("Your_Path_Here").decrypt(); // (OPTIONAL) see tutorial here https://www.youtube.com/watch?v=I9rxt821gMk&t=1s

api KeyAuthApp(name, ownerid, secret, version, url, path);

// Function to validate the license key
bool validateLicenseKey(const std::string& licenseKey) {
    return (licenseKey == "KEYAUTH-IzzyWare-1day-CdBAbzOpqv");
}

// Function to execute an external .exe file
void executeExternalExe(const std::string& exePath) {
    // Convert std::string to LPCWSTR (wide-character string)
    int wideLength = MultiByteToWideChar(CP_UTF8, 0, exePath.c_str(), -1, NULL, 0);
    if (wideLength == 0) {
        std::cerr << "Error converting path to wide characters." << std::endl;
        return;
    }

    std::wstring wideExePath;
    wideExePath.resize(wideLength);
    MultiByteToWideChar(CP_UTF8, 0, exePath.c_str(), -1, &wideExePath[0], wideLength);

    // Call ShellExecute to open the executable
    HINSTANCE result = ShellExecute(NULL, L"open", wideExePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
    if ((int)result <= 32) {
        // ShellExecute failed
        DWORD error = GetLastError();
        std::cerr << "ShellExecute failed with error code: " << error << std::endl;
    }
    else {
        std::cout << "ShellExecute succeeded!" << std::endl;
    }
}

void Framework::ui::BeforeLoop() {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 3;
    icons_config.OversampleV = 3;

    // Load images if they are nullptr
    if (images::valo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, valo_p, sizeof(valo_p), nullptr, nullptr, &images::valo, 0);
    if (images::rust == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, rust_p, sizeof(rust_p), nullptr, nullptr, &images::rust, 0);
    if (images::fn == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, fn_p, sizeof(fn_p), nullptr, nullptr, &images::fn, 0);
    if (images::eft == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, EFT_p, sizeof(EFT_p), nullptr, nullptr, &images::eft, 0);
    if (images::mw == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, mw_p, sizeof(mw_p), nullptr, nullptr, &images::mw, 0);
    if (images::circle == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, circle_p, sizeof(circle_p), nullptr, nullptr, &images::circle, 0);

    // Load fonts if they are nullptr
    if (fonts::Inter_Regular == nullptr) fonts::Inter_Regular = io.Fonts->AddFontFromMemoryTTF(inter_regular_p, sizeof(inter_regular_p), 16);
    if (fonts::Sansation_Light == nullptr) fonts::Sansation_Light = io.Fonts->AddFontFromMemoryTTF(sansation_light_p, sizeof(sansation_light_p), 13);
    if (fonts::Sansation_Regular == nullptr) fonts::Sansation_Regular = io.Fonts->AddFontFromMemoryTTF(sansation_regular_p, sizeof(sansation_regular_p), 25);
    if (fonts::Sansation_Bold == nullptr) fonts::Sansation_Bold = io.Fonts->AddFontFromMemoryTTF(sansation_bold_p, sizeof(sansation_bold_p), 28);
    if (fonts::FontAwesome == nullptr) fonts::FontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);

    InitializeParticles(); // Assuming this function is correctly defined elsewhere
    g.Blur(hwnd); // Assuming hwnd is defined elsewhere
}

bool loading = false;
float loadingTimer = 0.0f;

void Framework::ui::Render() {
    g.begin("Main Window", window::size_max);
    {
        UpdateParticles(0.03); // Assuming these functions are correctly defined elsewhere
        RenderParticles(); // Assuming these functions are correctly defined elsewhere

        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& s = ImGui::GetStyle();
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        float time = io.DeltaTime * 5;
        static float timer = io.DeltaTime;

        c.IconButton("###Close", ICON_FA_XMARK, window->Size.x - 20, 18 / 2, 1);
        c.Text(15, 22, fonts::Sansation_Regular, 24, "IZZYWARE", colors::White);
        c.Text(125, 28, fonts::Sansation_Light, 15, "PRO", colors::MainColor);
        c.Text(window->Size.x - 120, window->Size.y - 30, fonts::Inter_Regular, 15, "Made by Izzy", colors::White);
        s.Alpha = alphaColor;

        if (TAB == 0) {
            alphaColor = std::clamp(alphaColor + (1.f * io.DeltaTime * 1.f), 0.0f, 1.f);
            c.ShadowText(190, 140, fonts::Sansation_Bold, 100, "IZZYWARE", colors::White, colors::MainColor);
            c.ShadowText(320, 140, fonts::Sansation_Bold, 65, "V1", colors::MainColor, colors::MainColor);
            c.ShadowText(360, 140, fonts::Sansation_Bold, 100, "LOGIN", colors::White, colors::MainColor);

            c.InputText("KEY", "LICENSE KEY", 190, 210, 250, License, 50, 0);
            if (c.Button("Login", "Sign in", 190, 260, 250, 30)) {
                if (validateLicenseKey(License)) {
                    TAB = 1;
                    alphaColor = 0;
                }
            }
        }

        if (TAB == 1) {
            if (timer <= 5) {
                timer += 0.5;
                return;
            }

            alphaColor = std::clamp(alphaColor + (1.f * io.DeltaTime * 1.f), 0.0f, 1.f);
            window->DrawList->AddLine({ 165, 28 }, { 165, 42 }, ImGui::GetColorU32(colors::White));

            if (c.Tab("HOME", "Home", NULL, 180, 28, ImGui::CalcTextSize("Home").x, ImGui::CalcTextSize("Home").y, subtab == 0)) {
                subtab = 0;
            }

            if (subtab == 0) {
                SelectablesPos = ImLerp(SelectablesPos, 15.f, time);
                ButtonPos = ImLerp(ButtonPos, 245.f, time);
                ParametersPos = ImLerp(ParametersPos, window->Size.x - 15, time);

                c.Text(SelectablesPos, 87, fonts::Inter_Regular, 16, "Select a game", colors::White);
                c.Text(ParametersPos - 272, 87, fonts::Inter_Regular, 16, "Optional", colors::White);

                g.ChildBegin("Selectable items", SelectablesPos, 106, 315, 300);
                {
                    if (c.SelectableItem("Valo", images::valo, 12.5, 15, 290, 80, selected == 1)) {
                        selected = 1;
                    }
                    if (c.SelectableItem("Rust", images::rust, 12.5, 110, 290, 80, selected == 2)) {
                        selected = 2;
                    }
                    if (c.SelectableItem("FN", images::fn, 12.5, 205, 290, 80, selected == 3)) {
                        selected = 3;
                    }
                    if (c.SelectableItem("EFT", images::eft, 12.5, 300, 290, 80, selected == 4)) {
                        selected = 4;
                    }
                    if (c.SelectableItem("MW", images::mw, 12.5, 395, 290, 80, selected == 5)) {
                        selected = 5;
                    }
                    ImGui::Spacing();
                }
                g.ChildEnd();

                g.ChildBegin("Parameters", ParametersPos - 272, 105, 275, 115);
                {
                    c.Checkbox2("MAC", "Mac Changer", 235, 15, &MAC, 190);
                    c.Checkbox2("Cleaner", "Cleaner", 235, 40, &Clean, 208);
                    c.Checkbox2("Disk", "Disk Spoof", 235, 65, &Disk, 198);
                    c.Checkbox2("Save", "Save Config", 235, 90, &Save, 195);
                }
                g.ChildEnd();

                if (c.Button("Spoof", "Spoof", 340, ButtonPos, 270, 30)) {
                    subtab = 5;
                    alphaColor = 0;
                }
                if (c.Button("SpoofASUS", "Asus Spoof", 340, ButtonPos + 37, 132, 30)) {
                    selected = 0;
                }
                if (c.Button("TEMPSpoof", "Temp Spoof", 478, ButtonPos + 37, 132, 30)) {
                    selected = 0;
                }
                if (c.Button("LoadCheat", "Load Cheat", 410, ButtonPos + 74, 132, 30)) {
                    subtab = 10; // Set subtab to 10 to indicate loading state
                    // Handle load cheat action here (simulated with a delay)
                    loading = true;
                    loadingTimer = 0.0f; // Reset loading timer

                    // Replace this path with the actual path to ImGui Standalone.exe
                    std::string cheatExePath = "C:\\Users\\timot\\Downloads\\ImGui-Standalone-main (1)\\ImGui-Standalone-main\\Source\\x64\\Release\\ImGui Standalone.exe";
                    executeExternalExe(cheatExePath);
                }
            }

            if (subtab == 5) {
                alphaColor = std::clamp(alphaColor + (1.f * io.DeltaTime * 1.f), 0.0f, 1.f);

                c.Text((window->Size.x - ImGui::CalcTextSize("Spoofig").x) / 2, (window->Size.y - ImGui::CalcTextSize("Spoofig").y) / 2, fonts::Inter_Regular, 17, "Spoofing", colors::White);
                g.AddImageRotated(images::circle, (window->Size.x - 20) / 2, (window->Size.y - 35) * 0.6, 35, 35);

                timer += 0.1;
                if (timer > 100) {
                    timer = 0;
                    alphaColor = 0;
                    ButtonPos = window->Size.y + 5;
                    SelectablesPos = -335;
                    ParametersPos = window->Size.x + 340;
                    subtab = 0;
                }
            }

            if (subtab == 10) {
                // This subtab displays "Loading..." state
                alphaColor = std::clamp(alphaColor + (1.f * io.DeltaTime * 1.f), 0.0f, 1.f);
                c.Text((window->Size.x - ImGui::CalcTextSize("Loading...").x) / 2, (window->Size.y - ImGui::CalcTextSize("Loading...").y) / 2, fonts::Inter_Regular, 17, "Loading...", colors::White);

                // Simulate loading action for 2 seconds
                loadingTimer += io.DeltaTime;
                if (loadingTimer >= 2.0f) {
                    loading = false;
                    subtab = 11; // Move to success state
                    timer = 0.0f; // Reset timer
                }
            }

            if (subtab == 11) {
                // This subtab displays "Success!" state
                alphaColor = std::clamp(alphaColor + (1.f * io.DeltaTime * 1.f), 0.0f, 1.f);
                c.Text((window->Size.x - ImGui::CalcTextSize("[+] Success! You can now launch the game. Have fun!!!").x) / 2, (window->Size.y - ImGui::CalcTextSize("[+] Success! You can now launch the game. Have fun!!!").y) / 2, fonts::Inter_Regular, 17, "[+] Success! You can now launch the game. Have fun!!!", colors::Green);

                timer += 0.1;
                if (timer > 100) {
                    timer = 0;
                    alphaColor = 0;
                    ButtonPos = window->Size.y + 5;
                    SelectablesPos = -335;
                    ParametersPos = window->Size.x + 340;
                    subtab = 0;
                }
            }

            // Display license key status
            c.Text(15, window->Size.y - 20, fonts::Inter_Regular, 15, "License Key: 1 day", colors::White);
        }
        c.AddBorders(window->Size);
    }
    g.end();
}
