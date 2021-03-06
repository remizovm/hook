#pragma once

#include "stdafx.h"
#include "D3D.h"

class D3D9 : public D3D
{
public:
	D3D9(HWND hWnd);
	HRESULT Init();
	void Render();
	void CleanupDevice();
	~D3D9();

private:
	HWND g_hWnd = NULL;
	IDirect3D9* d3d = NULL;
	LPDIRECT3DDEVICE9 d3ddev = NULL;
};

