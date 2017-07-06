#pragma once

#include "stdafx.h"

class D3D
{
public:
	virtual HRESULT Init()=0;
	virtual void Render()=0;
	virtual void CleanupDevice()=0;
	virtual ~D3D();
};

