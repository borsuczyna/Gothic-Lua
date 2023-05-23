namespace GOTHIC_ENGINE {
	class zResource {
	public:
		zLuaScript mainLuaFile;
		char* name;
		const char* resourcePath;
		bool started = false;

		zResource(char* name);
		bool DoesScriptComesFromResource(lua_State* L);
		void StartLua();
	};
}