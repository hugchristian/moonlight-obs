# Development Container

This directory contains the configuration for a Visual Studio Code Development Container for the Moonlight OBS Plugin project.

## What is a Dev Container?

A development container is a running Docker container with a well-defined tool/runtime stack and its prerequisites. It allows you to use a container as a full-featured development environment.

## Prerequisites

To use this dev container, you need:

- [Visual Studio Code](https://code.visualstudio.com/)
- [Docker Desktop](https://www.docker.com/products/docker-desktop) (Windows/macOS) or Docker Engine (Linux)
- [Dev Containers extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers) for VS Code

## Getting Started

1. **Install Prerequisites**: Make sure you have VS Code, Docker, and the Dev Containers extension installed.

2. **Open in Container**:
   - Open this repository in VS Code
   - Press `F1` or `Ctrl+Shift+P` (Windows/Linux) / `Cmd+Shift+P` (macOS)
   - Type "Dev Containers: Reopen in Container" and select it
   - Wait for the container to build and start (first time may take several minutes)

3. **Start Developing**:
   - Once the container is ready, you'll have a fully configured development environment
   - Open a terminal in VS Code (`Ctrl+`` or `Cmd+`` )
   - All dependencies are pre-installed and ready to use

## What's Included

This dev container includes:

### Base Environment
- Ubuntu 22.04 LTS
- C/C++ development tools (GCC, G++, Make)
- CMake 3.x
- Git with useful configurations

### Dependencies
- FFmpeg development libraries (libavcodec, libavformat, libavutil, libswscale)
- OBS Studio development libraries (libobs-dev) - if available
- pkg-config for library discovery

### Code Quality Tools
- clang-format - Code formatting
- cppcheck - Static analysis
- valgrind - Memory debugging
- gdb - Debugger

### VS Code Extensions
- C/C++ Extension Pack - IntelliSense, debugging, and more
- CMake Tools - CMake integration
- Clang-Format - Code formatting
- GitHub Actions - Workflow editing support
- GitLens - Enhanced Git capabilities

## Building the Project

Inside the dev container:

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Build
make -j$(nproc)

# Run tests (if available)
ctest --output-on-failure
```

## Debugging

The dev container is configured with debugging support:

1. Set breakpoints in your code
2. Press `F5` or use the Run and Debug panel
3. Select "C++ (GDB/LLDB)" configuration
4. The debugger will start with proper symbols and source mapping

## Running Code Quality Tools

### Format code with clang-format
```bash
find src -name "*.c" -o -name "*.h" | xargs clang-format -i
```

### Run static analysis with cppcheck
```bash
cppcheck --enable=all --suppress=missingIncludeSystem src/
```

### Memory checking with valgrind
```bash
valgrind --leak-check=full ./your-binary
```

## Troubleshooting

### OBS Studio Libraries Not Found

If `libobs-dev` is not available in the default repositories, you have two options:

**Option 1: Add OBS Studio PPA** (Ubuntu-based systems)
```bash
sudo add-apt-repository ppa:obsproject/obs-studio
sudo apt-get update
sudo apt-get install libobs-dev
```

**Option 2: Build OBS Studio from source**
```bash
git clone --recursive https://github.com/obsproject/obs-studio.git
cd obs-studio
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make -j$(nproc)
sudo make install
```

Then configure your project with:
```bash
cmake .. -DCMAKE_PREFIX_PATH=/usr/local
```

### Container Fails to Start

- Ensure Docker is running
- Check Docker Desktop settings for sufficient memory (recommended: 4GB+)
- Try rebuilding the container: `Dev Containers: Rebuild Container`

### Extensions Not Working

- Reload VS Code: `Developer: Reload Window`
- Check the Output panel for any extension errors
- Ensure you're connected to the container (bottom-left corner should show "Dev Container: Moonlight OBS Plugin Development")

## Customization

You can customize the dev container by editing:

- `.devcontainer/devcontainer.json` - Main configuration file
- `.devcontainer/setup.sh` - Post-creation setup script

After making changes, rebuild the container using `Dev Containers: Rebuild Container`.

## Resources

- [VS Code Dev Containers Documentation](https://code.visualstudio.com/docs/devcontainers/containers)
- [Dev Container Specification](https://containers.dev/)
- [OBS Studio Plugin Development](https://obsproject.com/docs/plugins.html)
