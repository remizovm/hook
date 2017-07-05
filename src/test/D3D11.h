#pragma once

#include "stdafx.h"

class D3D11
{
public:
	D3D11(HWND hWnd);
	HRESULT Init();
	void Render();
	void CleanupDevice();
	~D3D11();

private:
	HWND g_hWnd = NULL;
	ID3D11Device  *g_pd3dDevice = NULL;
	ID3D11DeviceContext    *g_pImmediateContext = NULL;
	ID3D11RenderTargetView *g_pRenderTargetView = NULL;
	IDXGISwapChain* g_pSwapChain = NULL;
	D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D11_VIEWPORT vp;
};

