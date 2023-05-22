namespace GOTHIC_ENGINE {
	class zLuaScript {
	public:
		lua_State* L;
		bool initialized = false;

		zLuaScript();

		void LoadDefinitions(std::string name, const luaL_Reg* definitions);
		void DoString(std::string code);
		void DoFile(std::string file);
		void CloseState();
		void LoadDefaultDefinitions();
		void SetRequirePath(std::string path);
	};

	class zLua {
	public:
		static void ExecuteLuaCodeFromConsole(void* args, int argCount);
		static void Init();
	};
}