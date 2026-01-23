project(MoonLanderBackend)

find_package(NLopt REQUIRED)

file(GLOB
    BACKEND_SOURCES src/*.cpp
    BACKEND_HEADERS include/*.h
)

add_library(moonlander_backend
    ${BACKEND_SOURCES}
    ${BACKEND_HEADERS}
    src/Integrators/Dynamics.cpp
    src/Optimization/thrustCostFunction.cpp
    src/Optimization/thrustOptimizer.cpp
    include/Integrators/Dynamics.h
    include/Optimization/optimizationStruct.h
    include/Optimization/modelParams.h
    include/Optimization/thrustOptimizationProblem.h
    include/Optimization/thrustOptimizer.h
)

target_include_directories(moonlander_backend
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(moonlander_backend
    PUBLIC
        NLopt::nlopt
)
