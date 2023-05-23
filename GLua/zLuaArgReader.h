namespace GOTHIC_ENGINE {
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

		bool ReadBoolean(int arg, bool defaultValue = false) {
			int readBoolean = lua_toboolean(this->L, arg);
			if (readBoolean == 0) {
				return defaultValue;
			}
			return readBoolean != 0;
		}

		lua_CFunction ReadFunction(int arg) {
			if (lua_isfunction(this->L, arg)) {
				lua_pushvalue(this->L, arg);  // Push the function onto the stack
				return lua_tocfunction(this->L, -1);  // Return the C function pointer
			}
			return nullptr;  // Return nullptr if the argument is not a function
		}
	};
}