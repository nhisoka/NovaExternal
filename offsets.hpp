//
// Created by user on 5/8/2024.
//

#include <cstdint>

namespace offsets {
    constexpr std::uint32_t size = 0x8; // 8 bytes
    constexpr std::uint32_t name = 0x48; // updated
    constexpr std::uint32_t classname = 0x18; // updated
    constexpr std::uint32_t children = 0x50;  // updated
    constexpr std::uint32_t parent = 0x60; // updated
    constexpr std::uint32_t walkspeedA = 0x1B0; // updated
    constexpr std::uint32_t health = 0x174; // updated
    constexpr std::uint32_t maxHealth = 0x194; // updated
    constexpr std::uint32_t walkspeedB = 0x388; // updated
    constexpr std::uint32_t local_player = 0x100; // updated
    constexpr std::uint32_t model_instance = 0x298; // updated
    constexpr std::uint32_t dimensions = 0x720; // updated
    constexpr std::uint32_t viewmatrix = 0x4B0; // updated
    constexpr std::uint32_t camera_pos = 0x268; // updated
    constexpr std::uint32_t camera_fov = 0x138; // updated
    constexpr std::uint32_t primitive = 0x158; // updated
    constexpr std::uint32_t position = 0x140; // updated
    constexpr std::uint32_t cframe = 0x11c; // updated
    constexpr std::uint32_t velocity = 0x14C; // updated
    constexpr std::uint32_t team = 0x1d0; // updated
    constexpr std::uint32_t gameid = 0x160; // updated
};