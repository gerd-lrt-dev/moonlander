project(MoonLanderBackend)

file(GLOB
    BACKEND_SOURCES src/*.cpp
    BACKEND_HEADERS include/*.h
)

add_library(moonlander_backend ${BACKEND_SOURCES} ${BACKEND_HEADERS})

target_include_directories(moonlander_backend
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

