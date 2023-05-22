namespace GOTHIC_ENGINE {
	class zLuaConsole {
	public:
		static int Log(lua_State* L) {
			const char* readString = luaL_checkstring(L, 1);
			zConsole::Log(std::string(readString), LogType.Default);

			return 0;
		}
	};
}