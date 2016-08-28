#pragma once

#include "obs-studio-frontend-api.h"

#include <vector>
#include <string>

struct obs_studio_callbacks {
	virtual void *obs_studio_get_main_window(void)=0;
	virtual void *obs_studio_get_main_window_handle(void)=0;

	virtual void obs_studio_get_scenes(
			struct obs_studio_source_list *sources)=0;
	virtual obs_source_t *obs_studio_get_current_scene(void)=0;
	virtual void obs_studio_set_current_scene(obs_source_t *scene)=0;

	virtual void obs_studio_get_transitions(
			struct obs_studio_source_list *sources)=0;
	virtual obs_source_t *obs_studio_get_current_transition(void)=0;
	virtual void obs_studio_set_current_transition(
			obs_source_t *transition)=0;

	virtual void obs_studio_get_scene_collections(
			std::vector<std::string> &strings)=0;
	virtual char *obs_studio_get_current_scene_collection(void)=0;
	virtual void obs_studio_set_current_scene_collection(
			const char *collection)=0;

	virtual void obs_studio_get_profiles(
			std::vector<std::string> &strings)=0;
	virtual char *obs_studio_get_current_profile(void)=0;
	virtual void obs_studio_set_current_profile(const char *profile)=0;

	virtual void obs_studio_start_streaming(void)=0;
	virtual void obs_studio_stop_streaming(void)=0;
	virtual bool obs_studio_streaming_active(void)=0;

	virtual void obs_studio_start_recording(void)=0;
	virtual void obs_studio_stop_recording(void)=0;
	virtual bool obs_studio_recording_active(void)=0;

	virtual void *obs_studio_add_tools_menu_qaction(const char *name)=0;
	virtual void obs_studio_add_tools_menu_item(const char *name,
			obs_studio_cb callback, void *private_data)=0;

	virtual void obs_studio_add_event_callback(
			obs_studio_event_cb callback, void *private_data)=0;
	virtual void obs_studio_remove_event_callback(
			obs_studio_event_cb callback, void *private_data)=0;

	virtual obs_output_t *obs_studio_get_streaming_output(void)=0;
	virtual obs_output_t *obs_studio_get_recording_output(void)=0;

	virtual config_t *obs_studio_get_profile_config(void)=0;
	virtual config_t *obs_studio_get_global_config(void)=0;

	virtual void on_event(enum obs_studio_event event)=0;
};

EXPORT void obs_studio_set_callbacks_internal(obs_studio_callbacks *callbacks);
