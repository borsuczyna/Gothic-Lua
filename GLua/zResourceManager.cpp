namespace GOTHIC_ENGINE {
	std::vector<zResource>& zResourceManager::Resources() {
		static std::vector<zResource> resource;
		return resource;
	}

	bool zResourceManager::StartResource(std::string name) {
		zResource resource(name);
		if (resource.started) {
			Resources().push_back(resource);
			resource.StartLua();
		}

		return resource.started;
	}

	bool zResourceManager::IsResourceStarted(std::string name) {
		for (const auto& resource : Resources()) {
			if (resource.name == name) return true;
		}
		return false;
	}

	void zResourceManager::StartResourceFromConsole(void* args, int argCount) {
		char** arguments = static_cast<char**>(args);
		if (argCount == 1) return zConsole::Log("Use: start <resource name>", LogType.Warning);
		std::string resourceName(arguments[1]);

		if (zResourceManager::IsResourceStarted(resourceName)) {
			zConsole::Log("Resource '" + resourceName + "' is already started", LogType.Warning);
			return;
		}

		zResourceManager::StartResource(resourceName);
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
	}
}