#define DIRECT3D_VERSION         0x0900

#define D3DVECTOR_DEFINED
#define D3DRECT_DEFINED
#define D3DMATRIX_DEFINED
#define D3DLIGHTTYPE_DEFINED
#define D3DSHADEMODE_DEFINED
#define D3DFILLMODE_DEFINED
#define D3DBLEND_DEFINED
#define D3DTEXTUREADDRESS_DEFINED
#define D3DCMPFUNC_DEFINED
#define D3DSTENCILOP_DEFINED
#define D3DFOGMODE_DEFINED
#define D3DZBUFFERTYPE_DEFINED
#define D3DCULL_DEFINED
#define D3DPRIMITIVETYPE_DEFINED
#define D3DTRANSFORMSTATETYPE_DEFINED
#define D3DMATERIALCOLORSOURCE_DEFINED
#define D3DTEXTURESTAGESTATETYPE_DEFINED
#define D3DRENDERSTATETYPE_DEFINED
#define D3DTEXTUREOP_DEFINED
#define D3DSTATEBLOCKTYPE_DEFINED
#define D3DVERTEXBLENDFLAGS_DEFINED
#define D3DTEXTURETRANSFORMFLAGS_DEFINED

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#define DIRECT3D_VERSION         0x0700

namespace GOTHIC_ENGINE {
	class DirectX9 {
	public:
		static bool Init() {
			if (RenderHook::InitWindow() == false) {
				return false;
			}

			HMODULE D3D9Module = GetModuleHandle("d3d9.dll");
			if (D3D9Module == NULL) {
				RenderHook::DeleteWindow();
				return false;
			}

			void* Direct3DCreate9 = GetProcAddress(D3D9Module, "Direct3DCreate9");
			if (Direct3DCreate9 == NULL) {
				RenderHook::DeleteWindow();
				return false;
			}

			LPDIRECT3D9 Direct3D9 = ((LPDIRECT3D9(__stdcall*)(uint32_t))(Direct3DCreate9))(D3D_SDK_VERSION);
			if (Direct3D9 == NULL) {
				RenderHook::DeleteWindow();
				return false;
			}

			D3DDISPLAYMODE DisplayMode;
			if (Direct3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode) < 0) {
				RenderHook::DeleteWindow();
				return false;
			}

			D3DPRESENT_PARAMETERS Params;
			Params.BackBufferWidth = 0;
			Params.BackBufferHeight = 0;
			Params.BackBufferFormat = DisplayMode.Format;
			Params.BackBufferCount = 0;
			Params.MultiSampleType = D3DMULTISAMPLE_NONE;
			Params.MultiSampleQuality = NULL;
			Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
			Params.hDeviceWindow = WindowHwnd;
			Params.Windowed = 1;
			Params.EnableAutoDepthStencil = 0;
			Params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
			Params.Flags = NULL;
			Params.FullScreen_RefreshRateInHz = 0;
			Params.PresentationInterval = 0;

			LPDIRECT3DDEVICE9 Device;
			if (Direct3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, WindowHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT, &Params, &Device) < 0) {
				Direct3D9->Release();
				RenderHook::DeleteWindow();
				return false;
			}

			MethodsTable = (uintx_t*)::calloc(119, sizeof(uintx_t));
			memcpy(MethodsTable, *(uintx_t**)Device, 119 * sizeof(uintx_t));

			MH_Initialize();
			Direct3D9->Release();
			Direct3D9 = NULL;
			Device->Release();
			Device = NULL;
			RenderHook::DeleteWindow();
			return true;
		}
	};
}