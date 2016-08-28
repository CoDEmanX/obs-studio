#include "obs-studio-frontend-internal.hpp"
#include <memory>

using namespace std;

static unique_ptr<obs_studio_callbacks> c;

void obs_studio_set_callbacks_internal(obs_studio_callbacks *callbacks)
{
	c.reset(callbacks);
}

static inline bool callbacks_valid_(const char *func_name)
{
	if (!c) {
		blog(LOG_WARNING, "Tried to call %s with no callbacks!",
				func_name);
		return false;
	}

	return true;
}

#define callbacks_valid() callbacks_valid_(__FUNCTION__)

/* ------------------------------------------------------------------------- */

void *obs_studio_get_main_window(void)
{
	return !!callbacks_valid()
		? c->obs_studio_get_main_window()
		: nullptr;
}

void *obs_studio_get_main_window_handle(void)
{
	return !!callbacks_valid()
		? c->obs_studio_get_main_window_handle()
		: nullptr;
}

void obs_studio_get_scenes(struct obs_studio_source_list *sources)
{
	if (callbacks_valid()) c->obs_studio_get_scenes(sources);
}

obs_source_t *obs_studio_get_current_scene(void)
{
	return !!callbacks_valid()
		? c->obs_studio_get_current_scene()
		: nullptr;
}

void obs_studio_set_current_scene(obs_source_t *scene)
{
	if (callbacks_valid()) c->obs_studio_set_current_scene(scene);
}

void obs_studio_get_transitions(struct obs_studio_source_list *sources)
{
	if (callbacks_valid()) c->obs_studio_get_transitions(sources);
}

obs_source_t *obs_studio_get_current_transition(void)
{
	return !!callbacks_valid()
		? c->obs_studio_get_current_transition()
		: nullptr;
}

void obs_studio_set_current_transition(obs_source_t *transition)
{
	if (callbacks_valid()) c->obs_studio_set_current_transition(transition);
}

static char **convert_string_list(vector<string> &strings)
{
	size_t size = 0;
	size_t string_data_offset = (strings.size() + 1) * sizeof(char*);
	uint8_t *out;
	char **ptr_list;
	char *string_data;

	size += string_data_offset;

	for (auto &str : strings)
		size += str.size() + 1;

	if (!size)
		return 0;

	out = (uint8_t*)bmalloc(size);
	ptr_list = (char**)out;
	string_data = (char*)(out + string_data_offset);

	for (auto &str : strings) {
		*ptr_list = string_data;
		memcpy(string_data, str.c_str(), str.size() + 1);

		ptr_list++;
		string_data += str.size() + 1;
	}

	*ptr_list = nullptr;
	return (char**)out;
}

char **obs_studio_get_scene_collections(void)
{
	if (!callbacks_valid())
		return nullptr;

	vector<string> strings;
	c->obs_studio_get_scene_collections(strings);
	return convert_string_list(strings);
}

char *obs_studio_get_current_scene_collection(void)
{
	return !!callbacks_valid()
		? c->obs_studio_get_current_scene_collection()
		: nullptr;
}

void obs_studio_set_current_scene_collection(const char *collection)
{
	if (callbacks_valid())
		c->obs_studio_set_current_scene_collection(collection);
}

char **obs_studio_get_profiles(void)
{
	if (!callbacks_valid())
		return nullptr;

	vector<string> strings;
	c->obs_studio_get_profiles(strings);
	return convert_string_list(strings);
}

char *obs_studio_get_current_profile(void)
{
	return !!callbacks_valid()
		? c->obs_studio_get_current_profile()
		: nullptr;
}

void obs_studio_set_current_profile(const char *profile)
{
	if (callbacks_valid())
		c->obs_studio_set_current_profile(profile);
}

void obs_studio_start_streaming(void)
{
	if (callbacks_valid()) c->obs_studio_start_streaming();
}

void obs_studio_stop_streaming(void)
{
	if (callbacks_valid()) c->obs_studio_stop_streaming();
}

bool obs_studio_streaming_active(void)
{
	return !!callbacks_valid()
		? c->obs_studio_streaming_active()
		: false;
}

void obs_studio_start_recording(void)
{
	if (callbacks_valid()) c->obs_studio_start_recording();
}

void obs_studio_stop_recording(void)
{
	if (callbacks_valid()) c->obs_studio_stop_recording();
}

bool obs_studio_recording_active(void)
{
	return !!callbacks_valid()
		? c->obs_studio_recording_active()
		: false;
}

void *obs_studio_add_tools_menu_qaction(const char *name)
{
	return !!callbacks_valid()
		? c->obs_studio_add_tools_menu_qaction(name)
		: nullptr;
}

void obs_studio_add_tools_menu_item(const char *name,
		obs_studio_cb callback, void *private_data)
{
	if (callbacks_valid())
		c->obs_studio_add_tools_menu_item(name, callback, private_data);
}

void obs_studio_add_event_callback(obs_studio_event_cb callback,
		void *private_data)
{
	if (callbacks_valid())
		c->obs_studio_add_event_callback(callback, private_data);
}

void obs_studio_remove_event_callback(obs_studio_event_cb callback,
		void *private_data)
{
	if (callbacks_valid())
		c->obs_studio_remove_event_callback(callback, private_data);
}

obs_output_t *obs_studio_get_streaming_output(void)
{
	return !!callbacks_valid()
		? c->obs_studio_get_streaming_output()
		: nullptr;
}

obs_output_t *obs_studio_get_recording_output(void)
{
	return !!callbacks_valid()
		? c->obs_studio_get_recording_output()
		: nullptr;
}

config_t *obs_studio_get_profile_config(void)
{
	return !!callbacks_valid()
		? c->obs_studio_get_profile_config()
		: nullptr;
}

config_t *obs_studio_get_global_config(void)
{
	return !!callbacks_valid()
		? c->obs_studio_get_global_config()
		: nullptr;
}
