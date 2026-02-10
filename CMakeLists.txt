cmake_minimum_required(VERSION 3.5)
project(Simulation)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

# Tell the compiler where to find header files
include_directories(${PROJECT_SOURCE_DIR}/include)  # adds include/ to search path :contentReference[oaicite:0]{index=0}

# Gather all source files from src/
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cc
)

# (Optional) gather header files for IDE visibility
file(GLOB HEADERS
    ${PROJECT_SOURCE_DIR}/include/*.hh
)

# Gather macro files
file(GLOB MACRO_FILES
    "*.mac"
)

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

# Create the executable
add_executable(sim ${SOURCES} ${HEADERS})
target_link_libraries(sim ${Geant4_LIBRARIES})

# Optional target alias for convenience
add_custom_target(Simulation DEPENDS sim)
