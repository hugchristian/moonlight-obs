# Frequently Asked Questions

## General Questions

### What is Moonlight OBS?

Moonlight OBS is a plugin for OBS Studio that allows you to receive video and audio streams from NVIDIA GameStream and Sunshine servers directly as a source in OBS. This enables you to capture gameplay from remote PCs or use a dual-PC streaming setup without additional capture hardware.

### How is this different from NDI or capture cards?

- **vs NDI**: Moonlight uses the GameStream protocol which is optimized for gaming with lower latency. It's also built into NVIDIA GPUs and available via Sunshine.
- **vs Capture Cards**: No additional hardware needed. Works over network with any GameStream/Sunshine capable machine.

### Do I need an NVIDIA GPU?

For the streaming server (the PC you're capturing from):
- NVIDIA GPU with GameStream support, OR
- Any system running Sunshine (works with AMD, Intel, or NVIDIA)

For the OBS machine (where this plugin runs):
- Any GPU that can run OBS Studio

### Is this free?

Yes, completely free and open-source under the MIT license.

## Setup Questions

### What do I need to install?

On the **streaming PC** (game source):
- Either NVIDIA GeForce Experience with GameStream enabled, OR
- Sunshine server

On the **OBS PC** (capture destination):
- OBS Studio 28.0+
- This Moonlight OBS plugin

### How do I enable GameStream on NVIDIA?

1. Install NVIDIA GeForce Experience
2. Sign in with your NVIDIA account
3. Go to Settings → SHIELD
4. Enable "GAMESTREAM"

### How do I install Sunshine?

1. Download from https://github.com/LizardByte/Sunshine
2. Install on the PC you want to stream from
3. Configure applications and settings
4. Start the Sunshine service

### Can both PCs be the same machine?

Yes! You can use `localhost` as the host to stream from the same machine. This is useful for:
- Isolating game capture
- Using game-specific optimizations
- Reducing CPU load on OBS itself

## Configuration Questions

### What's the best bitrate?

It depends on your use case:
- **Local/Same PC**: 30,000-50,000 Kbps
- **Gigabit LAN**: 20,000-30,000 Kbps
- **100 Mbps LAN**: 15,000-20,000 Kbps
- **WiFi**: 10,000-15,000 Kbps
- **Remote/Internet**: 5,000-10,000 Kbps

Start with 20,000 Kbps and adjust based on quality and performance.

### What resolution should I use?

Match your game/source resolution:
- **1080p displays**: 1920x1080
- **1440p displays**: 2560x1440
- **4K displays**: 3840x2160

You can use lower resolution to reduce bandwidth/CPU usage.

### What FPS should I use?

Match your game framerate or OBS output:
- **30 FPS**: Desktop work, non-gaming content
- **60 FPS**: Most gaming, standard streaming
- **120 FPS**: High refresh rate gaming (requires powerful hardware)

### How do I find my PC's IP address?

**Windows**:
```cmd
ipconfig
```
Look for "IPv4 Address" under your network adapter.

**Linux**:
```bash
ip addr show
```

**macOS**:
```bash
ifconfig
```

## Troubleshooting

### "Cannot connect to server"

**Check these items:**
1. Is GameStream/Sunshine running on the host PC?
2. Is the IP address correct?
3. Is port 47989 open in the firewall?
4. Are both PCs on the same network (if not using port forwarding)?
5. Can you ping the host PC?

**Test connection:**
```bash
ping [host_ip]
telnet [host_ip] 47989
```

### "No video displayed"

**Possible causes:**
1. **Codec not supported**: Ensure FFmpeg with H.264 support is installed
2. **Resolution mismatch**: Try a standard resolution like 1920x1080
3. **Server not streaming**: Check if the application is running on the host
4. **Decoder error**: Check OBS logs for error messages

**Solutions:**
- Restart OBS
- Try a lower resolution
- Check OBS logs for specific errors
- Verify FFmpeg installation

### "Audio not working"

**Possible causes:**
1. **Codec not available**: Opus codec not in FFmpeg
2. **Audio mixer muted**: Check OBS audio mixer
3. **Server audio disabled**: Check GameStream/Sunshine audio settings

**Solutions:**
- Check OBS audio mixer settings
- Ensure audio source is selected in OBS
- Verify FFmpeg has Opus support
- Check server audio configuration

### "Stuttering or lag"

**Possible causes:**
1. **Network congestion**: Too much traffic on network
2. **Bitrate too high**: Exceeding network capacity
3. **CPU overload**: Decoding taking too much CPU
4. **Insufficient bandwidth**: Network too slow

**Solutions:**
- Reduce bitrate (try 10,000 Kbps)
- Lower resolution (try 1280x720)
- Reduce FPS (try 30 FPS)
- Use wired connection instead of WiFi
- Close other network applications

### "High latency"

**To reduce latency:**
1. Use wired Ethernet connection
2. Reduce resolution
3. Reduce bitrate (to a point)
4. Ensure minimal network hops
5. Use same subnet if possible

### Plugin doesn't appear in OBS

**Check:**
1. Plugin installed in correct location
2. OBS version is 28.0 or later
3. Check OBS logs for loading errors
4. FFmpeg libraries are available

**Reinstall:**
```bash
# Uninstall
sudo rm /usr/lib/obs-plugins/moonlight-obs.so

# Reinstall
cd build
sudo make install
```

## Performance Questions

### How much CPU does this use?

Depends on resolution and codec:
- **720p30**: ~5-10% CPU (1 core)
- **1080p60**: ~10-20% CPU (1-2 cores)
- **4K60**: ~30-50% CPU (2-4 cores)

Hardware-accelerated decoding reduces CPU usage significantly (future feature).

### How much network bandwidth?

Slightly more than the configured bitrate due to protocol overhead:
- **10 Mbps bitrate**: ~12-13 Mbps network usage
- **20 Mbps bitrate**: ~24-26 Mbps network usage
- **50 Mbps bitrate**: ~60-65 Mbps network usage

### Can I use this over the Internet?

Yes, but:
- **Latency**: Will be higher than LAN
- **Bandwidth**: Limited by upload/download speeds
- **Security**: Consider using VPN
- **Port forwarding**: May need to forward port 47989

Not recommended for real-time streaming, but works for remote content creation.

### Does this support 4K?

Yes, the plugin supports resolutions up to 4K (3840x2160). Requirements:
- Powerful CPU for software decoding
- High bitrate (50,000+ Kbps recommended)
- Gigabit+ network connection
- Sufficient GPU on host PC

## Feature Questions

### Can I send keyboard/mouse input back?

Not in the current version. This is a planned feature for future releases.

### Does it support HDR?

Not currently. HDR support may be added in a future version depending on GameStream/Sunshine protocol support.

### Can I use multiple sources?

Yes! You can add multiple Moonlight sources in OBS, each connecting to different hosts or applications.

### Does it work with remote play services?

The plugin is designed for GameStream/Sunshine. For other services:
- **Steam Remote Play**: No (different protocol)
- **Parsec**: No (different protocol)
- **AMD Link**: No (different protocol)

### Will you add hardware decoding?

Yes, this is planned for a future release. It will significantly reduce CPU usage:
- NVDEC (NVIDIA GPUs)
- Quick Sync (Intel GPUs)
- VA-API (Linux)
- VideoToolbox (macOS)

## Compatibility Questions

### Which OBS versions are supported?

OBS Studio 28.0 or later. Recommended: latest stable version.

### Which operating systems are supported?

- **Linux**: Yes (Ubuntu, Fedora, Arch, etc.)
- **Windows**: Yes (Windows 10/11)
- **macOS**: Yes (macOS 10.15+)

### Which GameStream versions?

Any version of NVIDIA GameStream or Sunshine that supports the standard protocol.

### Can I use this with OBS Studio forks?

Should work with any OBS Studio fork that maintains plugin API compatibility (e.g., StreamFX OBS).

## Advanced Questions

### How can I reduce latency even more?

1. Use localhost if same machine
2. Use wired Gigabit connection
3. Optimize network path (same switch)
4. Reduce buffering on server side
5. Use lower resolution/FPS
6. Enable hardware decoding (when available)

### Can I customize the stream protocol?

The plugin uses the standard GameStream protocol. Custom protocol modifications would require changes to both the plugin and the server (Sunshine).

### How do I enable debug logging?

Edit `src/plugin-main.h` and change:
```c
#define blog(log_level, format, ...) \
    blog(LOG_DEBUG, "[" PLUGIN_NAME "] " format, ##__VA_ARGS__)
```

Then rebuild and check OBS logs.

### Can I contribute to development?

Absolutely! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## Getting Help

### Where can I get support?

1. **GitHub Issues**: Report bugs and request features
2. **GitHub Discussions**: Ask questions and share tips
3. **Documentation**: Read the technical docs (INTEGRATION.md)

### How do I report a bug?

Open an issue on GitHub with:
1. Description of the problem
2. Steps to reproduce
3. OBS version
4. Operating system
5. Plugin version
6. Relevant log excerpts

### Where are the logs?

**OBS logs location:**
- **Windows**: `%APPDATA%\obs-studio\logs`
- **Linux**: `~/.config/obs-studio/logs`
- **macOS**: `~/Library/Application Support/obs-studio/logs`

Look for lines containing `[moonlight-obs]`.
