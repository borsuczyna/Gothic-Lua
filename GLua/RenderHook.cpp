namespace GOTHIC_ENGINE {
	uintx_t* MethodsTable = NULL;

	_DirectXVersion DirectXVersion;

	WNDCLASSEX WindowClass;
	HWND WindowHwnd;

	int RenderHook::GetDirectXVersion() {
		if (GetModuleHandle("AntTweakBar.dll") != NULL)
			return DirectXVersion.D3D11;
		if (GetModuleHandle("d3d9.dll") != NULL)
			return DirectXVersion.D3D9;
		return DirectXVersion.Unknown;
	}

	bool RenderHook::InitWindow() {
		WindowClass.cbSize = sizeof(WNDCLASSEX);
		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = DefWindowProc;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = GetModuleHandle(NULL);
		WindowClass.hIcon = NULL;
		WindowClass.hCursor = NULL;
		WindowClass.hbrBackground = NULL;
		WindowClass.lpszMenuName = NULL;
		WindowClass.lpszClassName = "MJ";
		WindowClass.hIconSm = NULL;
		RegisterClassEx(&WindowClass);
		WindowHwnd = CreateWindow(WindowClass.lpszClassName, "DirectX Window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, WindowClass.hInstance, NULL);
		if (WindowHwnd == NULL) {
			return false;
		}
		return true;
	}

	bool RenderHook::DeleteWindow() {
		DestroyWindow(WindowHwnd);
		UnregisterClass(WindowClass.lpszClassName, WindowClass.hInstance);
		if (WindowHwnd != NULL) {
			return false;
		}
		return true;
	}

	bool RenderHook::CreateHook(uint16_t Index, void** Original, void* Function) {
		assert(Index >= 0 && Original != NULL && Function != NULL);
		void* target = (void*)MethodsTable[Index];
		if (MH_CreateHook(target, Function, Original) != MH_OK || MH_EnableHook(target) != MH_OK) {
			return false;
		}
		return true;
	}

	void RenderHook::DisableHook(uint16_t Index) {
		assert(Index >= 0);
		MH_DisableHook((void*)MethodsTable[Index]);
	}

	void RenderHook::DisableAll() {
		MH_DisableHook(MH_ALL_HOOKS);
		free(MethodsTable);
		MethodsTable = NULL;
	}
}