//
// Created by user on 5/8/2024.
//

#include <iostream>
#include "Instance.hpp"

const auto pDriver{Driver::get_singleton()};

std::string readstring(std::uint64_t address)
{
    std::string string;
    char character = 0;
    int char_size = sizeof(character);
    int offset = 0;

    string.reserve(204);

    while (offset < 200)
    {
        character = pDriver->read<char>(address + offset);

        if (character == 0)
            break;

        offset += char_size;
        string.push_back(character);
    }

    return string;
}


std::string readstring2(std::uint64_t string)
{
    const auto length = pDriver->read<int>(string + 0x18);

    if (length >= 16u)
    {
        const auto New = pDriver->read<std::uint64_t>(string);
        return readstring(New);
    }
    else
    {
        const auto Name = readstring(string);
        return Name;
    }
}

std::string RobloxInstance::name()
{
    const auto ptr = pDriver->read<std::uint64_t>(this->self + offsets::name);

    if (ptr)
        return readstring2(ptr);

    return "???";
}

std::string RobloxInstance::class_name()
{
    const auto ptr = pDriver->read<std::uint64_t>(this->self + offsets::classname);

    if (ptr)
        return readstring(ptr + 0x8); // return readstring2(ptr + 0x8)

    return "???_classname";
}

std::vector<RobloxInstance> RobloxInstance::children()
{
    std::vector<RobloxInstance> container;

    if (!this->self)
        return container;

    auto start = pDriver->read<std::uint64_t>(this->self + offsets::children);

    if (!start)
        return container;

    auto end = pDriver->read<std::uint64_t>(start + offsets::size);

    for (auto instances = pDriver->read<std::uint64_t>(start); instances != end; instances += 16)
        container.emplace_back(pDriver->read<RobloxInstance>(instances));

    return container;
}

RobloxInstance RobloxInstance::find_first_child(std::string child)
{
    RobloxInstance ret;

    for (auto &object : this->children())
    {
        if (object.name() == child)
        {
            ret = static_cast<RobloxInstance>(object);
            break;
        }
    }

    return ret;
}

void RobloxInstance::set_cframe(vector3_t coords)
{
    if(this->self) {
        pDriver->write<vector3_t>(this->self + offsets::cframe, coords);
    }
};

RobloxInstance::vector3_t RobloxInstance::get_cframe()
{
    if(this->self) {
        return pDriver->read<vector3_t>(this->self + offsets::cframe);
    }
}

void RobloxInstance::set_humanoid_walkspeed(float value)
{
    auto humanoid_instance = this->get_model_instance().find_first_child("Humanoid");

    if (humanoid_instance.self) {
        std::cout << "[debug] Player::ModelInstance -> " << humanoid_instance.self << std::endl;
        pDriver->write<float>(humanoid_instance.self + offsets::walkspeedA, value);
        pDriver->write<float>(humanoid_instance.self + offsets::walkspeedB, value);
    } else {
        std::cout << "[debug] No Humanoid found" << std::endl;
    }
}

float RobloxInstance::get_walkspeed()
{
    return 100;
}

float RobloxInstance::get_ping()
{
    float ping = pDriver->read<float>(this->self + 0x3F8);
    return ping * 2000;
}

RobloxInstance RobloxInstance::get_local_player()
{
    auto local_player = pDriver->read<RobloxInstance>(this->self + offsets::local_player);
    return local_player;
}

RobloxInstance RobloxInstance::get_model_instance()
{
    auto characterPointer = pDriver->read<uint64_t>(this->self + offsets::model_instance);
    auto character = pDriver->read<RobloxInstance>(characterPointer + 0x8);

    return character;
}

RobloxInstance::vector2_t RobloxInstance::get_dimensions()
{
    auto dimensions = pDriver->read<vector2_t>(this->self + offsets::dimensions);
    return dimensions;
}

RobloxInstance::matrix4_t RobloxInstance::get_view_matrix()
{
    auto dimensions = pDriver->read<matrix4_t>(this->self + offsets::viewmatrix);
    return dimensions;
}

RobloxInstance::vector3_t RobloxInstance::get_camera_pos()
{
    auto camera_pos = pDriver->read<vector3_t>(this->self + offsets::camera_pos);
    return camera_pos;
}

RobloxInstance::vector3_t RobloxInstance::get_part_pos()
{
    vector3_t res{};

    auto primitive = pDriver->read<std::uint64_t>(this->self + offsets::primitive);

    if (!primitive)
        return res;

    res = pDriver->read<vector3_t>(primitive + offsets::position);
    return res;
}

RobloxInstance::vector3_t RobloxInstance::get_part_velocity()
{
    vector3_t res{};

    auto primitive = pDriver->read<std::uint64_t>(this->self + offsets::primitive);

    if (!primitive)
        return res;

    res = pDriver->read<vector3_t>(primitive + offsets::velocity);
    return res;
}


float RobloxInstance::get_health()
{
    auto humanoid_instance = this->get_model_instance().find_first_child("Humanoid");

    if (humanoid_instance.self) {
        return pDriver->read<float>(humanoid_instance.self + offsets::health);
    } else {
        std::cout << "hell nah"  << std::endl;
    }

    return 0;
}

float RobloxInstance::get_max_health()
{
    auto humanoid_instance = this->get_model_instance().find_first_child("Humanoid");

    if (humanoid_instance.self) {
        std::cout << "[debug] RBX::Humanoid -> " << humanoid_instance.self << std::endl;
        return pDriver->read<float>(humanoid_instance.self + offsets::maxHealth);
    } else {
        std::cout << "[debug] RBX::Humanoid -> 0x0" << std::endl;
    }
    return 100;
} // LocalPlayer->get_max_health() 🔥

RobloxInstance RobloxInstance::get_team()
{
    auto getteam = pDriver->read<RobloxInstance>(this->self + offsets::team);
    return getteam;
}

std::uintptr_t RobloxInstance::get_gameid()
{
    auto gameid = pDriver->read<std::uintptr_t>(this->self + offsets::gameid);
    return gameid;
}
