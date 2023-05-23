namespace GOTHIC_ENGINE {
	class zCursor {
	private:
		static bool visible;

	public:
		static float x;
		static float y;

		static void SetVisible(bool state);
		static bool IsVisible();
		static void Update();
	};
}