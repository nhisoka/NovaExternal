//
// Created by user on 5/9/2024.
//

#include <cstdint>

class Cheat {
    static Cheat *g_Singleton;
    std::uint64_t datamodel;

public:
    static Cheat *get_singleton() noexcept;


    void set_datamodel(std::uint64_t dataModel);

    void initialize() const;
};
