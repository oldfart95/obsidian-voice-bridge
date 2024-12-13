cmake_minimum_required(VERSION 3.15)
project(obsidian_tts_bridge VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_AUTOMOC ON)  # Enable Qt MOC
set(CMAKE_AUTORCC ON)  # Enable Qt Resource Compiler
set(CMAKE_AUTOUIC ON)  # Enable Qt UI Compiler

# Ensure we use system Qt
set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake")
set(Qt6_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt6")
set(Qt6Core_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt6Core")
set(Qt6Multimedia_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt6Multimedia")

# Find Python
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# Find required libraries
find_package(Qt6 COMPONENTS Core Multimedia REQUIRED)

# Source files
set(SOURCES
    src/tts_bridge.cpp
    src/text_processor.cpp
    src/audio_handler.cpp
    src/coqui_engine.cpp
)

# Header files
set(HEADERS
    include/tts_bridge.hpp
    include/text_processor.hpp
    include/audio_handler.hpp
    include/coqui_engine.hpp
)

# Create the Python module
pybind11_add_module(obsidian_tts_bridge ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(obsidian_tts_bridge PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries
target_link_libraries(obsidian_tts_bridge PRIVATE
    Qt6::Core
    Qt6::Multimedia
)

# Install Python server script
install(FILES src/tts_server.py
        DESTINATION ${CMAKE_INSTALL_PREFIX}/src
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
