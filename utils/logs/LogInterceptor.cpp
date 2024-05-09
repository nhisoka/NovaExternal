//
// Created by nhisoka on 5/8/2024.
//

#include "LogInterceptor.hpp"

LogInterceptor *LogInterceptor::g_Singleton = nullptr;

LogInterceptor *LogInterceptor::get_singleton() noexcept {
    if (g_Singleton == nullptr)
        g_Singleton = new LogInterceptor();
    return g_Singleton;
}

static std::wstring appdata_path()
{
    wchar_t path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path)))
    {
        return std::wstring(path);
    }
    return L"";
}

std::vector<std::filesystem::path> LogInterceptor::get_roblox_file_logs()
{
    std::vector<std::filesystem::path> roblox_log;
    std::wstring app_data_path = appdata_path();
    std::wstring roblox_log_path = app_data_path + L"\\Roblox\\logs";

    for (const auto &entry : std::filesystem::directory_iterator(roblox_log_path))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".log" && entry.path().filename().string().find("Player") != std::string::npos)
            roblox_log.push_back(entry.path());
    }

    return roblox_log;
}

std::wstring LogInterceptor::get_username()
{
    wchar_t username[256];
    DWORD length = sizeof(username);
    GetUserNameW(username, &length);
    return username;
}

std::filesystem::path LogInterceptor::get_latest_log()
{
    auto logs = get_roblox_file_logs();

    std::sort(logs.begin(), logs.end(), [](const std::wstring &a, const std::wstring &b) {
        return std::filesystem::last_write_time(a) > std::filesystem::last_write_time(b);
    });

    return logs[0];
}