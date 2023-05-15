float clamp(float n, float lower, float upper) {
	if (n < lower) return lower;
	if (n > upper) return upper;
	return n;
}

namespace GOTHIC_ENGINE {
	bool visible = false;
	float x = 0, y = 0;

	class zCursor {
	public:
		static void SetVisible(bool state) {
			visible = state;
		}

		static bool IsVisible() {
			return visible;
		}

		static void Update() {
			if (!zRender::Initialized()) return;
			ImGui::GetIO().MouseDrawCursor = visible;
			if (!visible || !zWindow::IsGothicFocused()) return;

			float vx, vy, vz;
			zinput->GetMousePos(vx, vy, vz);
			int px, py, width, height;
			screen->GetViewport(px, py, width, height);
			x = clamp(x + vx, 0, float(width));
			y = clamp(y + vy, 0, float(height));
			RECT rect, crect;
			GetWindowRect(zWindow::GetGothicWindow(), &rect);
			GetClientRect(zWindow::GetGothicWindow(), &crect);

			zRender::DrawRectangle(x, y, 10, 10, ImColor(255, 0, 0));

			bool isDullscreen = zWindow::isFullscreen();
			SetCursorPos(int(x) + rect.left + (isDullscreen ? 0 : 8), int(y) + rect.top + (isDullscreen ? 0 : 31));
		}
	};
}