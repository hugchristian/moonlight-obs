#!/bin/bash
set -e

echo "🚀 Setting up Moonlight OBS Plugin development environment..."

# Update package list
echo "📦 Updating package list..."
sudo apt-get update

# Install build essentials and CMake
echo "🔨 Installing build tools..."
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config

# Install OBS Studio development libraries
echo "📺 Installing OBS Studio development libraries..."
sudo apt-get install -y libobs-dev || {
    echo "⚠️  Warning: libobs-dev not available in default repositories."
    echo "You may need to add the OBS Studio PPA or build from source."
    echo "For now, we'll continue with FFmpeg libraries..."
}

# Install FFmpeg development libraries
echo "🎬 Installing FFmpeg development libraries..."
sudo apt-get install -y \
    libavcodec-dev \
    libavformat-dev \
    libavutil-dev \
    libswscale-dev \
    ffmpeg

# Install code quality tools
echo "🔍 Installing code quality tools..."
sudo apt-get install -y \
    clang-format \
    cppcheck \
    valgrind \
    gdb

# Install additional useful tools
echo "🛠️  Installing additional development tools..."
sudo apt-get install -y \
    curl \
    wget \
    nano \
    vim

# Clean up
echo "🧹 Cleaning up..."
sudo apt-get autoremove -y
sudo apt-get clean

echo "✅ Development environment setup complete!"
echo ""
echo "📝 Next steps:"
echo "   1. Create a build directory: mkdir build && cd build"
echo "   2. Configure CMake: cmake .. -DCMAKE_BUILD_TYPE=Debug"
echo "   3. Build the project: make -j\$(nproc)"
echo "   4. Run tests: ctest --output-on-failure"
echo ""
echo "⚠️  Note: If libobs-dev installation failed, you may need to:"
echo "   - Add the OBS Studio PPA: sudo add-apt-repository ppa:obsproject/obs-studio"
echo "   - Or build OBS Studio from source and set CMAKE_PREFIX_PATH"
