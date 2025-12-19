#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Forward declarations
struct moonlight_source;

// Audio decoder structure
struct audio_decoder {
	struct moonlight_source *source;
	
	// Decoder context
	void *codec_ctx;
	void *frame;
	
	// Audio format
	int sample_rate;
	int channels;
	
	// Output buffer
	uint8_t *output_data;
	size_t output_size;
};

// Decoder lifecycle
struct audio_decoder *audio_decoder_create(struct moonlight_source *source);
void audio_decoder_destroy(struct audio_decoder *decoder);

// Decode an audio frame
bool audio_decoder_decode(struct audio_decoder *decoder, uint8_t *data,
			  size_t size);
