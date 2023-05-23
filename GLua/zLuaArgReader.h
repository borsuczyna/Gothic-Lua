namespace GOTHIC_ENGINE {
	enum class LUA_ARG_TYPE {
		NONE,
		STRING,
		NUMBER,
		BOOLEAN,
		FUNCTION,
		TABLE
	};

	class zLuaArgReader {
		lua_State* L;

	public:
		zLuaArgReader(lua_State* L) {
			this->L = L;
		}

		const char* ReadString(int arg, const char* defaultValue = nullptr) {
			const char* readString = luaL_optstring(this->L, arg, defaultValue);
			return readString;
		}

		double ReadNumber(int arg, double defaultValue = 0.0) {
			double readNumber = luaL_optnumber(this->L, arg, defaultValue);
			return readNumber;
		}

		int ReadInt(int arg, int defaultValue = 0) {
			int readNumber = int(luaL_optnumber(this->L, arg, defaultValue));
			return readNumber;
		}

		float ReadFloat(int arg, float defaultValue = 0.0f) {
			float readNumber = float(luaL_optnumber(this->L, arg, defaultValue));
			return readNumber;
		}

		bool ReadBoolean(int arg, bool defaultValue = false) {
			int readBoolean = lua_toboolean(this->L, arg);
			if (readBoolean == 0) {
				return defaultValue;
			}
			return readBoolean != 0;
		}

		int ReadFunction(int arg) {
			luaL_checktype(L, arg, LUA_TFUNCTION);
			lua_pushvalue(L, arg);
			return luaL_ref(L, LUA_REGISTRYINDEX);
		}

		int ArgCount() {
			return lua_gettop(L);
		}

		LUA_ARG_TYPE NextArgType(int arg) {
			if (lua_isstring(L, arg)) {
				return LUA_ARG_TYPE::STRING;
			}
			else if (lua_isnumber(L, arg)) {
				return LUA_ARG_TYPE::NUMBER;
			}
			else if (lua_isboolean(L, arg)) {
				return LUA_ARG_TYPE::BOOLEAN;
			}
			else if (lua_isfunction(L, arg)) {
				return LUA_ARG_TYPE::FUNCTION;
			}
			else if (lua_istable(L, arg)) {
				return LUA_ARG_TYPE::TABLE;
			}
			else {
				return LUA_ARG_TYPE::NONE;
			}
		}
	};
}