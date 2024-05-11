//
// Created by user on 5/9/2024.
//

#include <iostream>
#include "Cheat.hpp"
#include "Instance.hpp"
#include "../utils/overlay/overlay.hpp"
#include <thread>

Cheat *Cheat::g_Singleton = nullptr;

Cheat *Cheat::get_singleton() noexcept {
    if (g_Singleton == nullptr)
        g_Singleton = new Cheat();
    return g_Singleton;
}

void Cheat::set_datamodel(std::uint64_t dataModel) {
    this->datamodel = dataModel;
};

void Cheat::initialize() const {
    system("cls");

    auto Datamodel = static_cast<RobloxInstance>(this->datamodel);

    auto Players = Datamodel.find_first_child("Players");


    while(true) {if(Players.get_local_player().self && Players.get_local_player().get_model_instance().self) break; std::this_thread::sleep_for(std::chrono::seconds(2));}; // important check

    auto LocalPlayer = Players.get_local_player();

    std::cout << "[debug] LocalPlayer: 0x" << LocalPlayer.self  << " -> " << LocalPlayer.name() << std::endl;

    auto character = LocalPlayer.get_model_instance();

    auto humanoidrootpart = character.find_first_child("HumanoidRootPart");


   // LocalPlayer.set_humanoid_walkspeed(120);

    std::cout << "[debug] Walkspeed set to 120" << std::endl;

    std::cout << "[debug] Health -> " << LocalPlayer.get_health() << std::endl;

};