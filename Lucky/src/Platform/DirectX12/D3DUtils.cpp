#include "D3DUtils.h"

#include "D3D12Util.h"
#include "HrException.h"

UINT D3DUtils::CalcConstantBufferByteSize(UINT byteSize)
{
    return (byteSize + 255) & (~255);
}

Microsoft::WRL::ComPtr<ID3D12Resource> D3DUtils::CreateDefaultBuffer(
    ID3D12Device*                           pDevice,
    ID3D12GraphicsCommandList*              pCmdList,
    const void*                             initData,
    UINT64                                  byteSize,
    Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
{
    Microsoft::WRL::ComPtr<ID3D12Resource> defaultBuffer;

    // ����ʵ�ʵ�Ĭ�ϻ�������Դ
    CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC rcDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
    ThrowIfFailed(
        pDevice->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &rcDesc, 
            D3D12_RESOURCE_STATE_COMMON,
            nullptr,
            IID_PPV_ARGS(defaultBuffer.GetAddressOf()))
    );

    CD3DX12_HEAP_PROPERTIES uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    // Ϊ�˽�CPU���ڴ����ݸ��Ƶ�Ĭ�ϻ�����������Ҫ����һ�������н�λ�õ��ϴ���
    ThrowIfFailed(
        pDevice->CreateCommittedResource(
            &uploadHeapProperties,
            D3D12_HEAP_FLAG_NONE,
            &rcDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(uploadBuffer.GetAddressOf()))
    );

    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData       = initData;
    subResourceData.RowPitch    = byteSize;
    subResourceData.SlicePitch  = subResourceData.RowPitch;

    // �����ݸ��Ƶ�Ĭ�ϻ�������Դ������
    // UpdateSubresources�����������Ƚ����ݴ�CPU�˵��ڴ渴�Ƶ�λ���н�λ�õ��ϴ�������ţ�
    // ��ͨ������ID3D12CommandList::CopySubresourceRegion���������ϴ����ڵ����ݸ��Ƶ�mBuffer��
    CD3DX12_RESOURCE_BARRIER commonToCopy = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
    pCmdList->ResourceBarrier(1, &commonToCopy);
    UpdateSubresources<1>(pCmdList, defaultBuffer.Get(), uploadBuffer.Get(), 0, 0, 1, &subResourceData);

    CD3DX12_RESOURCE_BARRIER copyToRead = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
    pCmdList->ResourceBarrier(1, &copyToRead);

    return defaultBuffer;
}
