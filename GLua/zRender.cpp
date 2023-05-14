#include "zRender.h"
#include <Directories/ImGui/imgui_impl_dx9.h>

#define MAX_DRAW_QUEUE 1024

bool ShowMenu = false;
bool ImGui_Initialised = false;
bool initializedRender = false;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Process {
	DWORD ID;
	HANDLE Handle;
	HWND Hwnd;
	HMODULE Module;
	WNDPROC WndProc;
	int WindowWidth;
	int WindowHeight;
	LPCSTR Title;
	LPCSTR ClassName;
	LPCSTR Path;
}

namespace DirectX11Interface {
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;
	ID3D11RenderTargetView* RenderTargetView;
}


namespace GOTHIC_ENGINE {
	int DXVersion = DirectXVersion.Unknown;

	// queue
	struct Rectangle {
		float x;
		float y;
		float w;
		float h;
		ImColor color;
		bool inUse;
	};

	static LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[0] = zinput->GetMouseButtonPressedLeft();
		return CallWindowProc(Process::WndProc, hwnd, uMsg, wParam, lParam);
	}

	Rectangle rectangleQueue[MAX_DRAW_QUEUE] = {};

	static void DrawQueuedElements() {
		// rectangles
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		for (int i = 0; i < MAX_DRAW_QUEUE; i++) {
			Rectangle rectangle = rectangleQueue[i];
			if (rectangle.inUse) {
				ImVec2 start(rectangle.x, rectangle.y);
				ImVec2 end(rectangle.x + rectangle.w, rectangle.y + rectangle.h);

				drawList->AddRectFilled(start, end, rectangle.color);
				printf("rectangle: %f, %f, %f, %f\n", start.x, start.y, end.x, end.y);
				rectangleQueue[i].inUse = false;
			}
		}
	}

	static void APIENTRY MJDrawIndexed(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) {
		
	}

	static HMODULE GetThisDllHandle()
	{
		MEMORY_BASIC_INFORMATION info;
		size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)GetThisDllHandle, &info, sizeof(info));
		return len ? (HMODULE)info.AllocationBase : NULL;
	}

	HRESULT APIENTRY MJEndSceneDX9(IDirect3DDevice9* pDevice) {
		if (!ImGui_Initialised) {
			ImGui::CreateContext();

			ImGuiIO& io = ImGui::GetIO(); (void)io;
			ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

			ImGui_ImplWin32_Init(Process::Hwnd);
			ImGui_ImplDX9_Init(pDevice);
			ImGui_ImplDX9_CreateDeviceObjects();
			ImGui::GetIO().ImeWindowHandle = Process::Hwnd;
			Process::WndProc = (WNDPROC)SetWindowLongPtr(Process::Hwnd, GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProc);
			ImGui_Initialised = true;
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::GetIO().MouseDrawCursor = true;
		ImGui::ShowDemoWindow();

		DrawQueuedElements();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		return oEndScene(pDevice);
	}

	static HRESULT APIENTRY MJPresentDX9(IDirect3DDevice9* pDevice, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion) {
		return oPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}

	static HRESULT APIENTRY MJDrawIndexedPrimitiveDX9(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) {
		return oDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}

	static HRESULT APIENTRY MJDrawPrimitiveDX9(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) {
		return oDrawPrimitive(pDevice, PrimitiveType, StartVertex, PrimitiveCount);
	}

	static HRESULT APIENTRY MJSetTextureDX9(LPDIRECT3DDEVICE9 pDevice, DWORD Sampler, IDirect3DBaseTexture9* pTexture) {
		return oSetTexture(pDevice, Sampler, pTexture);
	}

	static HRESULT APIENTRY MJResetDX9(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters) {
		return oReset(pDevice, pPresentationParameters);;
	}

	static HRESULT APIENTRY MJSetStreamSourceDX9(LPDIRECT3DDEVICE9 pDevice, UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT sStride) {
		return oSetStreamSource(pDevice, StreamNumber, pStreamData, OffsetInBytes, sStride);
	}

	static HRESULT APIENTRY MJSetVertexDeclarationDX9(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexDeclaration9* pdecl) {
		return oSetVertexDeclaration(pDevice, pdecl);
	}

	static HRESULT APIENTRY MJSetVertexShaderConstantFDX9(LPDIRECT3DDEVICE9 pDevice, UINT StartRegister, const float* pConstantData, UINT Vector4fCount) {
		return oSetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);
	}

	static HRESULT APIENTRY MJSetVertexShaderDX9(LPDIRECT3DDEVICE9 pDevice, IDirect3DVertexShader9* veShader) {
		return oSetVertexShader(pDevice, veShader);
	}

	static HRESULT APIENTRY MJSetPixelShaderDX9(LPDIRECT3DDEVICE9 pDevice, IDirect3DPixelShader9* piShader) {
		return oSetPixelShader(pDevice, piShader);
	}

	static HRESULT APIENTRY MJPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
		if (!ImGui_Initialised) {
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&DirectX11Interface::Device))) {
				ImGui::CreateContext();

				ImGuiIO& io = ImGui::GetIO(); (void)io;
				ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantTextInput || ImGui::GetIO().WantCaptureKeyboard;
				io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

				DirectX11Interface::Device->GetImmediateContext(&DirectX11Interface::DeviceContext);

				DXGI_SWAP_CHAIN_DESC Desc;
				pSwapChain->GetDesc(&Desc);
				WindowHwnd = Desc.OutputWindow;

				ID3D11Texture2D* BackBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&BackBuffer);
				DirectX11Interface::Device->CreateRenderTargetView(BackBuffer, NULL, &DirectX11Interface::RenderTargetView);
				BackBuffer->Release();

				ImGui_ImplWin32_Init(WindowHwnd);
				ImGui_ImplDX11_Init(DirectX11Interface::Device, DirectX11Interface::DeviceContext);
				ImGui_ImplDX11_CreateDeviceObjects();
				ImGui::GetIO().ImeWindowHandle = Process::Hwnd;
				Process::WndProc = (WNDPROC)SetWindowLongPtr(Process::Hwnd, GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProc);
				ImGui_Initialised = true;
			}
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::GetIO().MouseDrawCursor = true;
		ImGui::ShowDemoWindow();

		DrawQueuedElements();

		ImGui::EndFrame();
		ImGui::Render();
		DirectX11Interface::DeviceContext->OMSetRenderTargets(1, &DirectX11Interface::RenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		return oIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
	}

	static DWORD WINAPI MainThread(LPVOID lpParameter) {
		bool WindowFocus = false;
		while (WindowFocus == false) {
			DWORD ForegroundWindowProcessID;
			GetWindowThreadProcessId(GetForegroundWindow(), &ForegroundWindowProcessID);
			if (GetCurrentProcessId() == ForegroundWindowProcessID) {

				Process::ID = GetCurrentProcessId();
				Process::Handle = GetCurrentProcess();
				Process::Hwnd = GetForegroundWindow();

				RECT TempRect;
				GetWindowRect(Process::Hwnd, &TempRect);
				Process::WindowWidth = TempRect.right - TempRect.left;
				Process::WindowHeight = TempRect.bottom - TempRect.top;

				char TempTitle[MAX_PATH];
				GetWindowTextA(Process::Hwnd, TempTitle, sizeof(TempTitle));
				Process::Title = TempTitle;

				char TempClassName[MAX_PATH];
				GetClassNameA(Process::Hwnd, TempClassName, sizeof(TempClassName));
				Process::ClassName = TempClassName;

				char TempPath[MAX_PATH];
				GetModuleFileNameExA(Process::Handle, NULL, TempPath, sizeof(TempPath));
				Process::Path = TempPath;

				WindowFocus = true;
			}
		}
		bool InitHook = false;
		while (InitHook == false) {
			if (DXVersion == DirectXVersion.D3D11) {
				if (DirectX11::Init() == true) {
					RenderHook::CreateHook(8, (void**)&oIDXGISwapChainPresent, MJPresent);
					//RenderHook::CreateHook(12, (void**)&oID3D11DrawIndexed, MJDrawIndexed);
					InitHook = true;
				}
			}
			else if (DXVersion == DirectXVersion.D3D9) {
				if (DirectX9::Init() == true) {
					RenderHook::CreateHook(42, (void**)&oEndScene, MJEndSceneDX9);
					RenderHook::CreateHook(17, (void**)&oPresent, MJPresentDX9);
					RenderHook::CreateHook(82, (void**)&oDrawIndexedPrimitive, MJDrawIndexedPrimitiveDX9);
					RenderHook::CreateHook(81, (void**)&oDrawPrimitive, MJDrawPrimitiveDX9);
					RenderHook::CreateHook(65, (void**)&oSetTexture, MJSetTextureDX9);
					RenderHook::CreateHook(16, (void**)&oReset, MJResetDX9);
					RenderHook::CreateHook(100, (void**)&oSetStreamSource, MJSetStreamSourceDX9);
					RenderHook::CreateHook(87, (void**)&oSetVertexDeclaration, MJSetVertexDeclarationDX9);
					RenderHook::CreateHook(94, (void**)&oSetVertexShaderConstantF, MJSetVertexShaderConstantFDX9);
					RenderHook::CreateHook(92, (void**)&oSetVertexShader, MJSetVertexShaderDX9);
					RenderHook::CreateHook(107, (void**)&oSetPixelShader, MJSetPixelShaderDX9);
					InitHook = true;
				}
			}
		}
		return 0;
	}

	static void InitRender() {
		DXVersion = RenderHook::GetDirectXVersion();
		if (DXVersion == DirectXVersion.D3D11) {
			printf("hooking to dx11...\n");
		}
		else if (DXVersion == DirectXVersion.D3D9) {
			printf("hooking to dx9...\n");
		}
		else {
			MessageBoxA(zWindow::GetGothicWindow(), "error", "directx version not supported, install dx11 or gothic-legacyaltrenderer", MB_OK);
		}

		initializedRender = true;
		Process::Module = GetThisDllHandle();
		CreateThread(0, 0, MainThread, 0, 0, 0);
	}

	class zRender {
	public:
		static void Update() {
			if (!initializedRender) return InitRender();
		}

		static void DrawRectangle(float x, float y, float w, float h, ImColor color = ImColor(255, 255, 255)) {
			for (int i = 0; i < MAX_DRAW_QUEUE; i++) {
				if (!rectangleQueue[i].inUse) {
					rectangleQueue[i].x = x;
					rectangleQueue[i].y = y;
					rectangleQueue[i].w = w;
					rectangleQueue[i].h = h;
					rectangleQueue[i].color = color;
					rectangleQueue[i].inUse = true;
					break;
				}
			}
		}
	};
}