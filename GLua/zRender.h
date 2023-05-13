#pragma once

#include <d3d11.h>
// Supported with union (c) 2020 Union team
// Union HEADER file

typedef HRESULT(APIENTRY* IDXGISwapChainPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
IDXGISwapChainPresent oIDXGISwapChainPresent;

typedef void(APIENTRY* ID3D11DrawIndexed)(ID3D11DeviceContext* pContext, UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
ID3D11DrawIndexed oID3D11DrawIndexed;

namespace GOTHIC_ENGINE {
	// Add your code here . . .
}