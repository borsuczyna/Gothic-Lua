#include <d3d11.h>
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
#define DIRECT3D_VERSION         0x0700

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