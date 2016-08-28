#include "obs-app.hpp"
#include "qt-wrappers.hpp"
#include "window-basic-main.hpp"
#include "window-basic-main-outputs.hpp"
#include "obs-studio-frontend-api/obs-studio-frontend-internal.hpp"

#include <functional>

using namespace std;

Q_DECLARE_METATYPE(OBSScene);
Q_DECLARE_METATYPE(OBSSource);

template <typename T>
static T GetOBSRef(QListWidgetItem *item)
{
	return item->data(static_cast<int>(QtDataRole::OBSRef)).value<T>();
}

void EnumProfiles(function<bool (const char *, const char *)> &&cb);
void EnumSceneCollections(function<bool (const char *, const char *)> &&cb);

/* ------------------------------------------------------------------------- */

struct OBSStudioEventCallback {
	obs_studio_event_cb callback;
	void *private_data;

	inline OBSStudioEventCallback(obs_studio_event_cb cb, void *p) :
		callback(cb), private_data(p)
	{}
};

struct OBSStudioAPI : obs_studio_callbacks {
	OBSBasic *main;
	vector<OBSStudioEventCallback> callbacks;

	inline size_t GetCallbackIdx(obs_studio_event_cb callback,
			void *private_data)
	{
		for (size_t i = 0; i < callbacks.size(); i++) {
			OBSStudioEventCallback curCB = callbacks[i];
			if (curCB.callback     == callback &&
			    curCB.private_data == private_data)
				return i;
		}

		return (size_t)-1;
	}

	inline OBSStudioAPI(OBSBasic *main_) : main(main_) {}

	void *obs_studio_get_main_window(void) override
	{
		return (void*)main;
	}

	void *obs_studio_get_main_window_handle(void) override
	{
		return (void*)main->winId();
	}

	void obs_studio_get_scenes(
			struct obs_studio_source_list *sources) override
	{
		for (int i = 0; i < main->ui->scenes->count(); i++) {
			QListWidgetItem *item = main->ui->scenes->item(i);
			OBSScene scene = GetOBSRef<OBSScene>(item);
			obs_source_t *source = obs_scene_get_source(scene);

			obs_source_addref(source);
			da_push_back(sources->sources, &source);
		}
	}

	obs_source_t *obs_studio_get_current_scene(void) override
	{
		OBSSource source = main->GetCurrentSceneSource();
		obs_source_addref(source);
		return source;
	}

	void obs_studio_set_current_scene(obs_source_t *scene) override
	{
		QMetaObject::invokeMethod(main, "TransitionToScene",
				Q_ARG(OBSSource, OBSSource(scene)));
	}

	void obs_studio_get_transitions(
			struct obs_studio_source_list *sources) override
	{
		for (int i = 0; i < main->ui->transitions->count(); i++) {
			OBSSource tr = main->ui->transitions->itemData(i)
				.value<OBSSource>();

			obs_source_addref(tr);
			da_push_back(sources->sources, &tr);
		}
	}

	obs_source_t *obs_studio_get_current_transition(void) override
	{
		OBSSource tr = main->GetCurrentTransition();

		obs_source_addref(tr);
		return tr;
	}

	void obs_studio_set_current_transition(
			obs_source_t *transition) override
	{
		QMetaObject::invokeMethod(main, "SetTransition",
				Q_ARG(OBSSource, OBSSource(transition)));
	}

	void obs_studio_get_scene_collections(
			std::vector<std::string> &strings) override
	{
		auto addCollection = [&](const char *name, const char *)
		{
			strings.emplace_back(name);
			return true;
		};

		EnumSceneCollections(addCollection);
	}

	char *obs_studio_get_current_scene_collection(void) override
	{
		const char *cur_name = config_get_string(App()->GlobalConfig(),
				"Basic", "SceneCollection");
		return bstrdup(cur_name);
	}

	void obs_studio_set_current_scene_collection(
			const char *collection) override
	{
		QList<QAction*> menuActions =
			main->ui->sceneCollectionMenu->actions();
		QString qstrCollection = QT_UTF8(collection);

		for (int i = 0; i < menuActions.count(); i++) {
			QAction *action = menuActions[i];
			QVariant v = action->property("file_name");

			if (v.typeName() != nullptr) {
				if (action->text() == qstrCollection) {
					action->trigger();
					break;
				}
			}
		}
	}

	void obs_studio_get_profiles(
			std::vector<std::string> &strings) override
	{
		auto addProfile = [&](const char *name, const char *)
		{
			strings.emplace_back(name);
			return true;
		};

		EnumProfiles(addProfile);
	}

	char *obs_studio_get_current_profile(void) override
	{
		const char *name = config_get_string(App()->GlobalConfig(),
				"Basic", "Profile");
		return bstrdup(name);
	}

	void obs_studio_set_current_profile(const char *profile) override
	{
		QList<QAction*> menuActions =
			main->ui->profileMenu->actions();
		QString qstrProfile = QT_UTF8(profile);

		for (int i = 0; i < menuActions.count(); i++) {
			QAction *action = menuActions[i];
			QVariant v = action->property("file_name");

			if (v.typeName() != nullptr) {
				if (action->text() == qstrProfile) {
					action->trigger();
					break;
				}
			}
		}
	}

	void obs_studio_start_streaming(void) override
	{
		QMetaObject::invokeMethod(main, "StartStreaming");
	}

	void obs_studio_stop_streaming(void) override
	{
		QMetaObject::invokeMethod(main, "StopStreaming");
	}

	bool obs_studio_streaming_active(void) override
	{
		return main->outputHandler->StreamingActive();
	}

	void obs_studio_start_recording(void) override
	{
		QMetaObject::invokeMethod(main, "StartRecording");
	}

	void obs_studio_stop_recording(void) override
	{
		QMetaObject::invokeMethod(main, "StopRecording");
	}

	bool obs_studio_recording_active(void) override
	{
		return main->outputHandler->StreamingActive();
	}

	void *obs_studio_add_tools_menu_qaction(const char *name) override
	{
		main->ui->menuTools->setEnabled(true);
		return (void*)main->ui->menuTools->addAction(QT_UTF8(name));
	}

	void obs_studio_add_tools_menu_item(const char *name,
			obs_studio_cb callback, void *private_data) override
	{
		main->ui->menuTools->setEnabled(true);

		auto func = [private_data, callback] ()
		{
			callback(private_data);
		};

		main->ui->menuTools->addAction(QT_UTF8(name), func);
	}

	void obs_studio_add_event_callback(obs_studio_event_cb callback,
			void *private_data) override
	{
		size_t idx = GetCallbackIdx(callback, private_data);
		if (idx == (size_t)-1)
			callbacks.emplace_back(callback, private_data);
	}

	void obs_studio_remove_event_callback(obs_studio_event_cb callback,
			void *private_data) override
	{
		size_t idx = GetCallbackIdx(callback, private_data);
		if (idx == (size_t)-1)
			return;

		callbacks.erase(callbacks.begin() + idx);
	}

	obs_output_t *obs_studio_get_streaming_output(void) override
	{
		OBSOutput output = main->outputHandler->streamOutput;
		obs_output_addref(output);
		return output;
	}

	obs_output_t *obs_studio_get_recording_output(void) override
	{
		OBSOutput out = main->outputHandler->fileOutput;
		obs_output_addref(out);
		return out;
	}

	config_t *obs_studio_get_profile_config(void) override
	{
		return main->basicConfig;
	}

	config_t *obs_studio_get_global_config(void) override
	{
		return App()->GlobalConfig();
	}

	void on_event(enum obs_studio_event event) override
	{
		if (main->disableSaving)
			return;

		for (OBSStudioEventCallback cb : callbacks)
			cb.callback(event, cb.private_data);
	}
};

obs_studio_callbacks *InitializeAPIInterface(OBSBasic *main)
{
	obs_studio_callbacks *api = new OBSStudioAPI(main);
	obs_studio_set_callbacks_internal(api);
	return api;
}
