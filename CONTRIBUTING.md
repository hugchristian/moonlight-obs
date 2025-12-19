# Contributing to Moonlight OBS Plugin

Thank you for your interest in contributing to the Moonlight OBS plugin! This document provides guidelines and information for contributors.

## Code of Conduct

Be respectful and considerate in all interactions. We aim to maintain a welcoming and inclusive community.

## How to Contribute

### Reporting Bugs

When reporting bugs, please include:

1. **Description**: Clear description of the issue
2. **Steps to Reproduce**: Detailed steps to reproduce the problem
3. **Expected Behavior**: What you expected to happen
4. **Actual Behavior**: What actually happened
5. **Environment**:
   - OBS Studio version
   - Operating system and version
   - Plugin version
   - GameStream/Sunshine server version
6. **Logs**: Relevant log excerpts from OBS

### Suggesting Features

Feature requests are welcome! Please:

1. Check if the feature has already been requested
2. Describe the feature and its use case
3. Explain how it would benefit users
4. Consider implementation complexity

### Pull Requests

1. **Fork the repository** and create a feature branch
2. **Follow coding style** (see below)
3. **Write clear commit messages**
4. **Test your changes** thoroughly
5. **Update documentation** if needed
6. **Submit the pull request** with a clear description

## Development Setup

### Prerequisites

See [BUILDING.md](BUILDING.md) for detailed build instructions.

### Development Workflow

```bash
# Fork and clone
git clone https://github.com/YOUR_USERNAME/-moonlight-obs.git
cd -moonlight-obs

# Create feature branch
git checkout -b feature/my-new-feature

# Make changes
# ... edit files ...

# Build and test
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make

# Test with OBS
OBS_PLUGINS_PATH=$(pwd) obs

# Commit changes
git add .
git commit -m "Add feature: description"

# Push to your fork
git push origin feature/my-new-feature

# Create pull request on GitHub
```

## Coding Style

### C Code Style

We follow a style similar to OBS Studio:

```c
// Use tabs for indentation
// Opening braces on same line for functions
void function_name(int param)
{
	// Code here
}

// Use snake_case for functions and variables
int my_variable = 0;
void my_function(void);

// Use descriptive names
struct moonlight_source *source;  // Good
struct ml_src *src;                // Avoid

// Add spaces around operators
int result = a + b;

// Use proper header guards
#pragma once

// Comment complex logic
// Calculate the frame timestamp based on server time
uint64_t timestamp = convert_time(server_time);
```

### Code Organization

- **Keep functions focused**: Each function should do one thing well
- **Limit function length**: Aim for under 50 lines per function
- **Use meaningful names**: Variables and functions should be self-documenting
- **Error handling**: Always check return values and handle errors
- **Resource cleanup**: Ensure all resources are properly freed

### Memory Management

```c
// Use OBS memory functions
void *data = bmalloc(size);
void *data = bzalloc(size);  // Zero-initialized
bfree(data);

// Always check allocations
struct my_data *data = bzalloc(sizeof(*data));
if (!data) {
	blog(LOG_ERROR, "Failed to allocate memory");
	return NULL;
}

// Free in reverse order of allocation
// Initialize mutexes before use
pthread_mutex_init(&data->mutex, NULL);

// Clean up in opposite order
pthread_mutex_destroy(&data->mutex);
bfree(data);
```

### Threading

```c
// Protect shared data with mutexes
pthread_mutex_lock(&context->mutex);
// Access shared data
pthread_mutex_unlock(&context->mutex);

// Use proper thread lifecycle
pthread_create(&thread, NULL, thread_func, data);
// ... work ...
pthread_join(thread, NULL);
```

### Logging

```c
// Use appropriate log levels
blog(LOG_ERROR, "Critical error: %s", error_msg);
blog(LOG_WARNING, "Warning: %s", warning_msg);
blog(LOG_INFO, "Information: %s", info_msg);
blog(LOG_DEBUG, "Debug: value=%d", value);

// Include context in messages
blog(LOG_INFO, "Connected to %s:%d", host, port);
```

## Testing

### Manual Testing

Before submitting a PR, test the following:

1. **Plugin Loading**
   - Plugin loads without errors
   - Shows up in OBS source list

2. **Configuration**
   - Properties dialog opens
   - All settings can be changed
   - Changes are applied correctly

3. **Streaming**
   - Connects to server
   - Video displays correctly
   - Audio plays correctly
   - Can start/stop multiple times

4. **Error Handling**
   - Invalid host/port handled gracefully
   - Server unavailable handled gracefully
   - Network errors don't crash OBS

5. **Resource Cleanup**
   - No memory leaks (use valgrind)
   - Clean shutdown without crashes

### Testing Tools

```bash
# Memory leak detection (Linux)
valgrind --leak-check=full obs

# Address sanitizer (compile with)
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DCMAKE_C_FLAGS="-fsanitize=address -g"

# Thread sanitizer
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DCMAKE_C_FLAGS="-fsanitize=thread -g"
```

## Documentation

### Code Documentation

```c
/**
 * Brief description of the function
 * 
 * Detailed description if needed.
 * Can span multiple lines.
 * 
 * @param source The moonlight source context
 * @param data Video frame data
 * @param size Size of data in bytes
 * @return true on success, false on failure
 */
bool video_decoder_decode(struct video_decoder *decoder, 
                          uint8_t *data, size_t size);
```

### User Documentation

When adding features, update:
- README.md - User-facing features
- BUILDING.md - Build instructions if changed
- INTEGRATION.md - Technical details

## Commit Messages

Use clear, descriptive commit messages:

```
Add feature: video bitrate control

- Add UI slider for bitrate configuration
- Implement bitrate adjustment in encoder
- Update documentation

Fixes #123
```

Format:
- **First line**: Brief summary (50 chars or less)
- **Blank line**
- **Body**: Detailed description if needed
- **References**: Issue numbers, related PRs

## Review Process

1. **Automated checks**: Code must pass CI builds
2. **Code review**: Maintainer will review the code
3. **Testing**: Changes should be tested
4. **Documentation**: Updates should be documented
5. **Approval**: Approved PRs will be merged

## Areas for Contribution

### High Priority

1. **Moonlight Protocol Implementation**
   - Complete GameStream protocol client
   - Implement connection handshake
   - Add packet parsing and handling

2. **Hardware Acceleration**
   - NVDEC support for NVIDIA GPUs
   - VA-API support for Linux
   - VideoToolbox support for macOS

3. **Input Forwarding**
   - Capture keyboard/mouse input
   - Send to GameStream server
   - Implement proper focus handling

### Medium Priority

1. **HEVC Support**
   - Add H.265 decoder option
   - Negotiate codec with server

2. **Connection Status**
   - Display stream statistics
   - Show connection quality
   - Network latency indicator

3. **Error Recovery**
   - Automatic reconnection
   - Better error messages
   - Graceful degradation

### Nice to Have

1. **Multiple Streams**
   - Support multiple sources
   - Different apps simultaneously

2. **Recording Integration**
   - Optimize for recording
   - Better quality presets

3. **Advanced Settings**
   - Codec parameters tuning
   - Network buffer configuration
   - Performance optimizations

## Getting Help

- **Issues**: Check existing issues on GitHub
- **Discussions**: Use GitHub Discussions for questions
- **Documentation**: Read the technical documentation

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Recognition

Contributors will be:
- Listed in CONTRIBUTORS.md
- Credited in release notes
- Mentioned in significant feature announcements

Thank you for contributing!
