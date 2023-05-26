// Automatically generated block
#pragma region Includes
#define D3DCOLORVALUE_DEFINED

// Lua
extern "C" {
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

// Default
#include <cassert>
#include <vector>
#include <functional>
#include <filesystem>
#include <sstream>
#include <algorithm>

// DirectX
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

#include <dxgi.h>
#include <d3d9.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

// ImGui
#include <Directories/ImGui/imgui.h>
#include <Directories/ImGui/imgui_impl_win32.h>
#include <Directories/ImGui/imgui_impl_dx9.h>
#include <Directories/ImGui/imgui_impl_dx11.h>
#include "Directories/MinHook/Include/MinHook.h"

// GLua files
#include "zUtils.h"
#include "zFileSystem.h"
#include "zEvents.h"
#include "zKeyboard.h"
#include "zWindow.h"
#include "RenderHook.h"
#include "zRender.h"
#include "zColor.h"
#include "zCursor.h"
#include "zLuaConsole.h"
#include "zLuaEvents.h"
#include "zLuaCursor.h"
#include "zLuaRender.h"
#include "zLuaColor.h"
#include "zLuaArgReader.h"
#include "zLua.h"
#include "zResource.h"
#include "zResourceManager.h"

// Default
#include "Plugin.h"
#pragma endregion