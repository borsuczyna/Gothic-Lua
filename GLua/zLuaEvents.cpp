namespace GOTHIC_ENGINE {
    std::vector<zLuaEvents::GLuaEvent>& zLuaEvents::Events() {
        static std::vector<GLuaEvent> events;
        return events;
    }

    bool zLuaEvents::CompareByPriority(const zLuaEvents::GLuaEvent& event1, const zLuaEvents::GLuaEvent& event2) {
        return event1.priority < event2.priority;
    }

    void zLuaEvents::EventTriggered(void* arguments) {
        EventTriggeredArguments* eventArgs = static_cast<EventTriggeredArguments*>(arguments);

        auto& events = Events();
        std::sort(events.begin(), events.end(), zLuaEvents::CompareByPriority);

        for (const GLuaEvent& event : events) {
            if (strcmp(event.name, eventArgs->eventName) == 0) {
                lua_rawgeti(event.L, LUA_REGISTRYINDEX, event.callback);
                int error = lua_pcall(event.L, 0, LUA_MULTRET, 0);
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

    void zLuaEvents::Init() {
        zEvents::AddEventHandler("EventTriggered", zLuaEvents::EventTriggered);
    }
}