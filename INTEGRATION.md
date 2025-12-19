# Moonlight Protocol Integration Guide

This document describes how the Moonlight OBS plugin integrates with the GameStream/Sunshine protocol.

## Architecture Overview

```
┌─────────────────┐         ┌──────────────────┐         ┌─────────────┐
│  GameStream/    │         │  Moonlight OBS   │         │  OBS Studio │
│  Sunshine       │◄────────┤  Plugin          │────────►│             │
│  Server         │         │                  │         │             │
└─────────────────┘         └──────────────────┘         └─────────────┘
  │                           │                            │
  ├─ Video Encoder            ├─ Video Decoder            ├─ Rendering
  ├─ Audio Encoder            ├─ Audio Decoder            └─ Audio Mixer
  └─ Network Stream           └─ Network Client
```

## Components

### 1. Plugin Main (`plugin-main.c`)
- **Purpose**: OBS plugin lifecycle management
- **Functions**:
  - `obs_module_load()`: Registers the Moonlight source with OBS
  - `obs_module_unload()`: Cleanup when OBS exits
  - Plugin metadata and identification

### 2. Moonlight Source (`moonlight-source.c`)
- **Purpose**: OBS source implementation
- **Key Callbacks**:
  - `create()`: Initialize source context and state
  - `destroy()`: Cleanup resources
  - `update()`: Apply configuration changes
  - `show()`: Start streaming when source becomes visible
  - `hide()`: Stop streaming when source becomes hidden
  - `video_render()`: Render video frames to OBS canvas
  - `get_properties()`: Define configuration UI

### 3. Moonlight Client (`moonlight-client.c`)
- **Purpose**: GameStream protocol client implementation
- **Responsibilities**:
  - Connection management
  - Stream negotiation
  - Packet reception
  - Frame delivery to decoders

### 4. Video Decoder (`video-decoder.c`)
- **Purpose**: Decode H.264/H.265 video stream
- **Process**:
  1. Receive encoded video packets
  2. Decode using FFmpeg's libavcodec
  3. Convert from YUV420P to RGBA
  4. Update OBS texture with decoded frame

### 5. Audio Decoder (`audio-decoder.c`)
- **Purpose**: Decode Opus audio stream
- **Process**:
  1. Receive encoded audio packets
  2. Decode using FFmpeg's libavcodec
  3. Convert to OBS audio format (float planar)
  4. Output to OBS audio pipeline

## GameStream Protocol Flow

### Connection Establishment

```
Client                          Server
  │                               │
  ├─────── Discovery ────────────►│
  │◄──── Server Info ─────────────┤
  │                               │
  ├────── Pairing ───────────────►│
  │◄──── Pairing OK ──────────────┤
  │                               │
  ├────── App List ──────────────►│
  │◄──── Available Apps ──────────┤
  │                               │
  ├──── Launch App ──────────────►│
  │◄──── Stream Ready ────────────┤
```

### Streaming Session

```
Server                          Client
  │                               │
  ├──── Video Packet ────────────►│
  ├──── Video Packet ────────────►│
  ├──── Audio Packet ────────────►│
  ├──── Video Packet ────────────►│
  │                               │
  │◄──── Input Data ──────────────┤ (Future feature)
  │                               │
  ├──── Video Packet ────────────►│
  ├──── Audio Packet ────────────►│
```

## Data Flow

### Video Path

```
GameStream Server
  │
  └─► H.264/H.265 Encoded Stream
        │
        └─► Network (UDP/TCP)
              │
              └─► Moonlight Client
                    │
                    └─► Video Decoder (FFmpeg)
                          │
                          ├─► YUV420P Frame
                          │
                          └─► SwScale (Color Conversion)
                                │
                                └─► RGBA Frame
                                      │
                                      └─► OBS Texture
                                            │
                                            └─► OBS Rendering Pipeline
```

### Audio Path

```
GameStream Server
  │
  └─► Opus Encoded Stream
        │
        └─► Network (UDP/TCP)
              │
              └─► Moonlight Client
                    │
                    └─► Audio Decoder (FFmpeg)
                          │
                          └─► Float Planar Samples
                                │
                                └─► OBS Audio Output
                                      │
                                      └─► OBS Audio Mixer
```

## Thread Model

### Main Thread (OBS Event Loop)
- Source callbacks (create, destroy, update, render)
- Properties UI
- Texture updates (graphics context required)

### Streaming Thread
- Network I/O
- Packet reception
- Decoder feeding
- Created in `moonlight_client_start()`
- Joined in `moonlight_client_stop()`

### Synchronization
- Mutex protects shared source state
- Video texture updates use `obs_enter_graphics()` / `obs_leave_graphics()`
- Audio output is thread-safe via `obs_source_output_audio()`

## Configuration Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| host | string | localhost | GameStream/Sunshine server IP/hostname |
| port | int | 47989 | Server port |
| app_name | string | Steam | Application to stream |
| width | int | 1920 | Video width in pixels |
| height | int | 1080 | Video height in pixels |
| fps | int | 60 | Target framerate |
| bitrate | int | 20000 | Stream bitrate in Kbps |

## Codec Support

### Video Codecs
- **H.264 (AVC)**: Primary codec, universally supported
- **H.265 (HEVC)**: Optional, higher compression (future)

### Audio Codecs
- **Opus**: Primary codec for low-latency audio
- **AAC**: Fallback option (future)

## Network Protocol

GameStream uses a combination of protocols:

1. **HTTPS** (TCP 47989): Control channel
   - Server discovery
   - Pairing
   - App launching

2. **UDP** (47998-48000): Streaming data
   - Video packets
   - Audio packets
   - Control messages

3. **RTP**: Packet framing
   - Sequence numbers
   - Timestamps
   - Payload identification

## Performance Considerations

### Latency
- Target: <100ms glass-to-glass latency
- Factors:
  - Network latency
  - Encoding delay (server)
  - Decoding delay (client)
  - Rendering delay (OBS)

### Resource Usage
- CPU: Video/audio decoding (FFmpeg)
- GPU: Texture uploads and rendering (OBS)
- Network: Stream bitrate bandwidth
- Memory: Frame buffers and textures

### Optimization Strategies
1. Hardware-accelerated decoding (future enhancement)
2. Zero-copy texture updates where possible
3. Efficient buffer management
4. Adaptive bitrate based on network conditions

## Future Enhancements

### Planned Features
1. **Input Forwarding**: Send keyboard/mouse input back to server
2. **Hardware Decoding**: Use GPU for video decoding
3. **HEVC Support**: Add H.265 codec option
4. **Connection Status**: Display connection quality/statistics
5. **Multiple Streams**: Support multiple concurrent sources
6. **Encryption**: TLS/DTLS for secure streaming
7. **Authentication**: Proper pairing and PIN verification

### Protocol Extensions
1. Custom control messages
2. Enhanced error reporting
3. Quality adaptation
4. Multi-stream synchronization

## Debugging

### Enable Debug Logging

In `plugin-main.h`, change log level:
```c
#define blog(log_level, format, ...) \
    blog(LOG_DEBUG, "[" PLUGIN_NAME "] " format, ##__VA_ARGS__)
```

### Common Issues

1. **Connection Fails**
   - Check network connectivity
   - Verify server is running
   - Check firewall rules

2. **No Video**
   - Verify codec support
   - Check resolution compatibility
   - Review decoder logs

3. **Audio Desync**
   - Check timestamp handling
   - Verify sample rate matching
   - Review audio buffer management

## Testing

### Unit Tests (Future)
- Decoder initialization
- Packet parsing
- Buffer management
- Error handling

### Integration Tests
- Connect to test server
- Stream sample content
- Verify video/audio output
- Performance profiling

### Manual Testing Checklist
- [ ] Plugin loads in OBS
- [ ] Source appears in source list
- [ ] Properties dialog opens
- [ ] Can configure all parameters
- [ ] Connects to server successfully
- [ ] Video displays correctly
- [ ] Audio plays correctly
- [ ] Can start/stop streaming
- [ ] Clean shutdown without crashes

## References

- [Moonlight Protocol Documentation](https://github.com/moonlight-stream/moonlight-docs)
- [OBS Studio Plugin Guide](https://obsproject.com/docs/plugins.html)
- [FFmpeg Documentation](https://ffmpeg.org/documentation.html)
- [GameStream Protocol](https://github.com/moonlight-stream/moonlight-common-c)
