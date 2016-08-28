#pragma once

#include <obs.h>
#include <util/darray.h>

#ifdef __cplusplus
extern "C" {
#endif

struct config_data;
typedef struct config_data config_t;

/* ------------------------------------------------------------------------- */

struct obs_studio_source_list {
	DARRAY(obs_source_t*) sources;
};

static inline void obs_studio_source_list_free(
		struct obs_studio_source_list *source_list)
{
	size_t num = source_list->sources.num;
	for (size_t i = 0; i < num; i++)
		obs_source_release(source_list->sources.array[i]);
	da_free(source_list->sources);
}

/* ------------------------------------------------------------------------- */

EXPORT void *obs_studio_get_main_window(void);
EXPORT void *obs_studio_get_main_window_handle(void);

EXPORT void obs_studio_get_scenes(struct obs_studio_source_list *sources);
EXPORT obs_source_t *obs_studio_get_current_scene(void);
EXPORT void obs_studio_set_current_scene(obs_source_t *scene);

EXPORT void obs_studio_get_transitions(struct obs_studio_source_list *sources);
EXPORT obs_source_t *obs_studio_get_current_transition(void);
EXPORT void obs_studio_set_current_transition(obs_source_t *transition);

EXPORT char **obs_studio_get_scene_collections(void);
EXPORT char *obs_studio_get_current_scene_collection(void);
EXPORT void obs_studio_set_current_scene_collection(const char *collection);

EXPORT char **obs_studio_get_profiles(void);
EXPORT char *obs_studio_get_current_profile(void);
EXPORT void obs_studio_set_current_profile(const char *profile);

EXPORT void obs_studio_start_streaming(void);
EXPORT void obs_studio_stop_streaming(void);
EXPORT bool obs_studio_streaming_active(void);

EXPORT void obs_studio_start_recording(void);
EXPORT void obs_studio_stop_recording(void);
EXPORT bool obs_studio_recording_active(void);

typedef void (*obs_studio_cb)(void *private_data);

EXPORT void *obs_studio_add_tools_menu_qaction(const char *name);
EXPORT void obs_studio_add_tools_menu_item(const char *name,
		obs_studio_cb callback, void *private_data);

enum obs_studio_event {
	OBS_STUDIO_EVENT_STREAM_STARTED,
	OBS_STUDIO_EVENT_STREAM_STOPPING,
	OBS_STUDIO_EVENT_STREAM_STOPPED,
	OBS_STUDIO_EVENT_RECORDING_STARTED,
	OBS_STUDIO_EVENT_RECORDING_STOPPING,
	OBS_STUDIO_EVENT_RECORDING_STOPPED,
	OBS_STUDIO_EVENT_SCENE_CHANGED,
	OBS_STUDIO_EVENT_SCENE_LIST_CHANGED,
	OBS_STUDIO_EVENT_TRANSITION_CHANGED,
	OBS_STUDIO_EVENT_TRANSITION_STOPPED,
	OBS_STUDIO_EVENT_TRANSITION_LIST_CHANGED,
	OBS_STUDIO_EVENT_SCENE_COLLECTION_CHANGED,
	OBS_STUDIO_EVENT_SCENE_COLLECTION_LIST_CHANGED,
	OBS_STUDIO_EVENT_PROFILE_CHANGED,
	OBS_STUDIO_EVENT_PROFILE_LIST_CHANGED,
	OBS_STUDIO_EVENT_EXIT
};

typedef void (*obs_studio_event_cb)(enum obs_studio_event event,
		void *private_data);

EXPORT void obs_studio_add_event_callback(obs_studio_event_cb callback,
		void *private_data);
EXPORT void obs_studio_remove_event_callback(obs_studio_event_cb callback,
		void *private_data);

EXPORT obs_output_t *obs_studio_get_streaming_output(void);
EXPORT obs_output_t *obs_studio_get_recording_output(void);

EXPORT config_t *obs_studio_get_profile_config(void);
EXPORT config_t *obs_studio_get_global_config(void);

/* ------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif
