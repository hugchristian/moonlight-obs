# Building Moonlight OBS Plugin

This document provides detailed instructions for building the Moonlight OBS plugin on different platforms.

## Prerequisites

### All Platforms

- CMake 3.16 or later
- C/C++ compiler (GCC, Clang, or MSVC)
- OBS Studio development files
- FFmpeg development libraries

### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install build-essential cmake git
sudo apt-get install libobs-dev
sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev libswscale-dev
```

### Linux (Fedora/RHEL)

```bash
sudo dnf install cmake gcc gcc-c++ git
sudo dnf install obs-studio-devel
sudo dnf install ffmpeg-devel
```

### macOS

Install Xcode Command Line Tools and Homebrew:

```bash
xcode-select --install
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Install dependencies:

```bash
brew install cmake
brew install obs
brew install ffmpeg
```

### Windows

1. Install Visual Studio 2022 with C++ development tools
2. Install CMake from https://cmake.org/download/
3. Download OBS Studio development files from https://obsproject.com/
4. Download FFmpeg development libraries from https://ffmpeg.org/download.html

## Building

### Linux

```bash
# Clone the repository
git clone https://github.com/gebhug/-moonlight-obs.git
cd -moonlight-obs

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DCMAKE_BUILD_TYPE=Release

# Build
make -j$(nproc)

# Install (requires sudo)
sudo make install
```

### macOS

```bash
# Clone the repository
git clone https://github.com/gebhug/-moonlight-obs.git
cd -moonlight-obs

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. \
    -DCMAKE_INSTALL_PREFIX=/Applications/OBS.app/Contents \
    -DCMAKE_BUILD_TYPE=Release

# Build
make -j$(sysctl -n hw.ncpu)

# Install
make install
```

### Windows

Using Visual Studio Developer Command Prompt:

```cmd
REM Clone the repository
git clone https://github.com/gebhug/-moonlight-obs.git
cd -moonlight-obs

REM Create build directory
mkdir build
cd build

REM Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64 ^
    -DCMAKE_PREFIX_PATH="C:\path\to\obs-studio;C:\path\to\ffmpeg"

REM Build
cmake --build . --config Release

REM Install
cmake --install . --config Release
```

## Troubleshooting

### libobs not found

If CMake cannot find OBS Studio libraries, specify the path manually:

```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/obs-studio
```

### FFmpeg not found

If CMake cannot find FFmpeg, install the development packages or specify the path:

```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/ffmpeg
```

### Build errors

Make sure you have all required dependencies installed. Check the error message carefully and install any missing libraries.

### Permission denied during installation

Use `sudo` on Linux/macOS or run Command Prompt as Administrator on Windows.

## Development Build

For development, you may want to build with debug symbols:

```bash
mkdir build-debug
cd build-debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```

To run OBS with your development plugin:

```bash
# Linux
OBS_PLUGINS_PATH=/path/to/build-debug obs

# macOS
OBS_PLUGINS_PATH=/path/to/build-debug /Applications/OBS.app/Contents/MacOS/obs
```

## Custom Installation Path

To install to a custom location:

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/custom/path
make install
```

The plugin will be installed to:
- Linux: `<prefix>/lib/obs-plugins/`
- macOS: `<prefix>/obs-plugins/`
- Windows: `<prefix>/obs-plugins/64bit/`

## Uninstalling

To remove the plugin:

### Linux
```bash
sudo rm /usr/lib/obs-plugins/moonlight-obs.so
sudo rm -rf /usr/share/obs/obs-plugins/moonlight-obs
```

### macOS
```bash
rm /Applications/OBS.app/Contents/PlugIns/moonlight-obs.so
rm -rf /Applications/OBS.app/Contents/Resources/obs-plugins/moonlight-obs
```

### Windows
Delete the plugin files from the OBS installation directory:
```
C:\Program Files\obs-studio\obs-plugins\64bit\moonlight-obs.dll
C:\Program Files\obs-studio\data\obs-plugins\moonlight-obs\
```
