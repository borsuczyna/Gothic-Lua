namespace GOTHIC_ENGINE {
	class zLuaConsole {
	public:
		static int Log(lua_State* L) {
			const char* readString = luaL_checkstring(L, 1);
			std::string message = "[temp runtime] " + std::string(readString);
			zConsole::Log(message, LogType.Default);

			return 0;
		}
	};
}