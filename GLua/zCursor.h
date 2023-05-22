namespace GOTHIC_ENGINE {
	class zCursor {
	private:
		static bool visible;
		static float x;
		static float y;

	public:
		static void SetVisible(bool state);
		static bool IsVisible();
		static void Update();
	};
}