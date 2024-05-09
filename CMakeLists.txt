cmake_minimum_required(VERSION 3.28)
project(NovaExternal)

set(CMAKE_CXX_STANDARD 23)

add_executable(NovaExternal entry.cpp
        utils/logs/LogInterceptor.cpp
        utils/logs/LogInterceptor.hpp
        utils/datamodel/DataModel.cpp
        utils/datamodel/DataModel.hpp
        driver/Driver.cpp
        driver/Driver.hpp
        driver/librarys.hpp
        Roblox/Instance.cpp
        Roblox/Instance.hpp
        offsets.hpp
        Roblox/TeleportHandler.cpp
        Roblox/TeleportHandler.hpp)

SET_TARGET_PROPERTIES(NovaExternal PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:CONSOLE")
