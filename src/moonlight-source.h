#pragma once

#include <obs-module.h>
#include <stdbool.h>
#include <pthread.h>

// Forward declaration of moonlight client
struct moonlight_client;
struct video_decoder;
struct audio_decoder;

// Moonlight source context
struct moonlight_source {
	obs_source_t *source;

	// Connection settings
	char *host;
	int port;
	char *app_name;
	int width;
	int height;
	int fps;
	int bitrate;

	// Connection state
	bool connected;
	bool streaming;
	pthread_t stream_thread;
	pthread_mutex_t mutex;

	// Client and decoders
	struct moonlight_client *client;
	struct video_decoder *video_dec;
	struct audio_decoder *audio_dec;

	// Video rendering
	gs_texture_t *texture;
	uint8_t *video_data;
	uint32_t video_linesize;
};

// External source info structure
extern struct obs_source_info moonlight_source_info;
