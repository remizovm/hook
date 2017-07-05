#pragma once

#include "stdafx.h"

class D3D11
{
public:
	D3D11(HWND hWnd);
	//HRESULT hr;
	void Render();
	void CleanupDevice();
	~D3D11();

private:
	ID3D11Device  *g_pd3dDevice;
	ID3D11DeviceContext    *g_pImmediateContext;
	ID3D11RenderTargetView *g_pRenderTargetView;
	IDXGISwapChain* g_pSwapChain;
	D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	D3D11_VIEWPORT vp;
};

