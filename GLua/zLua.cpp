namespace GOTHIC_ENGINE {
	class zLuaScript
	{
	public:
		lua_State* L = NULL;

		zLuaScript()
		{
			L = luaL_newstate();
			luaL_openlibs(L);
		}

		void LoadDefinitions(std::string name, const luaL_Reg* definitions) {
			lua_newtable(L);
			luaL_setfuncs(L, definitions, 0);
			lua_setglobal(L, name.c_str());
		}

		void DoString(std::string code)
		{
			if (luaL_dostring(L, code.c_str()) != LUA_OK)
			{
				printf("Error: %s\n", lua_tostring(L, -1));
			}
		}

		void DoFile(std::string file)
		{
			if (luaL_dofile(L, file.c_str()) != LUA_OK)
			{
				printf("Error: %s\n", lua_tostring(L, -1));
			}
		}

		void CloseState()
		{
			lua_close(L);
		}

		void LoadDefaultDefinitions() {
			static const luaL_Reg consoleLib[] = {
				{"log", zLuaConsole::Log},			// console.log(string: text)
				{ NULL, NULL }
			};

			LoadDefinitions("console", consoleLib);
		}

		void SetRequirePath(std::string path) {
			lua_getglobal(L, "package");  // Get the "package" table
			lua_pushstring(L, path.c_str());  // Push the new require path
			lua_setfield(L, -2, "path");  // Set the "path" field in the "package" table
			lua_pop(L, 1);  // Pop the "package" table from the stack
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

			zLuaScript l;
			l.LoadDefaultDefinitions();
			l.DoString(concatenatedString.c_str());
			l.CloseState();
		}

		static void Init() {
			zConsole::AddCommandHandler("lua", ExecuteLuaCodeFromConsole);
		}
	};
}