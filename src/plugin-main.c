#include "plugin-main.h"
#include "moonlight-source.h"
#include <obs-module.h>

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE(PLUGIN_NAME, "en-US")

bool obs_module_load(void)
{
	blog(LOG_INFO, "Moonlight OBS Plugin loaded successfully (version %s)",
	     PLUGIN_VERSION);

	// Register the Moonlight source
	obs_register_source(&moonlight_source_info);

	return true;
}

void obs_module_unload(void)
{
	blog(LOG_INFO, "Moonlight OBS Plugin unloaded");
}

const char *obs_module_name(void)
{
	return "Moonlight GameStream Source";
}

const char *obs_module_description(void)
{
	return "Receive NVIDIA GameStream and Sunshine video/audio streams in OBS Studio";
}
