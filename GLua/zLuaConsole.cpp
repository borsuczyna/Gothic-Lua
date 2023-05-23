namespace GOTHIC_ENGINE {
	int zLuaConsole::Log(lua_State* L) {
		zLuaArgReader argReader = zLuaArgReader(L);
		int numArgs = argReader.ArgCount();

		std::stringstream logMessage;
		zResource resource = zResourceManager::GetResourceByScript(L);
		std::string resourceName = resource.started ? resource.name : "temp runtime";

		// Concatenate all the arguments into a single log message
		for (int i = 1; i <= numArgs; i++) {
			logMessage << luaL_tolstring(L, i, nullptr);
			
			if (i < numArgs) {
				logMessage << "\t";
			}
		}

		zConsole::Log("[" + resourceName + "] " + logMessage.str(), LogType.Default);

		return 0;
	}
}
