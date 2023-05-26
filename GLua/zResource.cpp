namespace GOTHIC_ENGINE {
	zResource::zResource(std::string name) {
		this->name = name;

		std::filesystem::path cwd = zFileSystem::GetCurrentDirectory() + ("resources\\" + this->name + "\\");
		resourcePath = cwd.string().c_str();

		if (!zFileSystem::DirectoryExists(resourcePath)) {
			zConsole::Log("Resource '" + this->name + "' does not exist", LogType.Error);
			return;
		}

		if (!zFileSystem::FileExists(std::string(resourcePath) + "main.lua")) {
			zConsole::Log("No entry Lua file for resource '" + this->name + "' was found (main.lua)", LogType.Error);
			return;
		}

		// load main.lua
		this->mainLuaFile = zLuaScript();
		this->mainLuaFile.SetRequirePath(std::string(std::string(resourcePath) + "?.lua").c_str());
		this->mainLuaFile.LoadDefaultDefinitions();

		// finish
		zConsole::Log("Started resource '" + this->name + "'", LogType.Success);
		this->started = true;
	}

	void zResource::StartLua() {
		this->mainLuaFile.DoFile(std::string(std::string(resourcePath) + "main.lua").c_str());
	}
}