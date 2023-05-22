namespace GOTHIC_ENGINE {
	zLuaScript::zLuaScript() {
		this->L = luaL_newstate();
		luaL_openlibs(this->L);

		this->initialized = true;
	}

	void zLuaScript::LoadDefinitions(std::string name, const luaL_Reg* definitions) {
		lua_newtable(this->L);
		luaL_setfuncs(this->L, definitions, 0);
		lua_setglobal(this->L, name.c_str());
	}

	void zLuaScript::DoString(std::string code) {
		if (luaL_dostring(this->L, code.c_str()) != LUA_OK) {
			printf("Error: %s\n", lua_tostring(this->L, -1));
		}
	}

	void zLuaScript::DoFile(std::string file) {
		if (luaL_dofile(this->L, file.c_str()) != LUA_OK) {
			printf("Error: %s\n", lua_tostring(this->L, -1));
		}
	}

	void zLuaScript::CloseState() {
		lua_close(this->L);
	}

	void zLuaScript::LoadDefaultDefinitions() {
		static const luaL_Reg consoleLib[] = {
			{"log", zLuaConsole::Log},			// console.log(string: text)
			{ NULL, NULL }
		};

		LoadDefinitions("console", consoleLib);
	}

	void zLuaScript::SetRequirePath(std::string path) {
		lua_getglobal(this->L, "package");  // Get the "package" table
		lua_pushstring(this->L, path.c_str());  // Push the new require path
		lua_setfield(this->L, -2, "path");  // Set the "path" field in the "package" table
		lua_pop(this->L, 1);  // Pop the "package" table from the stack
	}

	void zLua::ExecuteLuaCodeFromConsole(void* args, int argCount) {
		char** arguments = static_cast<char**>(args);
		std::string concatenatedString;

		for (int i = 1; i < argCount; ++i) {
			concatenatedString += arguments[i];
			if (i < argCount - 1) concatenatedString += " ";
		}

		zLuaScript l = zLuaScript();
		l.LoadDefaultDefinitions();
		l.DoString(concatenatedString.c_str());
		l.CloseState();
	}

	void zLua::Init() {
		zConsole::AddCommandHandler("lua", ExecuteLuaCodeFromConsole);
	}
}