#pragma once

#include <memory>

#include "Lucky.h"


class D3D12Wrapper
{
public:
	D3D12Wrapper();
	~D3D12Wrapper();

public:
	bool InitializeCommon();
	bool InitializeDirect3D(HWND, int, int);
	void Update();
	void Render();
	void Destroy();
private:
	std::unique_ptr<Lucky::D3D12Adapter> m_Adapter;
};

