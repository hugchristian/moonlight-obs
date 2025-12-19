# Project Roadmap

This document outlines the planned features and improvements for the Moonlight OBS plugin.

## Version 1.0 (Current) - Core Functionality

### Completed ✅
- [x] Basic OBS plugin structure
- [x] Source registration and lifecycle
- [x] Configuration UI (host, port, resolution, FPS, bitrate)
- [x] Video decoder (H.264 via FFmpeg)
- [x] Audio decoder (Opus via FFmpeg)
- [x] Basic connection management
- [x] Texture rendering in OBS
- [x] Audio output to OBS
- [x] Documentation and build system

### In Progress 🔄
- [ ] Complete GameStream protocol implementation
- [ ] Actual network packet reception
- [ ] Connection handshaking with server
- [ ] Proper error handling and recovery

### Planned for 1.0 📋
- [ ] Full working connection to GameStream/Sunshine
- [ ] Stable video and audio streaming
- [ ] Basic error messages to user
- [ ] Performance optimization

## Version 1.1 - Stability & Polish

### Features
- [ ] Connection status indicator
- [ ] Automatic reconnection on disconnect
- [ ] Better error messages and user feedback
- [ ] Connection quality indicator
- [ ] Stream statistics display (bitrate, latency, dropped frames)
- [ ] Improved logging and diagnostics

### Bug Fixes
- [ ] Memory leak fixes
- [ ] Thread safety improvements
- [ ] Edge case handling
- [ ] Resource cleanup improvements

### Performance
- [ ] Optimize decoder pipeline
- [ ] Reduce memory allocations
- [ ] Better buffer management
- [ ] CPU usage optimization

## Version 1.2 - Enhanced Features

### Hardware Acceleration
- [ ] NVDEC support (NVIDIA GPUs)
- [ ] Intel Quick Sync support
- [ ] VA-API support (Linux)
- [ ] VideoToolbox support (macOS)
- [ ] AMD VCE support

### Quality Improvements
- [ ] H.265/HEVC codec support
- [ ] Adaptive bitrate based on network
- [ ] Quality presets (Low, Medium, High, Ultra)
- [ ] Custom codec parameters
- [ ] Better color space handling

### User Experience
- [ ] Connection wizard/setup assistant
- [ ] Server discovery (mDNS/UPnP)
- [ ] Save connection presets
- [ ] Quick connect from recent hosts
- [ ] Visual feedback during connection

## Version 2.0 - Advanced Features

### Input Forwarding
- [ ] Keyboard input to server
- [ ] Mouse input to server
- [ ] Gamepad input forwarding
- [ ] Touch input support
- [ ] Clipboard synchronization

### Multi-Stream Support
- [ ] Multiple simultaneous connections
- [ ] Different apps from same server
- [ ] Multiple servers at once
- [ ] Stream synchronization
- [ ] Resource sharing between streams

### Advanced Configuration
- [ ] Network buffer tuning
- [ ] Decoder thread configuration
- [ ] Custom stream parameters
- [ ] Advanced codec settings
- [ ] Performance profiling tools

## Version 2.1 - Professional Features

### Recording Optimization
- [ ] Lossless capture mode
- [ ] High bitrate recording mode
- [ ] Multi-track audio support
- [ ] Metadata embedding
- [ ] Timecode synchronization

### Collaboration Features
- [ ] Multi-user support
- [ ] Session sharing
- [ ] Remote control permissions
- [ ] Annotation tools
- [ ] Recording markers

### Integration
- [ ] REST API for external control
- [ ] Scripting support
- [ ] WebSocket status updates
- [ ] Plugin system for extensions
- [ ] Third-party integration hooks

## Future Considerations (3.0+)

### Cloud Gaming Support
- [ ] Cloud gaming service integration
- [ ] Low-latency protocol optimizations
- [ ] CDN-aware routing
- [ ] Quality adaptation algorithms

### AI/ML Features
- [ ] Automatic quality adjustment
- [ ] Predictive buffering
- [ ] Smart connection routing
- [ ] Content-aware encoding

### Platform Expansion
- [ ] Mobile companion app
- [ ] Web-based configuration
- [ ] Browser extension
- [ ] Standalone viewer application

### Advanced Graphics
- [ ] HDR support
- [ ] 10-bit color depth
- [ ] Variable refresh rate sync
- [ ] Multi-display capture
- [ ] 360° video support

## Community Requests

Track popular feature requests from the community:

1. **Hardware Decoding** (High Priority)
   - Status: Planned for v1.2
   - Votes: TBD

2. **Input Forwarding** (High Priority)
   - Status: Planned for v2.0
   - Votes: TBD

3. **HEVC Support** (Medium Priority)
   - Status: Planned for v1.2
   - Votes: TBD

4. **Server Discovery** (Medium Priority)
   - Status: Planned for v1.2
   - Votes: TBD

5. **HDR Support** (Low Priority)
   - Status: Future consideration
   - Votes: TBD

## Contributing to the Roadmap

We welcome community input on the roadmap:

1. **Feature Requests**: Open an issue with the `enhancement` label
2. **Vote**: Use 👍 reactions on existing feature requests
3. **Discuss**: Participate in GitHub Discussions
4. **Contribute**: Submit PRs for features you want to implement

See [CONTRIBUTING.md](CONTRIBUTING.md) for more details.

## Timeline Estimates

| Version | Target Date | Status |
|---------|-------------|--------|
| 1.0     | Q1 2025     | In Development |
| 1.1     | Q2 2025     | Planned |
| 1.2     | Q3 2025     | Planned |
| 2.0     | Q4 2025     | Planned |

*Dates are estimates and subject to change based on development progress and community contributions.*

## Development Priorities

### High Priority
Focus on core functionality and stability:
- Complete protocol implementation
- Reliable connection handling
- Performance optimization
- Bug fixes

### Medium Priority
Enhance user experience:
- Hardware acceleration
- Better error handling
- Connection management
- Quality improvements

### Low Priority
Advanced features:
- Input forwarding
- Multi-stream support
- Professional features
- Experimental features

## Breaking Changes

We aim to minimize breaking changes, but some may be necessary:

### Planned Breaking Changes
- None currently planned for 1.x releases

### Future Breaking Changes (2.0+)
- Configuration format may change
- API changes for extensions
- Protocol updates for new features

All breaking changes will be:
- Announced well in advance
- Documented in release notes
- Accompanied by migration guides
- Phased in when possible

## Deprecation Policy

- **Minor versions**: New deprecations announced
- **Major versions**: Deprecated features removed
- **Notice period**: Minimum 2 releases
- **Documentation**: Clear migration paths provided

## Release Schedule

### Regular Releases
- **Patch releases** (x.x.1): Bug fixes, as needed
- **Minor releases** (x.1.0): New features, quarterly
- **Major releases** (2.0.0): Breaking changes, yearly

### Special Releases
- **Hotfixes**: Critical bugs, as needed
- **Beta releases**: Testing new features
- **Release candidates**: Pre-release testing

## Testing Strategy

### Before Each Release
- [ ] Automated build tests
- [ ] Manual testing on all platforms
- [ ] Performance benchmarking
- [ ] Memory leak detection
- [ ] Connection stability testing
- [ ] Compatibility testing with OBS versions

### Beta Testing Program
- Community beta testers
- Early access to new features
- Feedback collection
- Bug reporting priority

## Documentation Roadmap

### Current
- README with setup instructions
- Building guide
- Integration documentation
- Contributing guide
- FAQ

### Planned
- Video tutorials
- Interactive setup guide
- Troubleshooting flowcharts
- Best practices guide
- Performance tuning guide
- API documentation
- Developer guide

## Success Metrics

We'll measure success by:

1. **Adoption**: Number of users/downloads
2. **Stability**: Crash rate, error frequency
3. **Performance**: CPU usage, latency, quality
4. **Community**: Contributors, issues resolved, engagement
5. **Satisfaction**: User feedback, reviews, ratings

## Long-term Vision

The ultimate goal is to make Moonlight OBS the best solution for:

- **Game streaming**: Low latency, high quality game capture
- **Remote production**: Professional remote content creation
- **Dual PC setups**: Seamless gaming and streaming separation
- **Cloud gaming**: Integration with cloud gaming services
- **Education**: Remote teaching and demonstrations

We want to be:
- **Fast**: Lowest possible latency
- **Reliable**: Stable connections, no crashes
- **Easy**: Simple setup, great documentation
- **Powerful**: Professional features, advanced controls
- **Open**: Transparent development, community-driven

## Get Involved

Help shape the future of Moonlight OBS:

- ⭐ Star the repository
- 🐛 Report bugs
- 💡 Suggest features
- 💬 Join discussions
- 👨‍💻 Contribute code
- 📖 Improve documentation
- 🧪 Beta test new features

Together we can build the best game streaming plugin for OBS!
