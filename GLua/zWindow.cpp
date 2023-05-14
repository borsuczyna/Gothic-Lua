LONG_PTR dwStyle;
bool windowedModeInitialized = false;

int resolutions[4][2] = {
	{800, 600},
	{1024, 768},
	{1280, 720},
	{1920, 1080},
};
int currentResolution = 0;

namespace GOTHIC_ENGINE {
	class zWindow {
	private:
		static void InitWindowed() {
			HWND hWnd = GetGothicWindow();
			if (hWnd == (HWND)-1) return;

			hWnd = GetForegroundWindow();
			dwStyle = GetWindowLongPtr(hWnd, GWL_STYLE);

			dwStyle &= ~(WS_POPUP | WS_MAXIMIZEBOX);
			dwStyle |= WS_OVERLAPPEDWINDOW;

			SetWindowLongPtr(hWnd, GWL_STYLE, dwStyle);

			RECT rcClient;
			GetClientRect(hWnd, &rcClient);
			AdjustWindowRect(&rcClient, dwStyle, FALSE);

			zWindow::SetSize(1920, 1080);
			ogame->UpdateScreenResolution();

			windowedModeInitialized = true;
		}
	public:
		static bool isFullscreen()
		{
			HWND hwnd = GetGothicWindow();
			RECT windowRect;
			GetClientRect(hwnd, &windowRect);
			return windowRect.left == 0 && windowRect.top == 0 && windowRect.right == GetSystemMetrics(SM_CXSCREEN) && windowRect.bottom == GetSystemMetrics(SM_CYSCREEN);
		}

		static bool IsGothicFocused() {
			HWND hWnd = GetForegroundWindow();
			if (hWnd == NULL) return false;

			char buffer[256];
			GetWindowText(hWnd, buffer, sizeof(buffer));
			for (int i = 0; i < 6; i++) buffer[i] = tolower(buffer[i]);
			if (strncmp(buffer, "gothic", 6) != 0) return false;

			return true;
		}

		static HWND GetGothicWindow() {
			if (!zWindow::IsGothicFocused()) return (HWND)-1;
			HWND hWnd = GetForegroundWindow();
			 
			return hWnd;
		}

		static void SetSize(int width, int height) {
			HWND hWnd = GetGothicWindow();
			if (hWnd == (HWND)-1) return;

			int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
			int y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

			zCView::SetMode(width, height, 32, &hWnd);
			zCPosition pos = zCPosition();
			pos.X = 8192;
			pos.Y = 8192;
			zCViewDraw::GetScreen().SetVirtualSize(pos);
			SetWindowPos(hWnd, NULL, x, y, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}

		static void zWindow::Update() {
			if (!windowedModeInitialized) return InitWindowed();
			if (!(zKeyboard::IsKeyDown(VK_CONTROL) && zKeyboard::WasKeyPressed('L'))) return;

			currentResolution = (currentResolution + 1) % 4;
			SetSize(resolutions[currentResolution][0], resolutions[currentResolution][1]);

			printf("changed resolution\n");
		}
	};
}