#include <iostream>
#include "utils/datamodel/DataModel.hpp"
#include "utils/overlay/overlay.hpp"
#include "driver/driver.hpp"
#include "Roblox/TeleportHandler.hpp"
#include "Roblox/Cheat.hpp"

int main() {

    const auto pDriver{Driver::get_singleton()};
    const auto pDataModel {DataModel::get_singleton()};


    pDriver->setup();

    while (pDriver->process_id == 0)
    {
        Sleep(1);
        pDriver->process_id = pDriver->get_process_id("RobloxPlayerBeta.exe");
    }
    pDriver->base_address = pDriver->get_base_address();
    if (!pDriver->base_address)
    {
        std::cout << "[+] Driver not loaded" << std::endl;
        Sleep(3000);
        exit(0);
    }

    auto Datamodel = static_cast<RobloxInstance>(pDataModel->get_datamodel());

    std::cout << std::hex << "[debug] RBX::RenderView ->  0x" << pDataModel->get_render_view() << std::endl;
    std::cout << std::hex << "[debug] RBX::VisualEngine -> 0x" << pDataModel->get_visualengine_address() << std::endl;
    std::cout << std::hex << "[debug] RBX::DataModel -> 0x" << Datamodel.self<< std::endl;

    const auto pTeleportHandler{TeleportHandler::get_singleton()};
    const auto pCheat{Cheat::get_singleton()};
    const auto pOverlay{Overlay::get_singleton()};

    // Teleport Handler
    pTeleportHandler->initialize(Datamodel.self);
    pTeleportHandler->handle_teleports();

    // Cheat
    pCheat->set_datamodel(Datamodel.self);
    pCheat->initialize();

    // Overlay or gui how you wanna call it
    pOverlay->initialize();

    std::cin.get();

    return 0;
}