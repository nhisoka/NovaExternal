//
// Created by user on 5/9/2024.
//

#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_toggle.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "overlay.hpp"
#include "../../globals.hpp"
#include <dwmapi.h>
#include <thread>

#include <filesystem>
#include <dxgi.h>
#include <d3d11.h>


ID3D11Device* Overlay::d3d11_device = nullptr;

ID3D11DeviceContext* Overlay::d3d11_device_context = nullptr;

IDXGISwapChain* Overlay::dxgi_swap_chain = nullptr;

ID3D11RenderTargetView* Overlay::d3d11_render_target_view = nullptr;

static const char* combo_items_4[4] = { ("Head"), ("UpperTorso"), ("HumanoidRootPart"), ("LowerTorso") };
static const char* box_items_3[2] = { ("2D Box"), ("2D Corner Box") };

Overlay *Overlay::g_Singleton = nullptr;

Overlay *Overlay::get_singleton() noexcept {
    if (g_Singleton == nullptr)
        g_Singleton = new Overlay();
    return g_Singleton;
}


bool Overlay::fullsc(HWND windowHandle)
{
    MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
    if (GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), &monitorInfo))
    {
        RECT windowRect;
        if (GetWindowRect(windowHandle, &windowRect))
        {
            return windowRect.left == monitorInfo.rcMonitor.left
                   && windowRect.right == monitorInfo.rcMonitor.right
                   && windowRect.top == monitorInfo.rcMonitor.top
                   && windowRect.bottom == monitorInfo.rcMonitor.bottom;
        }
    }
}


bool isValid(const std::string& str) {
    bool startsWithNumber = std::isdigit(str[0]);
    bool containsSymbol = false;

    for (char ch : str) {
        if (!std::isalnum(ch) && ch != '_') {
            containsSymbol = true;
            break;
        }
    }

    return startsWithNumber || containsSymbol;
}


static char text[999] = "";
char configname[100];

bool Overlay::init = false;

bool if_first = false;

bool ButtonCenteredOnLine(const char* label, float alignment = 0.5f)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

    return ImGui::Button(label);
}

const char* fontPath = "C:/Users/Admin/Downloads/DOWNLOADS/Websites CCs and Fonts/Circular-Std-Bold.ttf";

bool Overlay::render()
{

    ImGui_ImplWin32_EnableDpiAwareness();

    WNDCLASSEX wc;
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpfnWndProc = window_proc;
    wc.lpszClassName = TEXT("flexmusix");
    wc.lpszMenuName = nullptr;
    wc.style = CS_VREDRAW | CS_HREDRAW;

    RegisterClassEx(&wc);
    const HWND hw = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, wc.lpszClassName, TEXT("4daflex"),
                                   WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, wc.hInstance, nullptr);

    SetLayeredWindowAttributes(hw, 0, 255, LWA_ALPHA);
    const MARGINS margin = { -1 };
    DwmExtendFrameIntoClientArea(hw, &margin);

    if (!create_device_d3d(hw))
    {
        cleanup_device_d3d();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return false;
    }

    ShowWindow(hw, SW_SHOW);
    UpdateWindow(hw);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui::GetIO().IniFilename = nullptr;

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4 originalButtonColor = style.Colors[ImGuiCol_Button];

    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath, 14.0f);

    io.FontDefault = font;

    style.WindowRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.ChildRounding = 5.0f;
    style.ScrollbarRounding = 0;

    style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);



    ImGui_ImplWin32_Init(hw);
    ImGui_ImplDX11_Init(d3d11_device, d3d11_device_context);

    const ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    init = true;

    bool draw = false;
    bool done = false;
    int tab = 0;


    while (!done)
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
            {
                done = true;
            }
        }

        move_window(hw);

        if (GetAsyncKeyState(VK_INSERT) & 1)
            draw = !draw;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            if (GetForegroundWindow() == FindWindowA(0, ("Roblox")) || GetForegroundWindow() == hw)
            {
                ImGui::Begin("Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);
                {

                    ImGui::End();
                }

                if (draw)
                {

                    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
                    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.80f, 0.80f, 0.80f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.0f, 0.0f, 0.0f, 0.80f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.05f, 0.05f, 0.05f, 0.80f));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.10f, 0.10f, 0.10f, 0.80f));

                    ImGui::SetNextWindowSize(ImVec2(705, 420));
                    ImGui::Begin("Nova External", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
                    ImGui::BeginChild("ChildWindow", ImVec2(685, 35), true);

                    if (tab == 0)
                    {
                        ImGui::EndChild();

                        ImGui::SetCursorPosX(8);
                        ImGui::SetCursorPosY(70);

                        float windowWidth = 678;

                        ImGui::BeginChild("LeftTab", ImVec2(windowWidth / 2, 340), true, ImGuiWindowFlags_NoScrollbar);
                        {
                            const ImVec4 green(1.0f, 0.7137f, 0.7568f, 0.65f);
                            const ImVec4 green_hover(1.0f, 0.70f, 0.80f, 0.85f);

                            ImGui::PushStyleColor(ImGuiCol_Button, green);
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, green_hover);

                            ImGui::Toggle("Enable", &globals::aimbot);

                            ImGui::Toggle(("Sticky Aim"), &globals::sticky_aim);
                            ImGui::Toggle(("Prediction"), &globals::prediction);
                            ImGui::Toggle(("Auto Pred"), &globals::autopred);

                            ImGui::Toggle(("Shake"), &globals::shake);
                            ImGui::Toggle(("Resolver"), &globals::resolver);
                            ImGui::Toggle(("Deadzone"), &globals::deadzone);
                            ImGui::Toggle(("Stabilizer"), &globals::stabilizer);
                            ImGui::Toggle(("Teamcheck"), &globals::team_check);
                            ImGui::Toggle(("Disable Outside of FOV"), &globals::disable_outside_fov);

                            ImGui::PopStyleColor(2);
                        }
                        ImGui::EndChild();

                        ImGui::SameLine();
                        ImGui::BeginChild(("RightTab"), ImVec2(windowWidth / 2, 340), true, ImGuiWindowFlags_NoScrollbar);
                        {
                            ImGui::Combo(("Part"), &globals::aimpart, combo_items_4, 4);
                            ImGui::SliderInt(("FOV"), &globals::fov, 1, 1000);
                            ImGui::SliderInt(("Deadzone"), &globals::deadzone_value, 1, 1000);
                            ImGui::Separator();
                            ImGui::SliderFloat(("Smoothness X"), &globals::smoothness_x, 1, 30);
                            ImGui::SliderFloat(("Smoothness Y"), &globals::smoothness_y, 1, 30);
                            ImGui::Separator();
                            ImGui::SliderFloat(("Prediction X"), &globals::prediction_x, 1, 10);
                            ImGui::SliderFloat(("Prediction Y"), &globals::prediction_y, 1, 10);
                            ImGui::Separator();
                            ImGui::SliderFloat(("Stabilizer X"), &globals::stabilizer_x, 1, 5);
                            ImGui::SliderFloat(("Stabilizer Y"), &globals::stabilizer_y, 1, 5);
                            ImGui::Separator();
                            ImGui::SliderFloat(("Shake Strenght"), &globals::shake_value, 0, 10);
                            ImGui::SliderFloat(("Sensitivity"), &globals::sensitivity, 0, 5);
                        }
                        ImGui::EndChild();

                        ImGui::BeginChild(("ChildWindow"), ImVec2(110, 358), true);
                    }

                    if (tab == 1)
                    {
                        static ImVec4 color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

                        ImGui::EndChild();

                        ImGui::BeginChild(("ChildWindow"), ImVec2(110, 358), true);
                        ImGui::EndChild();

                        ImGui::SetCursorPosX(8);
                        ImGui::SetCursorPosY(70);

                        float windowWidth = 678;

                        ImGui::BeginChild(("LeftTab"), ImVec2(windowWidth / 2, 340), true, ImGuiWindowFlags_NoScrollbar);
                        {

                            const ImVec4 green(1.0f, 0.7137f, 0.7568f, 0.65f);
                            const ImVec4 green_hover(1.0f, 0.70f, 0.80f, 0.85f);

                            ImGui::PushStyleColor(ImGuiCol_Button, green);
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, green_hover);

                            ImGui::Toggle(("Enabled"), &globals::esp);
                            ImGui::Toggle(("Box"), &globals::box);
                            ImGui::Toggle(("Outline"), &globals::outline);
                            ImGui::Toggle(("Fill Box"), &globals::filledbox);
                            ImGui::Toggle(("Rainbow Box"), &globals::rainbowbox);
                            ImGui::Combo(("Type"), &globals::boxtype, box_items_3, 2);
                            ImGui::Separator();
                            ImGui::Toggle(("Name"), &globals::name_esp);
                            ImGui::Toggle(("Healthbar"), &globals::healthbar);
                            ImGui::Toggle(("Distance"), &globals::distance_esp);
                            ImGui::Toggle(("Tracers"), &globals::tracers);
                            ImGui::Toggle(("Health Info"), &globals::healthinfo);
                            ImGui::Toggle(("Local ESP"), &globals::local_box);
                            ImGui::Toggle(("Teamcheck"), &globals::team_check_esp);
                            ImGui::Separator();
                            ImGui::Toggle(("Draw FOV"), &globals::fov_on);
                            ImGui::Toggle(("Fill FOV"), &globals::fov_filled);
                            ImGui::Toggle(("Draw Deadzone"), &globals::show_deadzone);

                            ImGui::PopStyleColor(2);
                        }
                        ImGui::EndChild();

                        ImGui::SameLine();
                        ImGui::BeginChild(("RightTab"), ImVec2(windowWidth / 2, 340), true, ImGuiWindowFlags_NoScrollbar);
                        {
                            ImGui::ColorEdit4("Box Color", globals::box_color, ImGuiColorEditFlags_NoInputs);
                            ImGui::ColorEdit4("Enemy Team Color", globals::enemy_team_color, ImGuiColorEditFlags_NoInputs);
                            ImGui::ColorEdit4("Fill Color", globals::fill_color, ImGuiColorEditFlags_NoInputs);
                            ImGui::Separator();
                            ImGui::ColorEdit4("Name Color", globals::name_color, ImGuiColorEditFlags_NoInputs);
                            ImGui::ColorEdit4("Tracers Color", globals::tracers_color, ImGuiColorEditFlags_NoInputs);
                            ImGui::Separator();
                            ImGui::ColorEdit4("FOV Color", globals::fov_color, ImGuiColorEditFlags_NoInputs);
                            ImGui::ColorEdit4("FOV Filled Color", globals::fov_fill_color, ImGuiColorEditFlags_NoInputs);
                            ImGui::ColorEdit4("Deadzone Color", globals::deadzone_color, ImGuiColorEditFlags_NoInputs);
                        }
                        ImGui::EndChild();

                        ImGui::BeginChild(("ChildWindow"), ImVec2(110, 358), true);
                    }



                    if (tab == 2)
                    {
                        ImGui::EndChild();

                        ImGui::SetCursorPosX(8);
                        ImGui::SetCursorPosY(70);

                        ImGui::BeginChild(("SettingsChildWindow"), ImVec2(685, 340), true);
                        {
                            const ImVec4 green(1.0f, 0.7137f, 0.7568f, 0.65f);
                            const ImVec4 green_hover(1.0f, 0.70f, 0.80f, 0.85f);

                            ImGui::PushStyleColor(ImGuiCol_Button, green);
                            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, green_hover);

                            ImGui::Toggle(("Healthcheck"), &globals::healthcheck);
                            ImGui::Toggle(("Knockcheck"), &globals::knock_check);
                            ImGui::Toggle(("Streamproof"), &globals::streamproof);
                            ImGui::Toggle(("V-Sync"), &globals::vsync);

                            if (ImGui::IsItemHovered())
                            {
                                ImGui::BeginTooltip();
                                ImGui::Text("Turn V-Sync off if you have a good PC");
                                ImGui::EndTooltip();
                            }

                            ImGui::PopStyleColor(2);

                            ImGui::Separator();

                            ImGui::Text(("Configs"));

                            std::vector<std::string> configFiles;


                            static std::string current_item = configFiles.empty() ? "" : configFiles[0];

                            for (int n = 0; n < configFiles.size(); n++) {
                                if (configFiles[n] == "")
                                    continue;

                                bool is_selected = (current_item == configFiles[n]);
                                if (ImGui::Selectable(configFiles[n].c_str(), is_selected)) {
                                    current_item = configFiles[n];

                                    size_t extensionPos = current_item.find(".cfg");
                                    if (extensionPos != std::string::npos) {
                                        current_item.erase(extensionPos);
                                    }

                                    strncpy(configname, current_item.c_str(), IM_ARRAYSIZE(configname));
                                }
                            }

                            ImGui::InputText(("Config Name"), configname, IM_ARRAYSIZE(configname));
                            if (ImGui::Button(("Load"))) {

                            }

                            ImGui::SameLine();

                            if (ImGui::Button(("Save"))) {

                            }

                            ImGui::SameLine();



                        }
                        ImGui::EndChild();

                        ImGui::BeginChild(("ChildWindow"), ImVec2(110, 358), true);
                    }


                    if (ImGui::Button(("Aimbot")))
                    {
                        tab = 0;
                    }
                    ImGui::SameLine();

                    if (ImGui::Button(("Visuals")))
                    {
                        tab = 1;
                    }
                    ImGui::SameLine();

                    if (ImGui::Button(("Settings")))
                    {
                        tab = 2;
                    }
                    ImGui::SameLine();


                    ImGui::PopStyleColor(4);

                    ImGui::EndChild();
                    ImGui::End();
                }
            }


            if (globals::streamproof)
            {
                SetWindowDisplayAffinity(hw, WDA_EXCLUDEFROMCAPTURE);
            }
            else
            {
                SetWindowDisplayAffinity(hw, WDA_NONE);
            }

            //if (globals::show_auth)
            if (draw)
            {
                SetWindowLong(hw, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
            }
            else
            {
                SetWindowLong(hw, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
            }

            ImGui::EndFrame();
            ImGui::Render();

            const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
            d3d11_device_context->OMSetRenderTargets(1, &d3d11_render_target_view, nullptr);
            d3d11_device_context->ClearRenderTargetView(d3d11_render_target_view, clear_color_with_alpha);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            if (globals::vsync)
            {
                dxgi_swap_chain->Present(1, 0);
            }
            else
            {
                dxgi_swap_chain->Present(0, 0);

            }
        }
    }

    init = false;


    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    cleanup_device_d3d();
    DestroyWindow(hw);
    UnregisterClass(wc.lpszClassName, wc.hInstance);
}


void Overlay::move_window(HWND hw)
{
    HWND target = FindWindowA(0, ("Roblox"));
    HWND foregroundWindow = GetForegroundWindow();

    if (target != foregroundWindow && hw != foregroundWindow)
    {
        MoveWindow(hw, 0, 0, 0, 0, true);
    }
    else
    {
        RECT rect;
        GetWindowRect(target, &rect);

        int rsize_x = rect.right - rect.left;
        int rsize_y = rect.bottom - rect.top;

        if (fullsc(target))
        {
            rsize_x += 16;
            rsize_y -= 24;
        }
        else
        {
            rsize_y -= 63;
            rect.left += 8;
            rect.top += 31;
        }

        MoveWindow(hw, rect.left, rect.top, rsize_x, rsize_y, TRUE);
    }
}

bool Overlay::create_device_d3d(HWND hw)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hw;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    const UINT create_device_flags = 0;
    D3D_FEATURE_LEVEL d3d_feature_level;
    const D3D_FEATURE_LEVEL feature_level_arr[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, create_device_flags, feature_level_arr, 2, D3D11_SDK_VERSION, &sd, &dxgi_swap_chain, &d3d11_device, &d3d_feature_level, &d3d11_device_context);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, create_device_flags, feature_level_arr, 2, D3D11_SDK_VERSION, &sd, &dxgi_swap_chain, &d3d11_device, &d3d_feature_level, &d3d11_device_context);
    if (res != S_OK)
        return false;

    create_render_target();
    return true;
}

void Overlay::cleanup_device_d3d()
{
    cleanup_render_target();

    if (dxgi_swap_chain)
    {
        dxgi_swap_chain->Release();
        dxgi_swap_chain = nullptr;
    }

    if (d3d11_device_context)
    {
        d3d11_device_context->Release();
        d3d11_device_context = nullptr;
    }

    if (d3d11_device)
    {
        d3d11_device->Release();
        d3d11_device = nullptr;
    }
}

void Overlay::create_render_target()
{
    ID3D11Texture2D* d3d11_back_buffer;
    dxgi_swap_chain->GetBuffer(0, IID_PPV_ARGS(&d3d11_back_buffer));
    if (d3d11_back_buffer != nullptr)
    {
        d3d11_device->CreateRenderTargetView(d3d11_back_buffer, nullptr, &d3d11_render_target_view);
        d3d11_back_buffer->Release();
    }
}

void Overlay::cleanup_render_target()
{
    if (d3d11_render_target_view)
    {
        d3d11_render_target_view->Release();
        d3d11_render_target_view = nullptr;
    }
}
void Overlay::initialize()
{
    std::thread([this] {
        this->render();
    }).detach();
}

LRESULT __stdcall Overlay::window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
        case WM_SIZE:
            if (d3d11_device != nullptr && wParam != SIZE_MINIMIZED)
            {
                cleanup_render_target();
                dxgi_swap_chain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                create_render_target();
            }
            return 0;

        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU)
                return 0;
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}