#include "zRender.h"

#define MAX_DRAW_QUEUE 1024

bool ShowMenu = false;
bool ImGui_Initialised = false;
bool initializedRender = false;

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

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ShowMenu) {
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
		return true;
	}
	return CallWindowProc(Process::WndProc, hwnd, uMsg, wParam, lParam);
}

namespace GOTHIC_ENGINE {
	// queue
	struct Rectangle {
		float x;
		float y;
		float w;
		float h;
		ImColor color;
		bool inUse;
	};

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
		if (GetAsyncKeyState(VK_INSERT) & 1) ShowMenu = !ShowMenu;
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
			if (DirectX11::Init() == true) {
				Global::CreateHook(8, (void**)&oIDXGISwapChainPresent, MJPresent);
				Global::CreateHook(12, (void**)&oID3D11DrawIndexed, MJDrawIndexed);
				InitHook = true;
			}
		}
		return 0;
	}

	class zRender {
	public:
		static void Update() {
			if (initializedRender) return;
			HMODULE hModule = GetThisDllHandle();
			if (Global::ChecktDirectXVersion(DirectXVersion.D3D9) == true) {
				initializedRender = true;
				Process::Module = hModule;
				printf("starting hooking to dx11\n");
				CreateThread(0, 0, MainThread, 0, 0, 0);
			}
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