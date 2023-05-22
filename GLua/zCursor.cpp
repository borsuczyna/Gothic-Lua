namespace GOTHIC_ENGINE {
	bool zCursor::visible = false;
	float zCursor::x = 0;
	float zCursor::y = 0;

	void zCursor::SetVisible(bool state) {
		visible = state;
	}

	bool zCursor::IsVisible() {
		return visible;
	}

	void zCursor::Update() {
		if (!zRender::Initialized()) return;
		ImGui::GetIO().MouseDrawCursor = visible;
		if (!visible || !zWindow::IsGothicFocused()) return;

		float vx, vy, vz;
		zinput->GetMousePos(vx, vy, vz);
		int px, py, width, height;
		screen->GetViewport(px, py, width, height);
		x = zUtils::ClampFloat(x + vx, 0, float(width));
		y = zUtils::ClampFloat(y + vy, 0, float(height));
		RECT rect, crect;
		GetWindowRect(zWindow::GetGothicWindow(), &rect);
		GetClientRect(zWindow::GetGothicWindow(), &crect);

		zRender::DrawRectangle(x, y, 10, 10, ImColor(255, 0, 0));

		bool isFullscreen = zWindow::isFullscreen();
		SetCursorPos(int(x) + rect.left + (isFullscreen ? 0 : 8), int(y) + rect.top + (isFullscreen ? 0 : 31));
	}
}