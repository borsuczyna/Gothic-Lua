namespace GOTHIC_ENGINE {
	class zLuaScript {
	public:
		lua_State* L;
		bool initialized = false;

		zLuaScript();

		void AddFunctionDefinition(const char* name, lua_CFunction function);
		void LoadDefinitions(const char* name, const luaL_Reg* definitions);
		void DoString(const char* code);
		void DoFile(const char* file);
		void CloseState();
		void LoadDefaultDefinitions();
		void SetRequirePath(const char* path);
	};

	class zLua {
	public:
		static void ExecuteLuaCodeFromConsole(void* args, int argCount);
		static void Init();
	};
}