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
			if (!visible) return;

			float vx, vy, vz;
			zinput->GetMousePos(vx, vy, vz);
			int px, py, width, height;
			screen->GetViewport(px, py, width, height);
			x = clamp(x + vx, 0, float(width));
			y = clamp(y + vy, 0, float(height));

			zRender::DrawRectangle(x, y, 50, 50);

			SetCursorPos(int(x), int(y));
		}
	};
}