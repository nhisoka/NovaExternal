//
// Created by user on 5/8/2024.
//
#pragma once

#include <Windows.h>
#include <ShlObj.h>
#include <string>
#include <vector>
#include <filesystem>

class LogInterceptor {
    static LogInterceptor *g_Singleton;

public:
    static LogInterceptor *get_singleton() noexcept;

    std::vector<std::filesystem::path> get_roblox_file_logs();

    std::wstring get_username();

    std::filesystem::path get_latest_log();
};
