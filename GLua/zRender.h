typedef HRESULT(APIENTRY* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
IDXGISwapChainPresent oIDXGISwapChainPresent;

typedef void(APIENTRY* ID3D11DrawIndexed)(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
ID3D11DrawIndexed oID3D11DrawIndexed;

typedef HRESULT(APIENTRY* EndScene) (IDirect3DDevice9*);
EndScene oEndScene = NULL;

typedef HRESULT(APIENTRY* Present) (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
Present oPresent = NULL;

typedef HRESULT(APIENTRY* DrawIndexedPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, INT, UINT, UINT, UINT, UINT);
DrawIndexedPrimitive oDrawIndexedPrimitive = NULL;

typedef HRESULT(APIENTRY* DrawPrimitive)(IDirect3DDevice9*, D3DPRIMITIVETYPE, UINT, UINT);
DrawPrimitive oDrawPrimitive = NULL;

typedef HRESULT(APIENTRY* SetTexture)(IDirect3DDevice9*, DWORD, IDirect3DBaseTexture9*);
SetTexture oSetTexture = NULL;

typedef HRESULT(APIENTRY* Reset)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
Reset oReset = NULL;

typedef HRESULT(APIENTRY* SetStreamSource)(IDirect3DDevice9*, UINT, IDirect3DVertexBuffer9*, UINT, UINT);
SetStreamSource oSetStreamSource = NULL;

typedef HRESULT(APIENTRY* SetVertexDeclaration)(IDirect3DDevice9*, IDirect3DVertexDeclaration9*);
SetVertexDeclaration oSetVertexDeclaration = NULL;

typedef HRESULT(APIENTRY* SetVertexShaderConstantF)(IDirect3DDevice9*, UINT, const float*, UINT);
SetVertexShaderConstantF oSetVertexShaderConstantF = NULL;

typedef HRESULT(APIENTRY* SetVertexShader)(IDirect3DDevice9*, IDirect3DVertexShader9*);
SetVertexShader oSetVertexShader = NULL;

typedef HRESULT(APIENTRY* SetPixelShader)(IDirect3DDevice9*, IDirect3DPixelShader9*);
SetPixelShader oSetPixelShader = NULL;

#define MAX_DRAW_QUEUE 1024

namespace GOTHIC_ENGINE {
    extern bool ShowMenu;
    extern bool ImGui_Initialised;
    extern bool initializedRender;

    namespace Process {
        extern DWORD ID;
        extern HANDLE Handle;
        extern HWND Hwnd;
        extern HMODULE Module;
        extern WNDPROC WndProc;
        extern int WindowWidth;
        extern int WindowHeight;
        extern LPCSTR Title;
        extern LPCSTR ClassName;
        extern LPCSTR Path;
    }

    namespace DirectX11Interface {
        extern ID3D11Device* Device;
        extern ID3D11DeviceContext* DeviceContext;
        extern ID3D11RenderTargetView* RenderTargetView;
    }

    int DXVersion = DirectXVersion.Unknown;

    struct Rectangle {
        float x;
        float y;
        float w;
        float h;
        ImColor color;
        bool inUse;
    };

    struct Text {
        char* text;
        float x;
        float y;
        float w;
        float h;
        ImColor color;
    };

    LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void DrawQueuedElements();
    HMODULE GetThisDllHandle();

    class zRender {
    private:
        static HRESULT APIENTRY MJEndSceneDX9(IDirect3DDevice9* pDevice);
        static HRESULT APIENTRY MJEndScene(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
        static DWORD WINAPI MainThread(LPVOID lpParameter);
        static void InitRender();
    public:
        static bool Initialized();
        static void Update();
        static void DrawRectangle(float x, float y, float w, float h, ImColor color = ImColor(255, 255, 255));
        static void DrawTextElement(char* string, float x, float y, float w, float h, ImColor color = ImColor(255, 255, 255));
    };

}