namespace GOTHIC_ENGINE {
	class zResource {
	private:
		std::string resourcePath;

		static std::vector<zLuaScript>& Scripts() {
			static std::vector<zLuaScript> scripts;
			return scripts;
		}

	public:
		zResource(std::string name) {
			resourcePath = "resources/" + name + '/';

			if (!zUtils::DirectoryExists(resourcePath)) {
				zConsole::Log("Resource '" + name + "' does not exist.", LogType.Error);
			}

			zUtils::ScanDirectory("resources/");
			printf("loading resource %s\n", name.c_str());

			zConsole::Log("Started resource '" + name + "'.", LogType.Success);
		}
	};
}