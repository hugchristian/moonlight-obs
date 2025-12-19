#pragma once

#include <stdbool.h>
#include <stdint.h>

// Forward declarations
struct moonlight_source;

// Moonlight client structure
struct moonlight_client {
	struct moonlight_source *source;
	
	// Connection info
	char *host;
	int port;
	char *app_name;
	
	// Stream parameters
	int width;
	int height;
	int fps;
	int bitrate;
	
	// State
	bool connected;
	bool streaming;
	
	// Implementation-specific data
	void *priv;
};

// Client lifecycle functions
struct moonlight_client *moonlight_client_create(struct moonlight_source *source);
void moonlight_client_destroy(struct moonlight_client *client);

// Connection management
bool moonlight_client_start(struct moonlight_client *client, const char *host,
			     int port, const char *app_name);
void moonlight_client_stop(struct moonlight_client *client);

// Callbacks (to be called by the protocol implementation)
void moonlight_client_video_frame(struct moonlight_client *client,
				  uint8_t *data, size_t size);
void moonlight_client_audio_frame(struct moonlight_client *client,
				  uint8_t *data, size_t size);
