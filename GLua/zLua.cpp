namespace GOTHIC_ENGINE {
	class zLuaScript
	{
		lua_State* L = NULL;

	public:
		zLuaScript()
		{
			L = luaL_newstate();
			luaL_openlibs(L);
		}

		void loadDefinitions(char* name, const luaL_Reg* definitions) {
			lua_newtable(L);
			luaL_setfuncs(L, definitions, 0);
			lua_setglobal(L, name);
		}

		void doString(const char* code)
		{
			if (luaL_dostring(L, code) != LUA_OK)
			{
				printf("Error: %s\n", lua_tostring(L, -1));
			}
		}

		void doFile(const char* file)
		{
			if (luaL_dofile(L, file) != LUA_OK)
			{
				printf("Error: %s\n", lua_tostring(L, -1));
			}
		}

		void closeState()
		{
			lua_close(L);
		}
	};

	class zLua {
	public:
		static void ExecuteLuaCodeFromConsole(void* args, int argCount) {
			char** arguments = static_cast<char**>(args);
			std::string concatenatedString;

			for (int i = 1; i < argCount; ++i) {
				concatenatedString += arguments[i];
				if (i < argCount - 1) concatenatedString += " ";
			}

			static const luaL_Reg consoleLib[] = {
				{"log", zLuaConsole::Log}, // console.log(string: text)
				{ NULL, NULL }
			};

			zLuaScript l;
			l.loadDefinitions("console", consoleLib);
			l.doString(concatenatedString.c_str());
			l.closeState();
		}

		static void Init() {
			zConsole::AddCommandHandler("lua", ExecuteLuaCodeFromConsole);
		}
	};
}