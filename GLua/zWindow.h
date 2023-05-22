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
        static void SetResolution(int width, int height);
        static void GetResolution(int& width, int& height);
        static void Update();
    };
}