//
// Created by user on 5/8/2024.
//

#include "DataModel.hpp"
#include "../../driver/Driver.hpp"
#include "../../Roblox/Instance.hpp"

DataModel *DataModel::g_Singleton = nullptr;

DataModel *DataModel::get_singleton() noexcept {
    if (g_Singleton == nullptr)
        g_Singleton = new DataModel();
    return g_Singleton;
}

std::uint64_t DataModel::get_render_view()
{
    const auto pLogInterceptor{LogInterceptor::get_singleton()};
    auto latest_log = pLogInterceptor->get_latest_log();

    std::ifstream rbx_log(latest_log);
    std::string rbx_log_line;

    while (true)
    {
        std::getline(rbx_log, rbx_log_line);
        if (rbx_log_line.contains("initialize view("))
        {
            rbx_log_line = rbx_log_line.substr(rbx_log_line.find("initialize view(") + 21);
            rbx_log_line = rbx_log_line.substr(0, rbx_log_line.find(')'));

            std::uint64_t renderview = std::strtoull(rbx_log_line.c_str(), nullptr, 16);
            return renderview;
        }
    }
}

std::uint64_t DataModel::get_visualengine_address()
{
    const auto pDriver{Driver::get_singleton()};
    auto render_view = this->get_render_view();
    auto visualengine =
            pDriver->read<std::uint64_t>(render_view + 0x10);

   return visualengine;
}

std::uint64_t DataModel::get_datamodel()
{
    const auto pDriver{Driver::get_singleton()};
    auto render_view = this->get_render_view();

    auto game_ptr = pDriver->read<std::uint64_t>(render_view + 0x118);
    auto game = pDriver->read<std::uint64_t>(game_ptr + 0x198); // update

    return game;
}