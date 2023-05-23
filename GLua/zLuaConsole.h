namespace GOTHIC_ENGINE {
	class zLuaConsole {
	public:
		static int Log(lua_State* L);
	};

	static const luaL_Reg consoleLib[] = {
		{"log", zLuaConsole::Log},
		{ NULL, NULL }
	};
}