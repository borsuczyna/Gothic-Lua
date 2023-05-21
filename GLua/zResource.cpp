namespace GOTHIC_ENGINE {
	class zResource {
	public:
		std::string name;
		std::string resourcePath;
		bool started = false;

	private:
		zLuaScript mainLuaFile;
		/*static std::vector<zLuaScript>& Scripts() {
			static std::vector<zLuaScript> scripts;
			return scripts;
		}*/

	public:
		zResource(std::string name) {
			this->name = name;

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
			mainLuaFile = zLuaScript();
			mainLuaFile.SetRequirePath(resourcePath + "?.lua");
			mainLuaFile.LoadDefaultDefinitions();
			mainLuaFile.DoFile(resourcePath + "main.lua");

			// finish
			zConsole::Log("Started resource '" + name, LogType.Success);
			this->started = true;
		}

		bool DoesScriptComesFromResource(lua_State* L) {
			return (mainLuaFile.L == L);
		}
	};
}