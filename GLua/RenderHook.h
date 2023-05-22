#if defined _M_X64
typedef uint64_t uintx_t;
#elif defined _M_IX86
typedef uint32_t uintx_t;
#endif

extern uintx_t* MethodsTable;

namespace GOTHIC_ENGINE {
	struct _DirectXVersion {
		int Unknown = 0;
		int D3D7 = 1;
		int D3D9 = 2;
		int D3D11 = 3;
	};

	extern _DirectXVersion DirectXVersion;

	extern WNDCLASSEX WindowClass;
	extern HWND WindowHwnd;

	class RenderHook {
	public:
		static int GetDirectXVersion();
		static bool InitWindow();
		static bool DeleteWindow();
		static bool CreateHook(uint16_t Index, void** Original, void* Function);
		static void DisableHook(uint16_t Index);
		static void DisableAll();
	};
}