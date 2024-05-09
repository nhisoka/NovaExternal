//
// Created by user on 5/9/2024.
//

#include <cstdint>

class TeleportHandler {
    static TeleportHandler *g_Singleton;

public:
    static TeleportHandler *get_singleton() noexcept;
};
