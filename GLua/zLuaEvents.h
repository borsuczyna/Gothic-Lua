namespace GOTHIC_ENGINE {
	class zLuaEvents {
	public:
		struct GLuaEvent {
			lua_State* L;
			const char* name;
			int callback;
			double priority = 0.0;
		};

		static int AddEventHandler(lua_State* L);
		static void Init();

	private:
		static bool CompareByPriority(const zLuaEvents::GLuaEvent& event1, const zLuaEvents::GLuaEvent& event2);
		static std::vector<GLuaEvent>& Events();
		static void EventTriggered(void* arguments);
	};
}