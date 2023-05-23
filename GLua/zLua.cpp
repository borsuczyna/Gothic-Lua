namespace GOTHIC_ENGINE {
	zLuaScript::zLuaScript() {
		this->L = luaL_newstate();
		luaL_openlibs(this->L);

		this->initialized = true;
	}

	void zLuaScript::AddFunctionDefinition(const char* name, lua_CFunction function) {
		lua_pushcfunction(L, function);
		lua_setglobal(L, name);
	}

	void zLuaScript::LoadDefinitions(const char* name, const luaL_Reg* definitions) {
		lua_newtable(this->L);
		luaL_setfuncs(this->L, definitions, 0);
		lua_setglobal(this->L, name);
	}

	void zLuaScript::DoString(const char* code) {
		if (luaL_dostring(this->L, code) != LUA_OK) {
			printf("Error: %s\n", lua_tostring(this->L, -1));
		}
	}

	void zLuaScript::DoFile(const char* file) {
		if (luaL_dofile(this->L, file) != LUA_OK) {
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
		AddFunctionDefinition("print", zLuaConsole::Log);

		// Events
		AddFunctionDefinition("addEventHandler", zLuaEvents::AddEventHandler);
	}

	void zLuaScript::SetRequirePath(const char* path) {
		lua_getglobal(this->L, "package");  // Get the "package" table
		lua_pushstring(this->L, path);  // Push the new require path
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
		l.DoString((char*)concatenatedString.c_str());
		l.CloseState();
	}

	void zLua::Init() {
		zConsole::AddCommandHandler("lua", ExecuteLuaCodeFromConsole);
	}
}