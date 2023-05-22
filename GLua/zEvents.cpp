namespace GOTHIC_ENGINE {
	std::vector<zEvents::GLuaEvent>& zEvents::Events() {
		static std::vector<GLuaEvent> events;
		return events;
	}

	void zEvents::AddEventHandler(char* name, EventCallback callback, int priority) {
		GLuaEvent newEvent;
		newEvent.name = name;
		newEvent.callback = callback;
		newEvent.priority = priority;
		Events().push_back(newEvent);
	}

	void zEvents::RemoveEventHandler(char* name, EventCallback callback) {
		auto& events = Events();
		for (auto it = events.begin(); it != events.end(); ++it) {
			if (strcmp(it->name, name) == 0 && it->callback.target_type() == callback.target_type()) {
				events.erase(it);
				break;
			}
		}
	}

	void zEvents::TriggerEvent(char* name, void* arguments) {
		auto& events = Events();
		for (auto& event : events) {
			if (strcmp(event.name, name) == 0) {
				event.callback(arguments);
			}
		}
	}
}