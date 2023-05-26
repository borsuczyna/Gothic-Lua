namespace GOTHIC_ENGINE {
	int zLuaColor::ToColor(lua_State* L) {
		zLuaArgReader argReader = zLuaArgReader(L);
		int r = argReader.ReadInt(1, 255);
		int g = argReader.ReadInt(2, 255);
		int b = argReader.ReadInt(3, 255);
		int a = argReader.ReadInt(4, 255);

		lua_pushnumber(L, zColor::ToColor(r, g, b, a));
		return 1;
	}

	int zLuaColor::FromColor(lua_State* L) {
		zLuaArgReader argReader = zLuaArgReader(L);
		int rgba = argReader.ReadInt(1);
		int r, g, b, a;
		zColor::FromColor(rgba, r, g, b, a);

		lua_pushnumber(L, r);
		lua_pushnumber(L, g);
		lua_pushnumber(L, b);
		lua_pushnumber(L, a);
		return 4;
	}
}