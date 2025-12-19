#include "video-decoder.h"
#include "moonlight-source.h"
#include "plugin-main.h"
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <obs-module.h>
#include <graphics/graphics.h>

// RGBA format has 4 bytes per pixel
#define BYTES_PER_PIXEL_RGBA 4

struct video_decoder *video_decoder_create(struct moonlight_source *source)
{
	struct video_decoder *decoder =
		bzalloc(sizeof(struct video_decoder));
	if (!decoder)
		return NULL;

	decoder->source = source;
	decoder->width = source->width;
	decoder->height = source->height;

	// Find H.264 decoder
	const AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_H264);
	if (!codec) {
		mlog(LOG_ERROR, "H.264 decoder not found");
		bfree(decoder);
		return NULL;
	}

	// Allocate codec context
	AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
	if (!codec_ctx) {
		mlog(LOG_ERROR, "Failed to allocate codec context");
		bfree(decoder);
		return NULL;
	}

	codec_ctx->width = source->width;
	codec_ctx->height = source->height;
	// Using YUV420P as it's the standard output format for H.264 in GameStream
	// In a production implementation, this could be detected from the stream
	codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;

	// Open codec
	if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
		mlog(LOG_ERROR, "Failed to open codec");
		avcodec_free_context(&codec_ctx);
		bfree(decoder);
		return NULL;
	}

	decoder->codec_ctx = codec_ctx;

	// Allocate frame
	AVFrame *frame = av_frame_alloc();
	if (!frame) {
		mlog(LOG_ERROR, "Failed to allocate frame");
		avcodec_free_context(&codec_ctx);
		bfree(decoder);
		return NULL;
	}

	decoder->frame = frame;

	// Allocate output buffer for RGBA data
	int output_size = source->width * source->height * BYTES_PER_PIXEL_RGBA;
	decoder->output_data = bmalloc(output_size);
	decoder->output_linesize = source->width * BYTES_PER_PIXEL_RGBA;

	mlog(LOG_INFO, "Video decoder created (%dx%d)", source->width,
	     source->height);
	return decoder;
}

void video_decoder_destroy(struct video_decoder *decoder)
{
	if (!decoder)
		return;

	mlog(LOG_INFO, "Destroying video decoder");

	if (decoder->sws_ctx) {
		sws_freeContext(decoder->sws_ctx);
		decoder->sws_ctx = NULL;
	}

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

bool video_decoder_decode(struct video_decoder *decoder, uint8_t *data,
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
		mlog(LOG_ERROR, "Error sending packet to decoder: %d", ret);
		return false;
	}

	// Receive decoded frame
	ret = avcodec_receive_frame(codec_ctx, frame);
	if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
		return true; // Need more data
	} else if (ret < 0) {
		mlog(LOG_ERROR, "Error receiving frame from decoder: %d", ret);
		return false;
	}

	// Convert frame to RGBA for OBS
	struct SwsContext *sws_ctx = decoder->sws_ctx;
	if (!sws_ctx) {
		sws_ctx = sws_getContext(
			frame->width, frame->height, codec_ctx->pix_fmt,
			decoder->width, decoder->height, AV_PIX_FMT_RGBA,
			SWS_BILINEAR, NULL, NULL, NULL);
		if (!sws_ctx) {
			mlog(LOG_ERROR, "Failed to create swscale context");
			return false;
		}
		decoder->sws_ctx = sws_ctx;
	}

	// Scale and convert to RGBA
	uint8_t *dst_data[4] = {decoder->output_data, NULL, NULL, NULL};
	int dst_linesize[4] = {decoder->output_linesize, 0, 0, 0};

	sws_scale(sws_ctx, (const uint8_t *const *)frame->data,
		  frame->linesize, 0, frame->height, dst_data, dst_linesize);

	// Update OBS texture
	struct moonlight_source *source = decoder->source;
	pthread_mutex_lock(&source->mutex);

	obs_enter_graphics();

	if (!source->texture) {
		source->texture = gs_texture_create(
			decoder->width, decoder->height, GS_RGBA, 1, NULL,
			GS_DYNAMIC);
	}

	if (source->texture) {
		gs_texture_set_image(source->texture, decoder->output_data,
				     decoder->output_linesize, false);
	}

	obs_leave_graphics();

	pthread_mutex_unlock(&source->mutex);

	return true;
}
