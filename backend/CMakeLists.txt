project(MoonLanderBackend)

file(GLOB BACKEND_SOURCES
    src/*.cpp
)

add_library(moonlander_backend ${BACKEND_SOURCES})

target_include_directories(moonlander_backend
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

