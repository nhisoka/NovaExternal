//
// Created by user on 5/9/2024.
//

#include <iostream>
#include <thread>
#include "../utils/datamodel/DataModel.hpp"
#include "TeleportHandler.hpp"

TeleportHandler *TeleportHandler::g_Singleton = nullptr;

TeleportHandler *TeleportHandler::get_singleton() noexcept {
    if (g_Singleton == nullptr)
        g_Singleton = new TeleportHandler();
    return g_Singleton;
}

void TeleportHandler::initialize(std::uint64_t datamodelAddress) {
    this->datamodel = datamodelAddress;
    this->isInitialized = true;
    std::cout << "[debug] Initialized Teleport Handler" << std::endl;
}

void TeleportHandler::handle_teleports() {
    if(this->isInitialized) {
        std::cout << "[debug] Started Teleport Handler" << std::endl;

        std::thread([this] {
            while (true) {
                auto pDatamodel{DataModel::get_singleton()};
                auto current_datamodel = static_cast<RobloxInstance>(pDatamodel->get_datamodel());

                if(this->datamodel != current_datamodel.self && current_datamodel.get_gameid() != 0) {
                    this->datamodel = current_datamodel.self; // set the teleport handler datamodel to the new one so it doesnt detect it as new teleport
                    std::cout << "[debug] Teleport detected" << std::endl;
                }

                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }).detach();
    } else {
        std::cout << "[debug] Teleport Handler not initialized!" << std::endl;
    }
}