namespace GOTHIC_ENGINE {
	class zLuaRender {
	public:
		static int DrawRectangle(lua_State* L);
	};

	static const luaL_Reg renderLib[] = {
		{"drawRectangle", zLuaRender::DrawRectangle},
		{ NULL, NULL }
	};
}