#include "moonlight-source.h"
#include "plugin-main.h"
#include "moonlight-client.h"
#include "video-decoder.h"
#include "audio-decoder.h"
#include <obs-module.h>
#include <util/dstr.h>
#include <util/threading.h>

// Default values
#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 47989
#define DEFAULT_APP "Steam"
#define DEFAULT_WIDTH 1920
#define DEFAULT_HEIGHT 1080
#define DEFAULT_FPS 60
#define DEFAULT_BITRATE 20000

// Source callbacks forward declarations
static const char *moonlight_source_get_name(void *unused);
static void *moonlight_source_create(obs_data_t *settings, obs_source_t *source);
static void moonlight_source_destroy(void *data);
static void moonlight_source_update(void *data, obs_data_t *settings);
static void moonlight_source_defaults(obs_data_t *settings);
static obs_properties_t *moonlight_source_properties(void *data);
static void moonlight_source_show(void *data);
static void moonlight_source_hide(void *data);
static void moonlight_source_video_tick(void *data, float seconds);
static void moonlight_source_video_render(void *data, gs_effect_t *effect);
static uint32_t moonlight_source_get_width(void *data);
static uint32_t moonlight_source_get_height(void *data);

// Source info structure
struct obs_source_info moonlight_source_info = {
	.id = "moonlight_source",
	.type = OBS_SOURCE_TYPE_INPUT,
	.output_flags = OBS_SOURCE_ASYNC_VIDEO | OBS_SOURCE_AUDIO |
			OBS_SOURCE_DO_NOT_DUPLICATE,
	.get_name = moonlight_source_get_name,
	.create = moonlight_source_create,
	.destroy = moonlight_source_destroy,
	.update = moonlight_source_update,
	.get_defaults = moonlight_source_defaults,
	.get_properties = moonlight_source_properties,
	.show = moonlight_source_show,
	.hide = moonlight_source_hide,
	.video_tick = moonlight_source_video_tick,
	.video_render = moonlight_source_video_render,
	.get_width = moonlight_source_get_width,
	.get_height = moonlight_source_get_height,
};

static const char *moonlight_source_get_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return "Moonlight GameStream";
}

static void *moonlight_source_create(obs_data_t *settings, obs_source_t *source)
{
	struct moonlight_source *context = bzalloc(sizeof(struct moonlight_source));
	context->source = source;

	pthread_mutex_init(&context->mutex, NULL);

	// Initialize with settings
	moonlight_source_update(context, settings);

	mlog(LOG_INFO, "Moonlight source created");
	return context;
}

static void moonlight_source_destroy(void *data)
{
	struct moonlight_source *context = data;

	if (!context)
		return;

	mlog(LOG_INFO, "Destroying Moonlight source");

	// Stop streaming if active
	if (context->streaming) {
		moonlight_client_stop(context->client);
		context->streaming = false;
	}

	// Cleanup client
	if (context->client) {
		moonlight_client_destroy(context->client);
		context->client = NULL;
	}

	// Cleanup decoders
	if (context->video_dec) {
		video_decoder_destroy(context->video_dec);
		context->video_dec = NULL;
	}

	if (context->audio_dec) {
		audio_decoder_destroy(context->audio_dec);
		context->audio_dec = NULL;
	}

	// Free resources
	if (context->texture) {
		obs_enter_graphics();
		gs_texture_destroy(context->texture);
		obs_leave_graphics();
		context->texture = NULL;
	}

	if (context->video_data) {
		bfree(context->video_data);
		context->video_data = NULL;
	}

	pthread_mutex_destroy(&context->mutex);

	bfree(context->host);
	bfree(context->app_name);
	bfree(context);
}

static void moonlight_source_update(void *data, obs_data_t *settings)
{
	struct moonlight_source *context = data;

	// Get settings
	const char *host = obs_data_get_string(settings, "host");
	int port = (int)obs_data_get_int(settings, "port");
	const char *app_name = obs_data_get_string(settings, "app_name");
	int width = (int)obs_data_get_int(settings, "width");
	int height = (int)obs_data_get_int(settings, "height");
	int fps = (int)obs_data_get_int(settings, "fps");
	int bitrate = (int)obs_data_get_int(settings, "bitrate");

	pthread_mutex_lock(&context->mutex);

	// Update connection settings
	if (context->host)
		bfree(context->host);
	context->host = bstrdup(host);

	context->port = port;

	if (context->app_name)
		bfree(context->app_name);
	context->app_name = bstrdup(app_name);

	context->width = width;
	context->height = height;
	context->fps = fps;
	context->bitrate = bitrate;

	pthread_mutex_unlock(&context->mutex);

	mlog(LOG_INFO, "Moonlight source updated: %s:%d (%dx%d@%dfps)",
	     host, port, width, height, fps);
}

static void moonlight_source_defaults(obs_data_t *settings)
{
	obs_data_set_default_string(settings, "host", DEFAULT_HOST);
	obs_data_set_default_int(settings, "port", DEFAULT_PORT);
	obs_data_set_default_string(settings, "app_name", DEFAULT_APP);
	obs_data_set_default_int(settings, "width", DEFAULT_WIDTH);
	obs_data_set_default_int(settings, "height", DEFAULT_HEIGHT);
	obs_data_set_default_int(settings, "fps", DEFAULT_FPS);
	obs_data_set_default_int(settings, "bitrate", DEFAULT_BITRATE);
}

static obs_properties_t *moonlight_source_properties(void *data)
{
	UNUSED_PARAMETER(data);

	obs_properties_t *props = obs_properties_create();

	obs_properties_add_text(props, "host", "Host", OBS_TEXT_DEFAULT);
	obs_properties_add_int(props, "port", "Port", 1, 65535, 1);
	obs_properties_add_text(props, "app_name", "Application Name",
				OBS_TEXT_DEFAULT);

	obs_property_t *width = obs_properties_add_int(props, "width", "Width",
						       640, 3840, 1);
	obs_property_t *height = obs_properties_add_int(
		props, "height", "Height", 480, 2160, 1);
	obs_property_t *fps = obs_properties_add_int(props, "fps", "FPS", 30,
						     120, 1);
	obs_property_t *bitrate = obs_properties_add_int(
		props, "bitrate", "Bitrate (Kbps)", 1000, 100000, 1000);

	return props;
}

static void moonlight_source_show(void *data)
{
	struct moonlight_source *context = data;

	mlog(LOG_INFO, "Moonlight source shown - starting stream");

	// Initialize client if needed
	if (!context->client) {
		context->client = moonlight_client_create(context);
		if (!context->client) {
			mlog(LOG_ERROR, "Failed to create Moonlight client");
			return;
		}
	}

	// Initialize decoders if needed
	if (!context->video_dec) {
		context->video_dec = video_decoder_create(context);
		if (!context->video_dec) {
			mlog(LOG_ERROR, "Failed to create video decoder");
			return;
		}
	}

	if (!context->audio_dec) {
		context->audio_dec = audio_decoder_create(context);
		if (!context->audio_dec) {
			mlog(LOG_ERROR, "Failed to create audio decoder");
			return;
		}
	}

	// Start streaming
	if (moonlight_client_start(context->client, context->host,
				   context->port, context->app_name)) {
		context->streaming = true;
		context->connected = true;
		mlog(LOG_INFO, "Moonlight streaming started");
	} else {
		mlog(LOG_ERROR, "Failed to start Moonlight streaming");
	}
}

static void moonlight_source_hide(void *data)
{
	struct moonlight_source *context = data;

	mlog(LOG_INFO, "Moonlight source hidden - stopping stream");

	if (context->streaming && context->client) {
		moonlight_client_stop(context->client);
		context->streaming = false;
		context->connected = false;
	}
}

static void moonlight_source_video_tick(void *data, float seconds)
{
	UNUSED_PARAMETER(seconds);
	struct moonlight_source *context = data;

	if (!context->streaming)
		return;

	// Video frames are delivered asynchronously through callbacks
	// This function can be used for periodic updates if needed
}

static void moonlight_source_video_render(void *data, gs_effect_t *effect)
{
	struct moonlight_source *context = data;

	if (!context->texture)
		return;

	effect = obs_get_base_effect(OBS_EFFECT_DEFAULT);

	gs_effect_set_texture(gs_effect_get_param_by_name(effect, "image"),
			      context->texture);

	gs_draw_sprite(context->texture, 0, context->width, context->height);
}

static uint32_t moonlight_source_get_width(void *data)
{
	struct moonlight_source *context = data;
	return context->width;
}

static uint32_t moonlight_source_get_height(void *data)
{
	struct moonlight_source *context = data;
	return context->height;
}
