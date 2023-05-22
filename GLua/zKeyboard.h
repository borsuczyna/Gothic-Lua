namespace GOTHIC_ENGINE {
    class zKeyboard {
    public:
        static bool WasKeyPressed(int key);
        static bool WasKeyReleased(int key);
        static bool IsKeyDown(int key);
        static void Update();
    };
}