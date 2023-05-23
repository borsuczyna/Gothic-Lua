namespace GOTHIC_ENGINE {
	struct EventTriggeredArguments {
		char* eventName;
		void* eventArguments;
	};

	class zEvents {
	public:
		using EventCallback = std::function<void(void*)>;

		struct GLuaEvent {
			char* name;
			EventCallback callback;
			int priority = 0;
		};

		static void AddEventHandler(char* name, EventCallback callback, int priority = 0);
		static void RemoveEventHandler(char* name, EventCallback callback);
		static void TriggerEvent(char* name, void* arguments);

	private:
		static bool zEvents::CompareByPriority(const zEvents::GLuaEvent& event1, const zEvents::GLuaEvent& event2);
		static std::vector<GLuaEvent>& Events();
	};
}