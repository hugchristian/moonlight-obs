#pragma once

#include <obs-module.h>
#include <util/platform.h>

#define PLUGIN_NAME "moonlight-obs"
#define PLUGIN_VERSION "1.0.0"

#define mlog(log_level, format, ...) \
	blog(log_level, "[" PLUGIN_NAME "] " format, ##__VA_ARGS__)

// Forward declarations
extern struct obs_source_info moonlight_source_info;
