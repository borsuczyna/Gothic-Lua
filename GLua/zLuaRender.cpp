namespace GOTHIC_ENGINE {
	int zLuaRender::DrawRectangle(lua_State* L) {
		zLuaArgReader argReader = zLuaArgReader(L);
		float x = argReader.ReadFloat(1);
		float y = argReader.ReadFloat(2);
		float w = argReader.ReadFloat(3);
		float h = argReader.ReadFloat(4);

		zRender::DrawRectangle(x, y, w, h, ImColor(0, 255, 0));
		return 0;
	}
}