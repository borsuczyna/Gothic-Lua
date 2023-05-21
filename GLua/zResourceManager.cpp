namespace GOTHIC_ENGINE {	
	static std::vector<zResource>& Resources() {
		static std::vector<zResource> resource;
		return resource;
	}

	class zResourceManager {
	public:
		static bool StartResource(std::string name) {
			zResource resource(name);
			if (resource.started)
				Resources().push_back(resource);

			return resource.started;
		}

		static bool IsResourceStarted(std::string name) {
			for (const auto& resource : Resources()) {
				if (resource.name == name) return true;
			}
			return false;
		}

		static void StartResourceFromConsole(void* args, int argCount) {
			char** arguments = static_cast<char**>(args);
			if (argCount == 1) return zConsole::Log("Use: start <resource name>", LogType.Warning);
			std::string resourceName(arguments[1]);

			if (zResourceManager::IsResourceStarted(resourceName)) {
				zConsole::Log("Resource '" + resourceName + "' is already started", LogType.Warning);
				return;
			}

			zResourceManager::StartResource(resourceName);
		}

		static zResource GetResourceByScript(lua_State* L) {
			for (zResource resource : Resources()) {
				if (resource.DoesScriptComesFromResource(L)) {
					return resource;
				}
			}
		}

		static void Init() {
			zConsole::AddCommandHandler("start", StartResourceFromConsole);
		}
	};
}