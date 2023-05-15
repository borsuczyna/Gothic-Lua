namespace GOTHIC_ENGINE {
	class zEvents {
		using EventCallback = std::function<void(void*)>;

		struct GLuaEvent {
			char* name;
			EventCallback callback;
			int priority = 0;
		};

		static std::vector<GLuaEvent>& Events() {
			static std::vector<GLuaEvent> events;
			return events;
		}
	public:
		static void AddEventHandler(char* name, EventCallback callback, int priority = 0) {
			GLuaEvent newEvent;
			newEvent.name = name;
			newEvent.callback = callback;
			newEvent.priority = priority;
			Events().push_back(newEvent);
		}

		static void RemoveEventHandler(char* name, EventCallback callback) {
			auto& events = Events();
			for (auto it = events.begin(); it != events.end(); ++it) {
				if (strcmp(it->name, name) == 0 && it->callback.target_type() == callback.target_type()) {
					events.erase(it);
					break;
				}
			}
		}

		static void TriggerEvent(char* name, void* arguments) {
			auto& events = Events();
			for (auto& event : events) {
				if (strcmp(event.name, name) == 0) {
					event.callback(arguments);
				}
			}
		}
	};
}