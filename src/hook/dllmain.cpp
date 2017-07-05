// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

void TF();
void GetDevice9Methods();
void GetDevice11Methods();

typedef HRESULT(WINAPI* pDrawIndexedPrimitive)(LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, int, UINT, UINT, UINT, UINT);
typedef HRESULT(WINAPI* pEndScene)(LPDIRECT3DDEVICE9);
typedef HRESULT(WINAPI* pReset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);

pDrawIndexedPrimitive oDrawIndexedPrimitive;
pEndScene oEndScene;
pReset oReset;

auto is_got_methods = false;
HMODULE hD3D9 = NULL;
HMODULE hD3D11;										//d3d11 vars init
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_HARDWARE;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;
ID3D11DeviceContext*    g_pImmediateContext = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;

HRESULT WINAPI hkDrawIndexedPrimitiveD3D9(LPDIRECT3DDEVICE9 pDev, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	return oDrawIndexedPrimitive(pDev, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT WINAPI hkEndSceneD3D9(LPDIRECT3DDEVICE9 pDev)
{
	return oEndScene(pDev);
}

HRESULT WINAPI hkResetD3D9(LPDIRECT3DDEVICE9 pDev, D3DPRESENT_PARAMETERS* PresP)
{
	return oReset(pDev, PresP);
}

void GetDevice9Methods()
{
	DWORD dip9 = 0;
	DWORD endScene9 = 0;
	DWORD reset9 = 0;
	IDirect3D9 *d3d9_ptr;
	IDirect3DDevice9* d3dDevice;
	DWORD* vtablePtr;
	D3DPRESENT_PARAMETERS d3dpp;
	HWND hWnd = CreateWindowExA(0, "STATIC", "dummy", 0, 0, 0, 0, 0, 0, 0, 0, 0);
	hD3D9 = GetModuleHandleA("d3d9.dll");
	d3d9_ptr = Direct3DCreate9(D3D_SDK_VERSION);
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3d9_ptr->CreateDevice(0, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3dDevice);
	vtablePtr = (PDWORD)(*((PDWORD)d3dDevice));
	dip9 = vtablePtr[82] - (DWORD)hD3D9;
	endScene9 = vtablePtr[42] - (DWORD)hD3D9;
	reset9 = vtablePtr[16] - (DWORD)hD3D9;
	d3dDevice->Release();
	d3d9_ptr->Release();
	CloseHandle(hWnd);
	oDrawIndexedPrimitive = (pDrawIndexedPrimitive)DetourFunction((PBYTE)((DWORD)hD3D9 + dip9), (PBYTE)hkDrawIndexedPrimitiveD3D9);
	oEndScene = (pEndScene)DetourFunction((PBYTE)((DWORD)hD3D9 + endScene9), (PBYTE)hkEndSceneD3D9);
	oReset = (pReset)DetourFunction((PBYTE)((DWORD)hD3D9 + reset9), (PBYTE)hkResetD3D9);
	if (oDrawIndexedPrimitive && oEndScene && oReset) {
		is_got_methods = true;
	}
}

void GetDevice11Methods()
{
	HRESULT hr = S_OK;
	DXGI_SWAP_CHAIN_DESC sd;
	UINT createDeviceFlags = 0;
	DWORD dip11 = 0;
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_0, };
	UINT numFeatureLevels = ARRAYSIZE(featureLevel);
	DWORD endScene11 = 0;
	DWORD reset11 = 0;
	DWORD* vtablePtr;
	D3DPRESENT_PARAMETERS d3dpp;
	HWND hWnd = CreateWindowExA(0, "STATIC", "dummy", 0, 0, 0,
		0, 0, 0, 0, 0, 0);
	hD3D11 = GetModuleHandleA("d3d11.dll");
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	//sd.BufferDesc.Width = width;                     //TODO: delete this if probably
	//sd.BufferDesc.Height = height;                          
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 75;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevel, numFeatureLevels, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
	vtablePtr = (PDWORD)(*((PDWORD)g_pd3dDevice));			//TODO: fix vtablePtr
															//dip11 = vtablePtr[82] - (DWORD)hD3D9;
															//endScene11 = vtablePtr[42] - (DWORD)hD3D9;
															//reset11 = vtablePtr[16] - (DWORD)hD3D9;
	g_pd3dDevice->Release();
	CloseHandle(hWnd);
	//oDrawIndexedPrimitive = (pDrawIndexedPrimitive)DetourFunction((PBYTE)((DWORD)hD3D9 + dip11), (PBYTE)hkDrawIndexedPrimitiveD3D9);
	//oEndScene = (pEndScene)DetourFunction((PBYTE)((DWORD)hD3D9 + endScene11), (PBYTE)hkEndSceneD3D9);
	//oReset = (pReset)DetourFunction((PBYTE)((DWORD)hD3D9 + reset11), (PBYTE)hkResetD3D9);
	//if (oDrawIndexedPrimitive && oEndScene && oReset)
	//	gotMethods = true;

}

void TF()
{
	while (true) {
		if ( !is_got_methods ) {
			if ( hD3D9 != 0 && hD3D11 == 0 ) { // use DX9
				GetDevice9Methods();
			} else if ( hD3D11 != 0 ) { // use DX11
				GetDevice11Methods();
			}
		}
		Sleep(100);
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)TF, 0, 0, 0);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

