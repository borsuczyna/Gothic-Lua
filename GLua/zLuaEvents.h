#define LUA_EVENT_MAX_ARGUMENTS 64

namespace GOTHIC_ENGINE {
	class zLuaEvents {
	public:
		enum LuaEventArgumentType {
			Number,
			String,
			Boolean,
			Userdata
		};

		struct LuaEventArguments {
			int argCount = 0;
			int argumentType[LUA_EVENT_MAX_ARGUMENTS];
			void* argument[LUA_EVENT_MAX_ARGUMENTS];
		};

		struct GLuaEvent {
			lua_State* L;
			const char* name;
			int callback;
			double priority = 0.0;
		};

		static int AddEventHandler(lua_State* L);
		static void TriggerEvent(char* name, LuaEventArguments arguments);

	private:
		static bool CompareByPriority(const zLuaEvents::GLuaEvent& event1, const zLuaEvents::GLuaEvent& event2);
		static std::vector<GLuaEvent>& Events();
		static void EventTriggered(void* arguments);
	};
}