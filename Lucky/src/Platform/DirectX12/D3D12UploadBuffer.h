#pragma once

#include <wrl.h>
#include <d3d12.h>

#include "D3D12Util.h"
#include "D3DUtils.h"
#include "HrException.h"

/// <summary>
/// �ϴ�����������������װ
/// ʵ���ϴ���������Դ�Ĺ�����������������Դ��ӳ���ȡ��ӳ��������ṩCopyData�������»������ڵ��ض�Ԫ�ء�
/// </summary>

template<typename T>
class D3D12UploadBuffer
{
public:
	D3D12UploadBuffer(ID3D12Device* pDevice, UINT elementCount, bool isConstantBuffer):m_IsConstantBuffer(isConstantBuffer)
	{
		m_ElementByteSize = sizeof(T);

		// �����������Ĵ�СΪ256B����������
		if (isConstantBuffer)
		{
			m_ElementByteSize = D3DUtils::CalcConstantBufferByteSize(sizeof(T));
		}

		CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC rcDesc = CD3DX12_RESOURCE_DESC::Buffer(m_ElementByteSize * elementCount);
		ThrowIfFailed(
			pDevice->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &rcDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_UploadBuffer))
		);

		ThrowIfFailed(
			m_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData))
		);
	}

	D3D12UploadBuffer(const D3D12UploadBuffer& rhs) = delete;
	D3D12UploadBuffer& operator=(const D3D12UploadBuffer& rhs) = delete;

	~D3D12UploadBuffer()
	{
		if (m_UploadBuffer != nullptr)
		{
			m_UploadBuffer->Unmap(0, nullptr);
		}

		m_MappedData = nullptr;
	}

public:
	ID3D12Resource* Resource() const
	{
		return m_UploadBuffer.Get();
	}

	void CopyData(int elementIndex, const T& data)
	{
		memcpy(&m_MappedData[elementIndex * m_ElementByteSize], &data, sizeof(T));
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource>	m_UploadBuffer;
	BYTE*									m_MappedData			= nullptr;

	UINT									m_ElementByteSize	= 0;
	bool									m_IsConstantBuffer	= false;
};

