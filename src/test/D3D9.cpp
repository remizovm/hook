#include "stdafx.h"
#include "D3D9.h"

D3D9::D3D9(HWND hWnd)
{
	g_hWnd = hWnd;
}
HRESULT D3D9::Init()
{
	HRESULT hres = S_OK;
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWnd;
	D3DDEVTYPE devTypes[] ={D3DDEVTYPE_HAL,D3DDEVTYPE_REF, D3DDEVTYPE_SW,};
	UINT numDevTypes = ARRAYSIZE(devTypes);
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDevTypes; driverTypeIndex++)
	{
		D3DDEVTYPE g_driverType = devTypes[driverTypeIndex];
		hres = d3d->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			g_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&d3ddev);
		if (SUCCEEDED(hres)) break;
	}
	if (FAILED(hres)) return hres;
	/*
	if ( hres != D3D_OK ) {
	// TODO: handle error
	}
	*/
	return hres;
}

void D3D9::Render()
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	d3ddev->BeginScene();
	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}
void D3D9::CleanupDevice()
{
	if (d3ddev) d3ddev->Release();
	if (d3d) d3d->Release();
}
D3D9::~D3D9()
{
	CleanupDevice();
}
