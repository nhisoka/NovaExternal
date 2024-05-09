#include <iostream>
#include "utils/datamodel/DataModel.hpp"
#include "driver/driver.hpp"

int main() {

    const auto pDriver{Driver::get_singleton()};
    const auto pDataModel {DataModel::get_singleton()};

    if(!pDriver->setup())
    {
        return false;
    }

    while (pDriver->process_id == 0)
    {
        Sleep(1);
        pDriver->process_id = pDriver->get_process_id("RobloxPlayerBeta.exe");
    }
    pDriver->base_address = pDriver->get_base_address();
    if (!pDriver->base_address)
    {
        std::cout << "[+] Driver error" << std::endl;
        Sleep(3000);
        exit(0);
    }

    std::cout << std::hex << "[debug] RBX::RenderView ->  0x" << pDataModel->get_render_view() << std::endl;
    std::cout << std::hex << "[debug] RBX::VisualEngine -> 0x" << pDataModel->get_visualengine_address() << std::endl;

    auto Datamodel = static_cast<RobloxInstance>(pDataModel->get_datamodel());

    std::cout << "[debug] RBX::DataModel -> 0x" << Datamodel.self<< std::endl;

    auto Players = Datamodel.find_first_child("Players");

    auto LocalPlayer = Players.get_local_player();

    std::cout << "[debug] LocalPlayer: 0x" << LocalPlayer.self  << " -> " << LocalPlayer.name() << std::endl;

    LocalPlayer.set_humanoid_walkspeed(120);

    std::cout << "[debug] Walkspeed set to 120" << std::endl;

    std::cout << "[debug] Health -> " << LocalPlayer.get_health() << std::endl;

    std::cin.get();

    return 0;
}