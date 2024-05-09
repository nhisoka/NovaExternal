//
// Created by user on 5/8/2024.
//
#pragma once

#include <string>
#include <vector>
#include "../driver/Driver.hpp"
#include "../offsets.hpp"

class RobloxInstance {
    struct vector2_t final { float x, y; };
    struct vector3_t final { float x, y, z; };
    struct quaternion final { float x, y, z, w; };
    struct matrix4_t final { float data[16]; };
public:
    std::uint64_t self;

    std::string name();
    std::string class_name();
    std::vector<RobloxInstance> children();
    RobloxInstance find_first_child(std::string child);

    RobloxInstance get_local_player();
    RobloxInstance get_model_instance();
    RobloxInstance get_team();

    std::uintptr_t get_gameid();

    RobloxInstance get_workspace();
    RobloxInstance get_current_camera();

    vector2_t get_dimensions();
    matrix4_t get_view_matrix();
    vector3_t get_camera_pos();
    vector3_t get_part_pos();
    vector3_t get_part_velocity();

    float get_health();
    float get_max_health();

    void set_humanoid_walkspeed(float value);

    float get_walkspeed();

    float get_ping();
};

