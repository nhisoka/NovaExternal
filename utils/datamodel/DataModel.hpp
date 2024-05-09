//
// Created by user on 5/8/2024.
//
#pragma once

#include <cstdint>
#include <fstream>
#include "../logs/LogInterceptor.hpp"
#include "../../Roblox/Instance.hpp"


class DataModel {
    static DataModel *g_Singleton;

public:
    static DataModel *get_singleton() noexcept;

    std::uint64_t get_render_view();

    std::uint64_t get_visualengine_address();

    std::uint64_t get_datamodel();
};
