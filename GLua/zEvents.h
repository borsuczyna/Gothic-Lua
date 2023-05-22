namespace GOTHIC_ENGINE {
	class zEvents {
	public:
		using EventCallback = std::function<void(void*)>;

		static void AddEventHandler(char* name, EventCallback callback, int priority = 0);
		static void RemoveEventHandler(char* name, EventCallback callback);
		static void TriggerEvent(char* name, void* arguments);

	private:
		struct GLuaEvent {
			char* name;
			EventCallback callback;
			int priority = 0;
		};

		static std::vector<GLuaEvent>& Events();
	};
}