#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Forward declarations
struct moonlight_source;

// Video decoder structure
struct video_decoder {
	struct moonlight_source *source;
	
	// Decoder context
	void *codec_ctx;
	void *frame;
	void *sws_ctx;
	
	// Stream info
	int width;
	int height;
	
	// Output buffer
	uint8_t *output_data;
	int output_linesize;
};

// Decoder lifecycle
struct video_decoder *video_decoder_create(struct moonlight_source *source);
void video_decoder_destroy(struct video_decoder *decoder);

// Decode a video frame
bool video_decoder_decode(struct video_decoder *decoder, uint8_t *data,
			  size_t size);
