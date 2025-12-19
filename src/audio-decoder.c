#include "audio-decoder.h"
#include "moonlight-source.h"
#include "plugin-main.h"
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <obs-module.h>

// Default audio settings
#define DEFAULT_SAMPLE_RATE 48000
#define DEFAULT_CHANNELS 2

struct audio_decoder *audio_decoder_create(struct moonlight_source *source)
{
	struct audio_decoder *decoder =
		bzalloc(sizeof(struct audio_decoder));
	if (!decoder)
		return NULL;

	decoder->source = source;
	decoder->sample_rate = DEFAULT_SAMPLE_RATE;
	decoder->channels = DEFAULT_CHANNELS;

	// Find Opus decoder (commonly used in game streaming)
	const AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_OPUS);
	if (!codec) {
		blog(LOG_ERROR, "Opus decoder not found");
		bfree(decoder);
		return NULL;
	}

	// Allocate codec context
	AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
	if (!codec_ctx) {
		blog(LOG_ERROR, "Failed to allocate audio codec context");
		bfree(decoder);
		return NULL;
	}

	codec_ctx->sample_rate = DEFAULT_SAMPLE_RATE;
	codec_ctx->channels = DEFAULT_CHANNELS;
	codec_ctx->channel_layout = AV_CH_LAYOUT_STEREO;

	// Open codec
	if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
		blog(LOG_ERROR, "Failed to open audio codec");
		avcodec_free_context(&codec_ctx);
		bfree(decoder);
		return NULL;
	}

	decoder->codec_ctx = codec_ctx;

	// Allocate frame
	AVFrame *frame = av_frame_alloc();
	if (!frame) {
		blog(LOG_ERROR, "Failed to allocate audio frame");
		avcodec_free_context(&codec_ctx);
		bfree(decoder);
		return NULL;
	}

	decoder->frame = frame;

	// Allocate output buffer
	decoder->output_size = DEFAULT_SAMPLE_RATE * DEFAULT_CHANNELS *
			       sizeof(float) * 2; // 2 seconds buffer
	decoder->output_data = bmalloc(decoder->output_size);

	blog(LOG_INFO, "Audio decoder created (%d Hz, %d channels)",
	     DEFAULT_SAMPLE_RATE, DEFAULT_CHANNELS);
	return decoder;
}

void audio_decoder_destroy(struct audio_decoder *decoder)
{
	if (!decoder)
		return;

	blog(LOG_INFO, "Destroying audio decoder");

	if (decoder->frame) {
		av_frame_free((AVFrame **)&decoder->frame);
		decoder->frame = NULL;
	}

	if (decoder->codec_ctx) {
		avcodec_free_context((AVCodecContext **)&decoder->codec_ctx);
		decoder->codec_ctx = NULL;
	}

	if (decoder->output_data) {
		bfree(decoder->output_data);
		decoder->output_data = NULL;
	}

	bfree(decoder);
}

bool audio_decoder_decode(struct audio_decoder *decoder, uint8_t *data,
			  size_t size)
{
	if (!decoder || !data || size == 0)
		return false;

	AVCodecContext *codec_ctx = decoder->codec_ctx;
	AVFrame *frame = decoder->frame;

	// Create packet
	AVPacket *packet = av_packet_alloc();
	if (!packet)
		return false;

	packet->data = data;
	packet->size = size;

	// Send packet to decoder
	int ret = avcodec_send_packet(codec_ctx, packet);
	av_packet_free(&packet);

	if (ret < 0) {
		blog(LOG_ERROR, "Error sending audio packet to decoder: %d",
		     ret);
		return false;
	}

	// Receive decoded frame
	ret = avcodec_receive_frame(codec_ctx, frame);
	if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
		return true; // Need more data
	} else if (ret < 0) {
		blog(LOG_ERROR, "Error receiving audio frame from decoder: %d",
		     ret);
		return false;
	}

	// Prepare audio data for OBS
	struct obs_source_audio audio_data = {0};
	audio_data.data[0] = frame->data[0];
	audio_data.frames = frame->nb_samples;
	audio_data.speakers = SPEAKERS_STEREO;
	audio_data.samples_per_sec = decoder->sample_rate;
	audio_data.format = AUDIO_FORMAT_FLOAT_PLANAR;
	audio_data.timestamp = os_gettime_ns();

	// If stereo, set second channel
	if (decoder->channels == 2 && frame->data[1]) {
		audio_data.data[1] = frame->data[1];
	}

	// Send audio to OBS
	struct moonlight_source *source = decoder->source;
	obs_source_output_audio(source->source, &audio_data);

	return true;
}
