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

	void zLuaScript::RemoveDefinition(const char* name) {
		lua_pushnil(this->L);
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
		zLuaEvents::RemoveLuaRefs(this->L);
		lua_close(this->L);
		this->L = nullptr;
	}

	void zLuaScript::LoadDefaultDefinitions() {
		// Console
		LoadDefinitions("console", consoleLib);
		AddFunctionDefinition("print", zLuaConsole::Log);

		// Events
		LoadDefinitions("event", eventLib);
		AddFunctionDefinition("addEventHandler", zLuaEvents::AddEventHandler);

		// Cursor
		LoadDefinitions("cursor", cursorLib);
		AddFunctionDefinition("getCursorPosition", zLuaCursor::GetPosition);

		// Render
		LoadDefinitions("render", renderLib);
		AddFunctionDefinition("dxDrawRectangle", zLuaRender::DrawRectangle);

		// Colors
		AddFunctionDefinition("tocolor", zLuaColor::ToColor);
		AddFunctionDefinition("fromcolor", zLuaColor::FromColor);

		// Remove for safety reasons
		RemoveDefinition("os");
		RemoveDefinition("io");
		RemoveDefinition("load");
		RemoveDefinition("loadfile");
		RemoveDefinition("dofile");
		RemoveDefinition("debug");
	}

	void zLuaScript::SetRequirePath(const char* path) {
		lua_getglobal(this->L, "package");
		lua_pushstring(this->L, path);
		lua_setfield(this->L, -2, "path");
		lua_pop(this->L, 1);
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