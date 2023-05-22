namespace GOTHIC_ENGINE {
	zResource::zResource(std::string name) {
		this->name = name;
		printf("init %s\n", name.c_str());

		std::filesystem::path cwd = zFileSystem::GetCurrentDirectory() + ("resources\\" + name + "\\");
		resourcePath = cwd.string();

		if (!zFileSystem::DirectoryExists(resourcePath)) {
			zConsole::Log("Resource '" + name + "' does not exist", LogType.Error);
			return;
		}

		if (!zFileSystem::FileExists(resourcePath + "main.lua")) {
			zConsole::Log("No entry Lua file for resource '" + name + "' was found (main.lua)", LogType.Error);
			return;
		}

		// load main.lua
		this->mainLuaFile = zLuaScript();
		this->mainLuaFile.SetRequirePath(resourcePath + "?.lua");
		this->mainLuaFile.LoadDefaultDefinitions();

		// finish
		zConsole::Log("Started resource '" + name + "'", LogType.Success);
		this->started = true;
	}

	void zResource::StartLua() {
		this->mainLuaFile.DoFile(resourcePath + "main.lua");
	}
}