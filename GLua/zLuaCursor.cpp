namespace GOTHIC_ENGINE {
	int zLuaCursor::GetPosition(lua_State* L) {
		lua_pushnumber(L, zCursor::x);
		lua_pushnumber(L, zCursor::y);

		return 2;
	}
}