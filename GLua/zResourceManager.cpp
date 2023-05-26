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

	bool zResourceManager::StopResource(std::string name) {
		auto& resources = Resources();
		for (auto it = resources.begin(); it != resources.end();) {
			if (it->name == name) {
				zLuaScript script = it->mainLuaFile;
				script.CloseState();
				it = resources.erase(it);

				return true;
			}
			else it++;
		}

		return false;
	}

	bool zResourceManager::RestartResource(std::string name) {
		if (!StopResource(name)) return false;
		if (!StartResource(name)) return false;
		return true;
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

	void zResourceManager::RestartResourceFromConsole(void* args, int argCount) {
		char** arguments = static_cast<char**>(args);
		if (argCount == 1) return zConsole::Log("Use: stop <resource name>", LogType.Warning);

		if (!zResourceManager::IsResourceStarted(arguments[1])) {
			zConsole::Log("Resource '" + std::string(arguments[1]) + "' is not started", LogType.Warning);
			return;
		}

		bool success = zResourceManager::RestartResource(arguments[1]);
		if (success) zConsole::Log("Resource '" + std::string(arguments[1]) + "' has been restarted", LogType.Success);
		else zConsole::Log("Failed restarting resource '" + std::string(arguments[1]) + "'", LogType.Error);
	}

	void zResourceManager::GetCurrentResources(void* args, int argCount) {
		char** arguments = static_cast<char**>(args);
		zConsole::Log("", LogType.Warning);
		zConsole::Log("List of all running resources:", LogType.Warning);

		for (zResource resource : Resources()) {
			zConsole::Log("  " + std::string(resource.name), LogType.Default);
		}
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
		zConsole::AddCommandHandler("restart", RestartResourceFromConsole);
		zConsole::AddCommandHandler("resources", GetCurrentResources);
	}
}