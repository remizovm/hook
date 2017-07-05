#include <vector>
#include <d3d9.h>
#include <d3d11.h>
#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3dx9core.h"
#include "detours.h"
#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")
#pragma comment(lib, "d3d11")
using namespace std;

typedef HRESULT(WINAPI* pDrawIndexedPrimitive)(LPDIRECT3DDEVICE9, D3DPRIMITIVETYPE, int, UINT, UINT, UINT, UINT);
typedef HRESULT(WINAPI* pEndScene)(LPDIRECT3DDEVICE9);
typedef HRESULT(WINAPI* pReset)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);

pDrawIndexedPrimitive oDrawIndexedPrimitive;
pEndScene oEndScene;
pReset oReset;

HMODULE hD3D11;										//d3d11 vars init
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_HARDWARE;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;		
ID3D11DeviceContext*    g_pImmediateContext = NULL;	
IDXGISwapChain*         g_pSwapChain = NULL;		
ID3D11RenderTargetView* g_pRenderTargetView = NULL;	

HMODULE hD3D9;										//d3d9 vars init
D3DVIEWPORT9 vpt;
LPDIRECT3DTEXTURE9 green = NULL; 
LPDIRECT3DTEXTURE9 pTx = NULL;

LPDIRECT3DTEXTURE9 red = NULL; 
LPDIRECT3DTEXTURE9 pTx1 = NULL;

D3DLOCKED_RECT d3dlr;
D3DLOCKED_RECT d3dlr1;
LPD3DXFONT pFont = NULL;
char strbuff[260];

bool wallHack = false;
bool gotMethods = false;

HRESULT WINAPI hkDrawIndexedPrimitiveD3D9(LPDIRECT3DDEVICE9 pDev, D3DPRIMITIVETYPE PrimType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	if (wallHack) 
	{
		LPDIRECT3DVERTEXBUFFER9 Stream_Data;
		UINT offset = 0;
		UINT stride = 0;
		if (pDev->GetStreamSource(0, &Stream_Data, &offset, &stride) == S_OK)
			Stream_Data->Release();
		if (stride == 32) 
		{
			if (NumVertices == 1946 || 
				NumVertices == 1308 || 
				NumVertices == 785  || 
				NumVertices == 2421 || 
				NumVertices == 3147 || 
				NumVertices == 275) 
			{
				pDev->SetRenderState(D3DRS_ZENABLE, false);
				pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
				pDev->SetTexture(0, red);
				oDrawIndexedPrimitive(pDev, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				pDev->SetRenderState(D3DRS_ZENABLE, true);
				pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
				pDev->SetTexture(0, green);
			}		
		}
	}
	return oDrawIndexedPrimitive(pDev, PrimType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT WINAPI hkEndSceneD3D9(LPDIRECT3DDEVICE9 pDev)
{
	
	pDev->GetViewport(&vpt);
		RECT FRect = { 10, 10, vpt.Width, vpt.Height };
		if (green == 0)
			if (pDev->CreateTexture(8, 8, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &green, 0) == S_OK)
				if (pDev->CreateTexture(8, 8, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pTx, 0) == S_OK)
					if (pTx->LockRect(0, &d3dlr, 0, D3DLOCK_DONOTWAIT | D3DLOCK_NOSYSLOCK) == S_OK) 
					{
						for (UINT xy = 0; xy < 8 * 8; xy++)
							((PDWORD)d3dlr.pBits)[xy] = 0xFF00FF00;
						pTx->UnlockRect(0);
						pDev->UpdateTexture(pTx, green);
						pTx->Release();
					}
		if (red == 0)
			if (pDev->CreateTexture(8, 8, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &red, 0) == S_OK)
				if (pDev->CreateTexture(8, 8, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pTx1, 0) == S_OK)
					if (pTx1->LockRect(0, &d3dlr1, 0, D3DLOCK_DONOTWAIT | D3DLOCK_NOSYSLOCK) == S_OK) 
					{
						for (UINT xy = 0; xy < 8 * 8; xy++)
							((PDWORD)d3dlr1.pBits)[xy] = 0xFFFF0000;
						pTx1->UnlockRect(0);
						pDev->UpdateTexture(pTx1, red);
						pTx1->Release();
					}
		if (pFont == 0)
			D3DXCreateFontA(pDev, 16, 0, 700, 0, 0, 1, 0, 0, DEFAULT_PITCH | FF_DONTCARE, "Terminal", &pFont);
		char* wallHackStatus = wallHack ? "on" : "off";
		sprintf_s(strbuff, "WallHack: %s", wallHackStatus);
		if (pFont)
			pFont->DrawTextA(0, strbuff, -1, &FRect, DT_CENTER | DT_NOCLIP, 0xFF00FF00);
	return oEndScene(pDev);
}

HRESULT WINAPI hkResetD3D9(LPDIRECT3DDEVICE9 pDev,D3DPRESENT_PARAMETERS* PresP)
{
	if (pFont) 
	{ 
		pFont->Release(); 
		pFont = NULL; 
	}
	if (green) 
	{ 
		green->Release(); 
		green = NULL; 
	}
	if (red)
	{
		red->Release();
		red = NULL;
	}
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
	HWND hWnd = CreateWindowExA(0, "STATIC", "dummy", 0, 0, 0, 
								0, 0, 0, 0, 0, 0);
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
	if (oDrawIndexedPrimitive && oEndScene && oReset)
		gotMethods = true;
}
void GetDevice11Methods()
{
	HRESULT hr = S_OK;
	DXGI_SWAP_CHAIN_DESC sd;            
	UINT createDeviceFlags = 0;
	DWORD dip11 = 0;
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_0,};
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
	Beep(500, 1000);
	while (1) 
	{		
		if (!gotMethods)
		{

			if (hD3D9 != 0 && hD3D11 == 0) //use DX9
				GetDevice9Methods();
			else if (hD3D11 != 0) //use DX11
				GetDevice11Methods();
		}
		if (GetAsyncKeyState(VK_F1) & 1)
			wallHack = !wallHack;
		Sleep(10);
	}
}

int WINAPI DllMain(HINSTANCE hInst, DWORD ul_reason_for_call, void* lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)TF, 0, 0, 0);
	return 1;
}
