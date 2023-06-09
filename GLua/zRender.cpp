extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace GOTHIC_ENGINE {
    bool ShowMenu = false;
    bool ImGui_Initialised = false;
    bool initializedRender = false;

    std::vector<Rectangle>& Rectangles() {
        static std::vector<Rectangle> rectangles;
        return rectangles;
    }

    std::vector<Text>& Texts() {
        static std::vector<Text> texts;
        return texts;
    }

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

    LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[0] = zinput->GetMouseButtonPressedLeft();
        return CallWindowProc(Process::WndProc, hwnd, uMsg, wParam, lParam);
    }

    void DrawQueuedElements() {
        // rectangles
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        for (Rectangle rectangle : Rectangles()) {
            ImVec2 start(rectangle.x, rectangle.y);
            ImVec2 end(rectangle.x + rectangle.w, rectangle.y + rectangle.h);

            drawList->AddRectFilled(start, end, rectangle.color);
        }
        Rectangles().clear();

        // texts
        for (Text text : Texts()) {
            ImGui::SetNextWindowPos(ImVec2(text.x, text.y), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::Begin("TextWindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs);

            ImGui::Text(text.text);

            ImGui::End();
            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor();
        }
        Texts().clear();
    }

    HMODULE GetThisDllHandle() {
        MEMORY_BASIC_INFORMATION info;
        size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)GetThisDllHandle, &info, sizeof(info));
        return len ? (HMODULE)info.AllocationBase : NULL;
    }

    bool zRender::Initialized() {
        return ImGui_Initialised;
    }

    void zRender::Update() {
        if (!initializedRender) return InitRender();
    }

    void zRender::DrawRectangle(float x, float y, float w, float h, ImColor color) {
        Rectangle rectangle;
        rectangle.x = x;
        rectangle.y = y;
        rectangle.w = w;
        rectangle.h = h;
        rectangle.color = color;

        Rectangles().push_back(rectangle);
    }

    void zRender::DrawTextElement(char* buffer, float x, float y, float w, float h, ImColor color, int xAlign, int yAlign) {
        ImVec2 textSize = ImGui::CalcTextSize(buffer);
        
        Text text;
        text.text = buffer;

        switch (xAlign) {
        case 0: // left
            text.x = x;
            break;
        case 1: // center
            text.x = (x + w / 2) - textSize.x / 2;
            break;
        case 2: // right
            text.x = (x + w) - textSize.x;
            break;
        }

        switch (yAlign) {
        case 0: // top
            text.y = y;
            break;
        case 1: // center
            text.y = (y + h / 2) - textSize.y / 2;
            break;
        case 2: // bottom
            text.y = (y + h) - textSize.y;
            break;
        }

        text.w = textSize.x;
        text.h = textSize.y;
        text.color = color;

        Texts().push_back(text);
    }

    // DirectX9 ImGui
    HRESULT APIENTRY zRender::MJEndSceneDX9(IDirect3DDevice9* pDevice) {
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

        zLuaEvents::LuaEventArguments arguments;

        zEvents::TriggerEvent("onImGuiRender", NULL);
        zLuaEvents::TriggerEvent("onImGuiRender", arguments);
        zEvents::TriggerEvent("onClientRender", NULL);
        zLuaEvents::TriggerEvent("onClientRender", arguments);

        DrawQueuedElements();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

        return oEndScene(pDevice);
    }

    // DirectX11 ImGui
    HRESULT APIENTRY zRender::MJEndScene(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
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

        zLuaEvents::LuaEventArguments arguments;

        zEvents::TriggerEvent("onImGuiRender", NULL);
        zLuaEvents::TriggerEvent("onImGuiRender", arguments);
        zEvents::TriggerEvent("onClientRender", NULL);
        zLuaEvents::TriggerEvent("onClientRender", arguments);

        DrawQueuedElements();

        int width, height;
        zWindow::GetResolution(width, height);
        char* buffer = new char[256];
        sprintf_s(buffer, 256, "Gothic : GLua %s build %s %s", CURRENT_VERSION, __DATE__, __TIME__);
        ImVec2 textSize = ImGui::CalcTextSize(buffer);

        zRender::DrawTextElement(buffer, 0, 0, (float)width, (float)height, ImColor(255, 255, 255), 2, 2);

        ImGui::EndFrame();
        ImGui::Render();
        DirectX11Interface::DeviceContext->OMSetRenderTargets(1, &DirectX11Interface::RenderTargetView, NULL);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        return oIDXGISwapChainPresent(pSwapChain, SyncInterval, Flags);
    }

    DWORD WINAPI zRender::MainThread(LPVOID lpParameter) {
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
                    RenderHook::CreateHook(8, (void**)&oIDXGISwapChainPresent, MJEndScene);
                    InitHook = true;
                }
            }
            else if (DXVersion == DirectXVersion.D3D9) {
                if (DirectX9::Init() == true) {
                    RenderHook::CreateHook(42, (void**)&oEndScene, MJEndSceneDX9);
                    InitHook = true;
                }
            }
        }

        return 0;
    }

    void zRender::InitRender() {
        DXVersion = RenderHook::GetDirectXVersion();
        if (DXVersion == DirectXVersion.D3D11) {
            zConsole::Log("Hooking to DX11 render...", LogType.Default);
        }
        else if (DXVersion == DirectXVersion.D3D9) {
            zConsole::Log("Hooking to DX9 render...", LogType.Default);
        }
        else {
            MessageBoxA(zWindow::GetGothicWindow(), "error", "directx version not supported, install dx11 or gothic-legacyaltrenderer", MB_OK);
        }

        initializedRender = true;
        Process::Module = GetThisDllHandle();
        CreateThread(0, 0, MainThread, 0, 0, 0);
    }
}