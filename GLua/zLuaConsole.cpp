namespace GOTHIC_ENGINE {
	int zLuaConsole::Log(lua_State* L) {
		const char* readString = luaL_checkstring(L, 1);
		zResource resource = zResourceManager::GetResourceByScript(L);
		std::string resourceName = "temp runtime";
		if(resource.started)
			resourceName = resource.name;

		zConsole::Log("[" + resourceName + "] " + std::string(readString), LogType.Default);

		return 0;
	}
}