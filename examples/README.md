# Configuration Examples

This directory contains example configurations for various use cases.

## Quick Start Examples

### Local Streaming (Same Machine)

**Use Case**: Capture games running on the same machine

```
Host: localhost
Port: 47989
Application: Desktop
Width: 1920
Height: 1080
FPS: 60
Bitrate: 20000
```

### LAN Gaming PC

**Use Case**: Capture from gaming PC on local network

```
Host: 192.168.1.100
Port: 47989
Application: Steam
Width: 1920
Height: 1080
FPS: 60
Bitrate: 30000
```

## Quality Presets

### Ultra Quality (4K60)

**Requirements**: High-end PC, good network (100+ Mbps)

```
Width: 3840
Height: 2160
FPS: 60
Bitrate: 80000
```

**Best For**:
- Recording content
- Maximum quality streaming
- High-bandwidth networks

### High Quality (1080p60)

**Requirements**: Mid-range PC, good network (30+ Mbps)

```
Width: 1920
Height: 1080
FPS: 60
Bitrate: 20000
```

**Best For**:
- Standard streaming
- Gaming content
- Most use cases

### Balanced (1080p30)

**Requirements**: Any modern PC, standard network (15+ Mbps)

```
Width: 1920
Height: 1080
FPS: 30
Bitrate: 10000
```

**Best For**:
- Slower networks
- CPU-limited systems
- Non-gaming content

### Performance (720p60)

**Requirements**: Lower-end PC, slower network (10+ Mbps)

```
Width: 1280
Height: 720
FPS: 60
Bitrate: 8000
```

**Best For**:
- Fast-paced gaming
- Limited bandwidth
- Resource-constrained systems

### Low Latency (720p120)

**Requirements**: High-end PC, excellent network (25+ Mbps)

```
Width: 1280
Height: 720
FPS: 120
Bitrate: 15000
```

**Best For**:
- Competitive gaming
- Reaction-based content
- High frame rate displays

## Use Case Scenarios

### 1. Local Game Capture

**Scenario**: Streaming games from the same PC

```
Host: localhost
Port: 47989
Application: Desktop
Width: 1920
Height: 1080
FPS: 60
Bitrate: 40000
```

**Notes**:
- Higher bitrate possible (no network bottleneck)
- Lower latency
- Consider impact on game performance

### 2. Dual PC Streaming Setup

**Scenario**: Gaming PC + streaming PC

```
Gaming PC (Sunshine Server):
- IP: 192.168.1.100
- Streaming "Desktop" or specific games

Streaming PC (OBS):
Host: 192.168.1.100
Port: 47989
Application: Desktop
Width: 1920
Height: 1080
FPS: 60
Bitrate: 30000
```

**Benefits**:
- No performance impact on gaming PC
- Dedicated resources for encoding/streaming
- Professional setup

### 3. Remote Game Capture

**Scenario**: Capturing from a remote gaming PC

```
Host: remote.example.com
Port: 47989
Application: Steam
Width: 1280
Height: 720
FPS: 30
Bitrate: 5000
```

**Considerations**:
- Network latency
- Bandwidth limitations
- VPN if needed for security

### 4. Console Capture (via PC)

**Scenario**: Console connected to PC running Sunshine

```
Host: 192.168.1.100
Port: 47989
Application: Desktop
Width: 1920
Height: 1080
FPS: 60
Bitrate: 25000
```

**Setup**:
- Console → Capture card → PC
- PC running Sunshine broadcasting capture card
- OBS receiving via Moonlight plugin

### 5. Multi-Source Setup

**Scenario**: Multiple game sources

```
Source 1 (Main Game):
Host: 192.168.1.100
Application: Game1
Width: 1920
Height: 1080
FPS: 60
Bitrate: 20000

Source 2 (Secondary):
Host: 192.168.1.101
Application: Game2
Width: 1280
Height: 720
FPS: 30
Bitrate: 10000
```

**Use Cases**:
- Multi-game streaming
- Picture-in-picture layouts
- Comparison videos

## Application Names

Common application names for GameStream/Sunshine:

- `Desktop` - Full desktop capture
- `Steam` - Steam Big Picture mode
- `mstsc.exe` - Remote Desktop
- Custom applications (as configured in Sunshine)

## Network Bandwidth Guide

| Quality          | Resolution | FPS | Bitrate | Bandwidth Needed |
|------------------|------------|-----|---------|------------------|
| Ultra            | 4K         | 60  | 80 Mbps | 100+ Mbps        |
| Very High        | 4K         | 30  | 50 Mbps | 60+ Mbps         |
| High             | 1080p      | 60  | 20 Mbps | 30+ Mbps         |
| Medium           | 1080p      | 30  | 10 Mbps | 15+ Mbps         |
| Low              | 720p       | 60  | 8 Mbps  | 10+ Mbps         |
| Very Low         | 720p       | 30  | 5 Mbps  | 8+ Mbps          |

*Add 20-30% overhead for network protocols and packet overhead*

## Performance Tips

### CPU Usage
- Lower resolution reduces CPU load
- Lower FPS reduces CPU load
- Hardware decoding helps (if available)

### Network Optimization
- Use wired connection when possible
- Enable QoS on router for gaming traffic
- Reduce bitrate on congested networks

### Quality Optimization
- Higher bitrate = better quality (up to a point)
- Match FPS to source content
- 60 FPS for gaming, 30 FPS for desktop work

## Troubleshooting Configurations

### High Latency

Try:
```
Width: 1280
Height: 720
FPS: 60
Bitrate: 10000
```

### Stuttering/Buffering

Try:
```
Width: 1920
Height: 1080
FPS: 30
Bitrate: 8000
```

### Poor Quality

Try:
```
Width: 1920
Height: 1080
FPS: 60
Bitrate: 30000
```

## Testing Configuration

Start with this baseline and adjust:

```
Host: [YOUR_SERVER_IP]
Port: 47989
Application: Desktop
Width: 1280
Height: 720
FPS: 30
Bitrate: 10000
```

Then gradually increase:
1. Resolution → 1920x1080
2. Bitrate → 15000
3. FPS → 60
4. Bitrate → 20000+

Stop when you reach quality/performance limits.
