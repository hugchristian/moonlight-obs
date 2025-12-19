#include "moonlight-client.h"
#include "moonlight-source.h"
#include "video-decoder.h"
#include "audio-decoder.h"
#include "plugin-main.h"
#include <obs-module.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Placeholder sleep duration in nanoseconds (approximately 60 FPS)
#define PLACEHOLDER_FRAME_INTERVAL_NS 16666666

// Private data structure for client implementation
struct client_priv {
	pthread_t thread;
	bool should_stop;
	pthread_mutex_t mutex;
};

// Thread function for streaming
static void *streaming_thread(void *arg)
{
	struct moonlight_client *client = arg;
	struct client_priv *priv = client->priv;

	mlog(LOG_INFO, "Streaming thread started for %s:%d", client->host,
	     client->port);

	// In a real implementation, this would:
	// 1. Connect to the GameStream/Sunshine host
	// 2. Negotiate stream parameters
	// 3. Start receiving video/audio packets
	// 4. Decode and pass frames to OBS
	//
	// For now, this is a placeholder that demonstrates the structure

	while (!priv->should_stop) {
		pthread_mutex_lock(&priv->mutex);
		bool should_stop = priv->should_stop;
		pthread_mutex_unlock(&priv->mutex);

		if (should_stop)
			break;

		// TODO: Receive and process stream packets
		// This would involve:
		// - Receiving H.264/H.265 video packets
		// - Receiving Opus audio packets
		// - Calling moonlight_client_video_frame() with video data
		// - Calling moonlight_client_audio_frame() with audio data

		// Placeholder sleep for ~60 FPS frame interval
		struct timespec ts = {
			.tv_sec = 0,
			.tv_nsec = PLACEHOLDER_FRAME_INTERVAL_NS
		};
		nanosleep(&ts, NULL);
	}

	mlog(LOG_INFO, "Streaming thread stopped");
	return NULL;
}

struct moonlight_client *moonlight_client_create(struct moonlight_source *source)
{
	struct moonlight_client *client =
		bzalloc(sizeof(struct moonlight_client));
	if (!client)
		return NULL;

	client->source = source;
	client->connected = false;
	client->streaming = false;

	// Allocate private data
	struct client_priv *priv = bzalloc(sizeof(struct client_priv));
	if (!priv) {
		bfree(client);
		return NULL;
	}

	pthread_mutex_init(&priv->mutex, NULL);
	priv->should_stop = false;
	client->priv = priv;

	mlog(LOG_INFO, "Moonlight client created");
	return client;
}

void moonlight_client_destroy(struct moonlight_client *client)
{
	if (!client)
		return;

	mlog(LOG_INFO, "Destroying Moonlight client");

	// Stop streaming if active
	if (client->streaming) {
		moonlight_client_stop(client);
	}

	// Free private data
	if (client->priv) {
		struct client_priv *priv = client->priv;
		pthread_mutex_destroy(&priv->mutex);
		bfree(priv);
	}

	// Free allocated strings
	bfree(client->host);
	bfree(client->app_name);
	bfree(client);
}

bool moonlight_client_start(struct moonlight_client *client, const char *host,
			     int port, const char *app_name)
{
	if (!client || client->streaming)
		return false;

	struct client_priv *priv = client->priv;

	mlog(LOG_INFO, "Starting Moonlight client: %s:%d (app: %s)", host, port,
	     app_name);

	// Store connection parameters
	client->host = bstrdup(host);
	client->port = port;
	client->app_name = bstrdup(app_name);

	// Get stream parameters from source
	struct moonlight_source *source = client->source;
	client->width = source->width;
	client->height = source->height;
	client->fps = source->fps;
	client->bitrate = source->bitrate;

	// Start streaming thread
	priv->should_stop = false;
	if (pthread_create(&priv->thread, NULL, streaming_thread, client) != 0) {
		mlog(LOG_ERROR, "Failed to create streaming thread");
		bfree(client->host);
		bfree(client->app_name);
		client->host = NULL;
		client->app_name = NULL;
		return false;
	}

	client->streaming = true;
	client->connected = true;

	mlog(LOG_INFO, "Moonlight client started successfully");
	return true;
}

void moonlight_client_stop(struct moonlight_client *client)
{
	if (!client || !client->streaming)
		return;

	mlog(LOG_INFO, "Stopping Moonlight client");

	struct client_priv *priv = client->priv;

	// Signal thread to stop
	pthread_mutex_lock(&priv->mutex);
	priv->should_stop = true;
	pthread_mutex_unlock(&priv->mutex);

	// Wait for thread to finish
	pthread_join(priv->thread, NULL);

	client->streaming = false;
	client->connected = false;

	mlog(LOG_INFO, "Moonlight client stopped");
}

void moonlight_client_video_frame(struct moonlight_client *client,
				  uint8_t *data, size_t size)
{
	if (!client || !client->source)
		return;

	struct moonlight_source *source = client->source;

	// Pass the video frame to the decoder
	if (source->video_dec) {
		video_decoder_decode(source->video_dec, data, size);
	}
}

void moonlight_client_audio_frame(struct moonlight_client *client,
				  uint8_t *data, size_t size)
{
	if (!client || !client->source)
		return;

	struct moonlight_source *source = client->source;

	// Pass the audio frame to the decoder
	if (source->audio_dec) {
		audio_decoder_decode(source->audio_dec, data, size);
	}
}
