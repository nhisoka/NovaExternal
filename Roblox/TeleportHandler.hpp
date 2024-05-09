//
// Created by user on 5/9/2024.
//

#include <cstdint>

class TeleportHandler {
    static TeleportHandler *g_Singleton;
    std::uint64_t datamodel;
    bool isInitialized;

public:
    static TeleportHandler *get_singleton() noexcept;


    void initialize(std::uint64_t dataModel);

    void handle_teleports();
};
