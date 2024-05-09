//
// Created by user on 5/9/2024.
//

#include "TeleportHandler.hpp"

TeleportHandler *TeleportHandler::g_Singleton = nullptr;

TeleportHandler *TeleportHandler::get_singleton() noexcept {
    if (g_Singleton == nullptr)
        g_Singleton = new TeleportHandler();
    return g_Singleton;
}
