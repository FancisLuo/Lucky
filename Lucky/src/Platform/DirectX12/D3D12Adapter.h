#pragma once

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <dxgi1_4.h>
#include <wrl.h>
#include <d3d12.h>


#include "D3D12Util.h"
#include "../../Lucky.h"
#include "D3D12UploadBuffer.h"

using namespace Microsoft::WRL;

NAMESPACE_LUCKY_START

class LUCKY_API D3D12Adapter
{
public:
	D3D12Adapter();
	virtual ~D3D12Adapter() = default;

	void PreInitialize(const DX3D12_ADAPTER_INFO* pInfo);
	void Initialize();
	void PostInitialize();

	void OnUpdate();
	void OnRender();

	void OnDestroy();

protected:
	void flushCommandQueue();

private:
	// ���ݳ�ʼ������
	void enableDebugLayer();
	void createDevice();
	void createFence();
	void checkMSAA();
	void createCommandObjects();
	void createSwapChain();
	void createRtvAndDsvDescriptorHeaps();
	void createRenderTargetView(); // RTV
	void createDepthStencilView();
	void setViewport();
	void setScissorRect();

	// ������Ⱦ����Ҫ������
	void buildDescriptorHeaps();
	void buildConstantBuffers();
	void buildRootSignature();
	void buildShadersAndInputLayout();
	void buildGeometry();
	void buildPSO();

private:

private:
	ComPtr<IDXGIFactory4>									m_DxgiFactory;
	ComPtr<ID3D12Device>									m_D3DDevice;
	ComPtr<IDXGISwapChain>									m_SwapChain;
	ComPtr<ID3D12Fence>										m_Fence;

	UINT64													m_CurrentFenceValue;
	HANDLE													m_FenceEvent;

	bool													m_4xMsaaState								= false;
	UINT													m_4xMsaaQuality								= 0;

	DXGI_FORMAT												m_BackBufferFormat							= DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT												m_DepthStencilFormat						= DXGI_FORMAT_D24_UNORM_S8_UINT;

	UINT													m_RtvDescriptorSize							= 0;
	UINT													m_DsvDescriptorSize							= 0;
	UINT													m_CbvUavDescriptorSize						= 0;

	// �������
	ComPtr<ID3D12CommandQueue>								m_CommandQueue;								// �������
	ComPtr<ID3D12CommandAllocator>							m_DirectCmdListAlloc;						// ������
	ComPtr<ID3D12GraphicsCommandList>						m_CommandList;								// �����б�(GPU��)

	HWND													m_MainWnd									= nullptr;	// ���ھ��
	int														m_ClientWidth								= 800;
	int														m_ClientHeight								= 600;

	// ��������
	ComPtr<ID3D12DescriptorHeap>							m_RtvHeap;
	ComPtr<ID3D12DescriptorHeap>							m_DsvHeap;

	int														m_CurrentBackBuffer							= 0;	// ��ǰ��̨����������

	static const int										SwapChainBufferCount						= 2;
	// ������
	ComPtr<ID3D12Resource>									m_SwapChainBuffer[SwapChainBufferCount];
	ComPtr<ID3D12Resource>									m_DepthStencilBuffer;

	D3D12_VIEWPORT											m_Viewport;
	D3D12_RECT												m_ScissorRect;

	// build
	ComPtr<ID3D12RootSignature>								m_RootSignature								= nullptr;
	ComPtr<ID3D12DescriptorHeap>							m_CbvHeap									= nullptr;

	// ��ν�Ӧ������Ƕ�뵽engine�У�
	//std::unique_ptr<D3D12UploadBuffer<ObjectConstatns>> m_ObjectCB = nullptr;
	//std::unique_ptr<MeshGeometry> m_BoxGeo = nullptr;

	ComPtr<ID3DBlob>										m_VSByteCode =								nullptr;
	ComPtr<ID3DBlob>										m_PSByteCode =								nullptr;
	std::vector<D3D12_INPUT_ELEMENT_DESC>					m_InputLayout;
	ComPtr<ID3D12PipelineState>								m_PSO										= nullptr;

	bool													m_PreInitialized;
};

NAMESPACE_LUCKY_END

