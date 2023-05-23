namespace GOTHIC_ENGINE {
	std::vector<zResource>& zResourceManager::Resources() {
		static std::vector<zResource> resource;
		return resource;
	}

	bool zResourceManager::StartResource(char* name) {
		zResource resource(name);
		if (resource.started) {
			Resources().push_back(resource);
			resource.StartLua();
		}

		return resource.started;
	}

	bool zResourceManager::StopResource(char* name) {
		for (auto it = Resources().begin(); it != Resources().end(); ++it) {
			if (strcmp(it->name, name) == 0) {
				zLuaScript script = it->mainLuaFile;
				script.CloseState();
				Resources().erase(it);
				return true;
			}
		}
		return false;
	}

	bool zResourceManager::IsResourceStarted(char* name) {
		for (const auto& resource : Resources()) {
			if (strcmp(resource.name, name) == 0) return true;
		}
		return false;
	}

	void zResourceManager::StartResourceFromConsole(void* args, int argCount) {
		char** arguments = static_cast<char**>(args);
		if (argCount == 1) return zConsole::Log("Use: start <resource name>", LogType.Warning);

		if (zResourceManager::IsResourceStarted(arguments[1])) {
			zConsole::Log("Resource '" + std::string(arguments[1]) + "' is already started", LogType.Warning);
			return;
		}

		zResourceManager::StartResource(arguments[1]);
	}

	void zResourceManager::StopResourceFromConsole(void* args, int argCount) {
		char** arguments = static_cast<char**>(args);
		if (argCount == 1) return zConsole::Log("Use: stop <resource name>", LogType.Warning);

		if (!zResourceManager::IsResourceStarted(arguments[1])) {
			zConsole::Log("Resource '" + std::string(arguments[1]) + "' is not started", LogType.Warning);
			return;
		}

		bool success = zResourceManager::StopResource(arguments[1]);
		if (success) zConsole::Log("Resource '" + std::string(arguments[1]) + "' has been stopped", LogType.Success);
		else zConsole::Log("Failed stopping resource '" + std::string(arguments[1]) + "'", LogType.Error);
	}

	zResource zResourceManager::GetResourceByScript(lua_State* L) {
		for (zResource resource : Resources()) {
			if (resource.mainLuaFile.L == L) {
				return resource;
			}
		}
	}

	void zResourceManager::Init() {
		zConsole::AddCommandHandler("start", StartResourceFromConsole);
		zConsole::AddCommandHandler("stop", StopResourceFromConsole);
	}
}