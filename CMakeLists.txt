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
        offsets.hpp
        Roblox/Instance.hpp
        Roblox/TeleportHandler.cpp
        Roblox/TeleportHandler.hpp
        Roblox/Cheat.cpp
        Roblox/Cheat.hpp
        utils/overlay/overlay.cpp
        utils/overlay/overlay.hpp
        globals.hpp)

SET_TARGET_PROPERTIES(NovaExternal PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:CONSOLE")

target_include_directories(NovaExternal PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies")
# Dependencies

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui")

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        # Imgui
        Imgui
)
