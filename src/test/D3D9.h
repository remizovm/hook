#pragma once

#include "stdafx.h"

class D3D9
{
public:
	D3D9(HWND hWnd);

	void Render();

	~D3D9();

private:
	unique_ptr<IDirect3D9> d3d;
	LPDIRECT3DDEVICE9 d3ddev;
};

