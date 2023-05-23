namespace GOTHIC_ENGINE {
	class zResourceManager {
	public:
		static std::vector<zResource>& Resources();
		static bool StartResource(char* name);
		static bool StopResource(char* name);
		static bool IsResourceStarted(char* name);
		static void StartResourceFromConsole(void* args, int argCount);
		static void StopResourceFromConsole(void* args, int argCount);
		static zResource GetResourceByScript(lua_State* L);
		static void Init();
	};
}