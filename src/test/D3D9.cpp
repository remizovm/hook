#include "stdafx.h"
#include "D3D9.h"

D3D9::D3D9(HWND hWnd)
{
	d3d = unique_ptr<IDirect3D9>(Direct3DCreate9(D3D_SDK_VERSION));
	/*
	if ( d3d9 == nullptr ) {
		// TODO: handle error
	}
	*/
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	auto hres = d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
	/*
	if ( hres != D3D_OK ) {
		// TODO: handle error
	}
	*/
}

void D3D9::Render()
{
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	d3ddev->BeginScene();
	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

D3D9::~D3D9()
{
	if (d3ddev != nullptr) d3ddev->Release();
	if (d3d.get() != nullptr) d3d.get()->Release();
}
