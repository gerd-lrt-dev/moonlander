cmake_minimum_required(VERSION 3.16)
project(MoonLanderUI)

find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB UI_SOURCES
    *.cpp
)

add_executable(moonlander_ui ${UI_SOURCES})

target_link_libraries(moonlander_ui
    PRIVATE
        Qt6::Widgets
        moonlander_backend
)

