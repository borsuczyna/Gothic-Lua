namespace GOTHIC_ENGINE {
	zResource::zResource(char* name) {
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
		std::string stringName = std::string(name);

		std::filesystem::path cwd = zFileSystem::GetCurrentDirectory() + ("resources\\" + stringName + "\\");
		resourcePath = cwd.string().c_str();

		if (!zFileSystem::DirectoryExists(resourcePath)) {
			zConsole::Log("Resource '" + stringName + "' does not exist", LogType.Error);
			return;
		}

		if (!zFileSystem::FileExists(std::string(resourcePath) + "main.lua")) {
			zConsole::Log("No entry Lua file for resource '" + stringName + "' was found (main.lua)", LogType.Error);
			return;
		}

		// load main.lua
		this->mainLuaFile = zLuaScript();
		this->mainLuaFile.SetRequirePath(std::string(std::string(resourcePath) + "?.lua").c_str());
		this->mainLuaFile.LoadDefaultDefinitions();

		// finish
		zConsole::Log("Started resource '" + stringName + "'", LogType.Success);
		this->started = true;
	}

	zResource::~zResource() {
		delete[] this->name;
	}

	void zResource::StartLua() {
		this->mainLuaFile.DoFile(std::string(std::string(resourcePath) + "main.lua").c_str());
	}
}