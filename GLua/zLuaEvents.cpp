namespace GOTHIC_ENGINE {
    std::vector<zLuaEvents::GLuaEvent>& zLuaEvents::Events() {
        static std::vector<GLuaEvent> events;
        return events;
    }

    bool zLuaEvents::CompareByPriority(const zLuaEvents::GLuaEvent& event1, const zLuaEvents::GLuaEvent& event2) {
        return event1.priority < event2.priority;
    }

    enum LuaEventArgumentType {
        Number,
        String,
        Boolean,
        Userdata
    };

    struct LuaEventArguments {
        int argCount = 0;
        int argumentType[LUA_EVENT_MAX_ARGUMENTS];
        void* argument[LUA_EVENT_MAX_ARGUMENTS];
    };

    void zLuaEvents::TriggerEvent(char* name, LuaEventArguments arguments) {
        auto& events = Events();
        std::sort(events.begin(), events.end(), zLuaEvents::CompareByPriority);

        for (const GLuaEvent& event : events) {
            if (strcmp(event.name, name) == 0) {
                lua_rawgeti(event.L, LUA_REGISTRYINDEX, event.callback);

                // Push arguments onto the Lua stack
                for (int i = 0; i < arguments.argCount; i++) {
                    switch (arguments.argumentType[i]) {
                    case LuaEventArgumentType::Number:
                        lua_pushnumber(event.L, *(static_cast<double*>(arguments.argument[i])));
                        break;
                    case LuaEventArgumentType::String:
                        lua_pushstring(event.L, static_cast<char*>(arguments.argument[i]));
                        break;
                    case LuaEventArgumentType::Boolean:
                        lua_pushboolean(event.L, *(static_cast<bool*>(arguments.argument[i])));
                        break;
                    case LuaEventArgumentType::Userdata:
                        lua_pushlightuserdata(event.L, arguments.argument[i]);
                        break;
                    }
                }

                int error = lua_pcall(event.L, arguments.argCount, LUA_MULTRET, 0);

                if (error != LUA_OK) {
                    const char* errorMessage = lua_tostring(event.L, -1);
                    // Handle Lua error
                    // ...
                }
            }
        }
    }

    int zLuaEvents::AddEventHandler(lua_State* L) {
        zLuaArgReader argReader = zLuaArgReader(L);

        const char* eventName = argReader.ReadString(1);
        int eventCallback = argReader.ReadFunction(2);
        double eventPriority = argReader.ReadNumber(3, 0);

        GLuaEvent eventElement;
        eventElement.L = L;
        eventElement.name = eventName;
        eventElement.callback = eventCallback;
        eventElement.priority = eventPriority;
        Events().push_back(eventElement);

        lua_pushboolean(L, true);
        return 1;
    }
}