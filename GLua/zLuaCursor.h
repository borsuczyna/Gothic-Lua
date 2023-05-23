namespace GOTHIC_ENGINE {
	class zLuaCursor {
	public:
		static int GetPosition(lua_State* L);
	};

	static const luaL_Reg cursorLib[] = {
		{"getPosition", zLuaCursor::GetPosition},
		{ NULL, NULL }
	};
}