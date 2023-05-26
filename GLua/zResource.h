namespace GOTHIC_ENGINE {
	class zResource {
	public:
		zLuaScript mainLuaFile;
		std::string name;
		const char* resourcePath;
		bool started = false;

		zResource(std::string name);
		bool DoesScriptComesFromResource(lua_State* L);
		void StartLua();
	};
}