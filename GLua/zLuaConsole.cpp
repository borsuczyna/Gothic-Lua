namespace GOTHIC_ENGINE {
	int zLuaConsole::Log(lua_State* L) {
		zLuaArgReader argReader = zLuaArgReader(L);
		const char* readString = argReader.ReadString(1);

		zResource resource = zResourceManager::GetResourceByScript(L);
		std::string resourceName = "temp runtime";
		if(resource.started)
			resourceName = resource.name;

		zConsole::Log("[" + resourceName + "] " + std::string(readString), LogType.Default);

		return 0;
	}
}