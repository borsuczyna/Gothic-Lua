namespace GOTHIC_ENGINE {	
	static std::vector<zResource>& Resources() {
		static std::vector<zResource> resource;
		return resource;
	}

	class zEngine {
	public:
		static bool StartResource(std::string name) {
			zResource resource(name);
			//Resources().push_back(resource);
			return true;
		}

		static void StartResourceFromConsole(void* args, int argCount) {
			char** arguments = static_cast<char**>(args);
			if (argCount == 1) return zConsole::Log("Use: start <resource name>", LogType.Warning);
			std::string resourceName(arguments[1]);

			zEngine::StartResource(resourceName);
		}

		static void Init() {
			zConsole::AddCommandHandler("start", StartResourceFromConsole);
		}
	};
}