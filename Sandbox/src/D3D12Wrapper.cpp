#include "D3D12Wrapper.h"

D3D12Wrapper::D3D12Wrapper()
{
	//
	m_Adapter = std::make_unique<Lucky::D3D12Adapter>();
}

D3D12Wrapper::~D3D12Wrapper()
{
	if (nullptr != m_Adapter)
	{
		m_Adapter.release();
	}
}

bool D3D12Wrapper::InitializeCommon()
{
	Lucky::LuckyLog::Init();

	return true;
}

bool D3D12Wrapper::InitializeDirect3D(HWND hWnd, int nWidth, int nHeight)
{
	DX3D12_ADAPTER_INFO info = {};
	info.MainWnd	= hWnd;
	info.Width		= nWidth;
	info.Height		= nHeight;
	m_Adapter->PreInitialize(&info);

	m_Adapter->Initialize();

	return true;
}

void D3D12Wrapper::Update()
{
	m_Adapter->OnUpdate();
}

void D3D12Wrapper::Render()
{
	m_Adapter->OnRender();
}

void D3D12Wrapper::Destroy()
{
	m_Adapter->OnDestroy();
}
