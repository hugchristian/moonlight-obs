# Moonlight OBS Plugin

Receive NVIDIA GameStream and Sunshine video and audio streams directly in OBS Studio.

## Overview

This plugin adds a new source type to OBS Studio that allows you to receive video and audio from NVIDIA GameStream and Sunshine game streaming servers. It uses the Moonlight streaming protocol to connect to your gaming PC and capture the stream directly in OBS.

## Features

- **Direct GameStream Integration**: Connect to NVIDIA GameStream or Sunshine servers
- **High-Quality Streaming**: Support for H.264/H.265 video up to 4K resolution
- **Low Latency Audio**: Opus audio codec support for high-quality, low-latency audio
- **Configurable Parameters**: Adjust resolution, framerate, and bitrate to match your needs
- **Easy Setup**: Simple configuration through OBS source properties

## Requirements

- OBS Studio 28.0 or later
- CMake 3.16 or later
- FFmpeg libraries (avcodec, avformat, avutil, swscale)
- A GameStream-enabled PC or Sunshine server

## Building

### Linux

```bash
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr
make
sudo make install
```

### macOS

```bash
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/Applications/OBS.app/Contents
make
make install
```

### Windows

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
cmake --install . --config Release
```

## Usage

1. **Add the Source**:
   - Open OBS Studio
   - Click the + button in the Sources panel
   - Select "Moonlight GameStream"

2. **Configure Connection**:
   - **Host**: IP address or hostname of your GameStream/Sunshine server
   - **Port**: Port number (default: 47989)
   - **Application Name**: Name of the application to stream (e.g., "Steam", "Desktop")

3. **Configure Stream Quality**:
   - **Width**: Video width (e.g., 1920)
   - **Height**: Video height (e.g., 1080)
   - **FPS**: Target framerate (e.g., 60)
   - **Bitrate**: Stream bitrate in Kbps (e.g., 20000 for 20 Mbps)

4. **Start Streaming**:
   - Make the source visible in your scene
   - The plugin will automatically connect and start receiving the stream

## Configuration Examples

### 1080p60 Gaming
- Width: 1920
- Height: 1080
- FPS: 60
- Bitrate: 20000 Kbps

### 4K30 High Quality
- Width: 3840
- Height: 2160
- FPS: 30
- Bitrate: 50000 Kbps

### 720p120 High Framerate
- Width: 1280
- Height: 720
- FPS: 120
- Bitrate: 15000 Kbps

## Troubleshooting

### Cannot Connect to Server
- Verify the host IP address is correct
- Check that the GameStream/Sunshine server is running
- Ensure port 47989 is not blocked by firewall
- Verify network connectivity between OBS and the server

### No Video Displayed
- Check OBS logs for decoder errors
- Verify the resolution and FPS settings match server capabilities
- Try reducing bitrate if experiencing network issues

### Audio Not Working
- Ensure the Opus codec is available in your FFmpeg installation
- Check OBS audio mixer settings
- Verify audio is enabled on the GameStream/Sunshine server

## Architecture

The plugin consists of several key components:

1. **Plugin Main** (`plugin-main.c`): OBS plugin registration and lifecycle management
2. **Moonlight Source** (`moonlight-source.c`): OBS source implementation and UI
3. **Moonlight Client** (`moonlight-client.c`): GameStream protocol client and connection management
4. **Video Decoder** (`video-decoder.c`): H.264/H.265 video decoding using FFmpeg
5. **Audio Decoder** (`audio-decoder.c`): Opus audio decoding using FFmpeg

## Development

### Project Structure

```
moonlight-obs/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── LICENSE                 # MIT License
├── src/                    # Source code
│   ├── plugin-main.c       # Plugin entry point
│   ├── moonlight-source.c  # OBS source implementation
│   ├── moonlight-client.c  # Streaming client
│   ├── video-decoder.c     # Video decoder
│   └── audio-decoder.c     # Audio decoder
└── data/                   # Plugin data
    └── locale/             # Translations
        └── en-US.ini       # English strings
```

### Building for Development

```bash
# Debug build
mkdir build-debug
cd build-debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```

### Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [Moonlight](https://github.com/moonlight-stream) - Open-source game streaming protocol
- [OBS Studio](https://obsproject.com/) - Free and open-source streaming software
- [FFmpeg](https://ffmpeg.org/) - Multimedia framework for decoding

## Related Projects

- [Moonlight Qt](https://github.com/moonlight-stream/moonlight-qt) - Moonlight game streaming client
- [Sunshine](https://github.com/LizardByte/Sunshine) - Self-hosted game streaming server
- [DistroAV](https://gitlab.com/fzwoch/obs-distroav) - Similar OBS plugin for NDI streaming
