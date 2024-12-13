# Obsidian Voice Bridge

⚠️ **UNSTABLE DEVELOPMENT BUILD** - This project is currently in early development and should not be used in production environments.

A C++ bridge for integrating Coqui TTS with Obsidian markdown processing. This bridge enables high-performance text-to-speech conversion of markdown content using Python's TTS library through a C++ interface.

## Development Status

This is an unstable development build. Features may be incomplete, APIs may change without notice, and there might be significant bugs. Use at your own risk.

## Features

- High-performance C++ to Python bridge for TTS functionality
- Support for Coqui TTS models
- Markdown text processing capabilities
- Efficient audio data handling
- Cross-platform compatibility

## Prerequisites

- CMake 3.15 or higher
- Python 3.9 or higher
- Qt6 (Core and Multimedia components)
- Coqui TTS library
- pybind11

## Building

1. Create a build directory:
```bash
mkdir build && cd build
```

2. Configure with CMake:
```bash
cmake ..
```

3. Build:
```bash
make
```

## Testing

Run the tests using:
```bash
./build_and_test.sh
```

## Project Structure

- `src/` - Source files
  - `tts_bridge.cpp` - Main bridge implementation
  - `coqui_engine.cpp` - TTS engine wrapper
  - `text_processor.cpp` - Markdown processing
  - `tts_server.py` - Python TTS server
- `include/` - Header files
- `tests/` - Test files
- `CMakeLists.txt` - CMake build configuration

## Contributing

As this is an unstable development build, please:
- Create issues for any bugs you find
- Make pull requests for any improvements
- Test thoroughly before submitting changes
- Document any breaking changes

## License

[Insert appropriate license information]
