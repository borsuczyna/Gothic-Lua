namespace GOTHIC_ENGINE {
    class zWindow {
    private:
        static LONG_PTR dwStyle;
        static bool windowedModeInitialized;

        static void InitWindowed();

    public:
        static bool isFullscreen();
        static bool IsGothicFocused();
        static HWND GetGothicWindow();
        static void SetSize(int width, int height);
        static void Update();
    };
}