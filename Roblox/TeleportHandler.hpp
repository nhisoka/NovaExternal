//
// Created by user on 5/9/2024.
//

#include <cstdint>

class TeleportHandler {
    static TeleportHandler *g_Singleton;

public:
    static TeleportHandler *get_singleton() noexcept;

    std::uint64_t datamodel;
    bool isInitialized;

    void initialize(std::uint64_t dataModel);

    void handle_teleports();
};
