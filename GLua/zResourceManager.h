namespace GOTHIC_ENGINE {
	class zResourceManager {
	public:
		static std::vector<zResource>& Resources();
		static bool StartResource(std::string name);
		static bool IsResourceStarted(std::string name);
		static void StartResourceFromConsole(void* args, int argCount);
		static zResource GetResourceByScript(lua_State* L);
		static void Init();
	};
}