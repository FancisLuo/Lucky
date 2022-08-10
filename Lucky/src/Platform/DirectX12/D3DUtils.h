#pragma once

#include <wrl.h>
#include <d3d12.h>

class D3DUtils
{
public:
	static UINT CalcConstantBufferByteSize(UINT byteSize);

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
		ID3D12Device*							pDevice,
		ID3D12GraphicsCommandList*				pCmdList,
		const void*								initData,
		UINT64									byteSize,
		Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer
	);
};

