#include "D3D12Adapter.h"

#include "HrException.h"

NAMESPACE_LUCKY_START

D3D12Adapter::D3D12Adapter() : m_CurrentFenceValue(0), m_FenceEvent(nullptr), m_PreInitialized(false)
{
}

void D3D12Adapter::PreInitialize(const DX3D12_ADAPTER_INFO* pInfo)
{
	if (nullptr == pInfo)
	{
		LK_CORE_ERROR("参数pInfo为空!!");
		return;
	}

	m_MainWnd		= pInfo->MainWnd;
	m_ClientWidth	= pInfo->Width;
	m_ClientHeight	= pInfo->Height;

	m_PreInitialized = true;
}

void D3D12Adapter::Initialize()
{
	if (!m_PreInitialized)
	{
		LK_CORE_ERROR("先调用PreIntialize()");
		return;
	}

	enableDebugLayer();
	createDevice();
	createFence();
	checkMSAA();
	createCommandObjects();
	createSwapChain();
	createRtvAndDsvDescriptorHeaps();
	createRenderTargetView();
	createDepthStencilView();
	setViewport();
	setScissorRect();

	LK_CORE_INFO("Initialized Adapter");
}

void D3D12Adapter::PostInitialize()
{
	// 初始化D3D12 API之后调用
	if (!m_PreInitialized)
	{
		LK_CORE_ERROR("先调用PreIntialize()");
		return;
	}
	
	// 重置命令列表
	ThrowIfFailed(
		m_CommandList->Reset(m_DirectCmdListAlloc.Get(), nullptr)
	);

	buildDescriptorHeaps();
	buildConstantBuffers();
	buildRootSignature();
	buildShadersAndInputLayout();
	buildGeometry();
	buildPSO();

	ThrowIfFailed(
		m_CommandList->Close()
	);

	ID3D12CommandList* cmdLists[] = { m_CommandList.Get() };
	m_CommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	flushCommandQueue();
}

void D3D12Adapter::OnUpdate()
{
	if (!m_PreInitialized)
	{
		return;
	}

}

void D3D12Adapter::OnRender()
{
	if (!m_PreInitialized)
	{
		return;
	}

}

void D3D12Adapter::OnDestroy()
{
}

void D3D12Adapter::flushCommandQueue()
{
	m_CurrentFenceValue++;

	ThrowIfFailed(
		m_CommandQueue->Signal(m_Fence.Get(), m_CurrentFenceValue)
	);

	if (m_Fence->GetCompletedValue() < m_CurrentFenceValue)
	{
		HANDLE eventHandle = CreateEvent(nullptr, FALSE, FALSE, L"FenceResetDone");
		ThrowIfFailed(
			m_Fence->SetEventOnCompletion(m_CurrentFenceValue, eventHandle)
		);

		// 等待GPU命中围栏，激发事件
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}

void D3D12Adapter::enableDebugLayer()
{
#if defined(DEBUG) || defined(_DEBUG)
	{
		ComPtr<ID3D12Debug> debugController;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();
	}
#endif
}

void D3D12Adapter::createDevice()
{
	// 创建factory
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_DxgiFactory)));

	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_D3DDevice));
	if (FAILED(hr))
	{
		ComPtr<IDXGIAdapter1> pWarpAdapter;
		ThrowIfFailed(m_DxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(pWarpAdapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_D3DDevice)));
	}
}

void D3D12Adapter::createFence()
{
	ThrowIfFailed(
		m_D3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))
	);

	// TODO: 是否应该在这就创建事件？
	//m_CurrentFenceValue = 1;
	//m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	//if (nullptr == m_FenceEvent)
	//{
	//	ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	//}

	m_RtvDescriptorSize		= m_D3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_DsvDescriptorSize		= m_D3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_CbvUavDescriptorSize	= m_D3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void D3D12Adapter::checkMSAA()
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format				= m_BackBufferFormat;
	msQualityLevels.SampleCount			= 4;
	msQualityLevels.Flags				= D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels	= 0;
	
	ThrowIfFailed(
		m_D3DDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels))
	);

	m_4xMsaaQuality = msQualityLevels.NumQualityLevels;
}

void D3D12Adapter::createCommandObjects()
{
	// 创建命令队列
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type	= D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(
		m_D3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue))
	);

	// 命令列表分配器
	ThrowIfFailed(
		m_D3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_DirectCmdListAlloc.GetAddressOf()))
	);

	// 创建命令列表
	ThrowIfFailed(
		// 会关联上命令分配器
		m_D3DDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_DirectCmdListAlloc.Get(), nullptr, IID_PPV_ARGS(m_CommandList.GetAddressOf()))
	);

	m_CommandList->Close();
}

void D3D12Adapter::createSwapChain()
{
	// 先销毁旧的交换链
	m_SwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC scDesc = {};
	scDesc.BufferDesc.Width						= m_ClientWidth;
	scDesc.BufferDesc.Height					= m_ClientHeight;
	scDesc.BufferDesc.RefreshRate.Numerator		= 60;
	scDesc.BufferDesc.RefreshRate.Denominator	= 1;
	scDesc.BufferDesc.Format					= m_BackBufferFormat;
	scDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;

	scDesc.SampleDesc.Count						= m_4xMsaaState ? 4 : 1;
	scDesc.SampleDesc.Quality					= m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;

	scDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount							= SwapChainBufferCount;
	scDesc.OutputWindow							= m_MainWnd;
	scDesc.Windowed								= true;
	scDesc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ThrowIfFailed(
		m_DxgiFactory->CreateSwapChain(m_CommandQueue.Get(), &scDesc, m_SwapChain.GetAddressOf())
	);
}

void D3D12Adapter::createRtvAndDsvDescriptorHeaps()
{
	// 创建rtv描述符堆
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors	= SwapChainBufferCount;	// 与交换链缓冲区数相同
	rtvHeapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask		= 0;
	ThrowIfFailed(
		m_D3DDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(m_RtvHeap.GetAddressOf()))
	);

	// 创建dsv描述符堆 --- depth/stencil
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors	= 1;
	dsvHeapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask		= 0;
	ThrowIfFailed(
		m_D3DDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(m_DsvHeap.GetAddressOf()))
	);
}

void D3D12Adapter::createRenderTargetView()
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(
		m_RtvHeap->GetCPUDescriptorHandleForHeapStart()
	);

	// 为交换链的每个缓冲区创建缓冲区视图（RTV）
	for (UINT i = 0; i < SwapChainBufferCount; ++i)
	{
		// 获得交换链内的第i个缓冲区
		ThrowIfFailed(
			m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_SwapChainBuffer[i]))
		);

		// 为此缓冲区创建一个RTV
		m_D3DDevice->CreateRenderTargetView(m_SwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);

		// 偏移到描述符堆中的下一个缓冲区
		rtvHeapHandle.Offset(1, m_RtvDescriptorSize);
	}
}

void D3D12Adapter::createDepthStencilView()
{
	// 创建深度/模板缓冲区及其视图
	D3D12_RESOURCE_DESC depthStencilDesc = {};
	depthStencilDesc.Dimension			= D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment			= 0;
	depthStencilDesc.Width				= m_ClientWidth;
	depthStencilDesc.Height				= m_ClientHeight;
	depthStencilDesc.DepthOrArraySize	= 1;
	depthStencilDesc.MipLevels			= 1;
	depthStencilDesc.Format				= m_DepthStencilFormat;
	depthStencilDesc.SampleDesc.Count	= m_4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout				= D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags				= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear = {};
	optClear.Format						= m_DepthStencilFormat;
	optClear.DepthStencil.Depth			= 1.0f;
	optClear.DepthStencil.Stencil		= 0;

	CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	ThrowIfFailed(
		m_D3DDevice->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&depthStencilDesc,
			D3D12_RESOURCE_STATE_COMMON,	// 初始状态
			&optClear,
			IID_PPV_ARGS(m_DepthStencilBuffer.GetAddressOf())
		)
	);

	// 利用此资源格式，为整个资源的第0 mip层创建描述符
	m_D3DDevice->CreateDepthStencilView(m_DepthStencilBuffer.Get(), nullptr, m_DsvHeap->GetCPUDescriptorHandleForHeapStart());

	// 转换资源初始状态为深度缓冲区
	D3D12_RESOURCE_BARRIER rb = CD3DX12_RESOURCE_BARRIER::Transition(m_DepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	m_CommandList->ResourceBarrier(1, &rb);
}

void D3D12Adapter::setViewport()
{
	D3D12_VIEWPORT vp = {};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width	= static_cast<float>(m_ClientWidth);
	vp.Height	= static_cast<float>(m_ClientHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_CommandList->RSSetViewports(1, &vp);
}

void D3D12Adapter::setScissorRect()
{
	m_ScissorRect = { 0, 0, m_ClientWidth / 2, m_ClientHeight / 2 };
	m_CommandList->RSSetScissorRects(1, &m_ScissorRect);
}

void D3D12Adapter::buildDescriptorHeaps()
{

}

void D3D12Adapter::buildConstantBuffers()
{
}

void D3D12Adapter::buildRootSignature()
{
}

void D3D12Adapter::buildShadersAndInputLayout()
{
}

void D3D12Adapter::buildGeometry()
{
}

void D3D12Adapter::buildPSO()
{
}

NAMESPACE_LUCKY_END
