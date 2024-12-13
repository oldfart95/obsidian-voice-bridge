#!/bin/bash
set -e

# Create build directory
mkdir -p build
cd build

# Configure and build
cmake ..
make -j$(nproc)

# Copy the built module to the project root for testing
cp obsidian_tts_bridge*.so ..

# Run tests
cd ..
python3 -m pip install --user numpy scipy
python3 -m unittest discover tests -v
