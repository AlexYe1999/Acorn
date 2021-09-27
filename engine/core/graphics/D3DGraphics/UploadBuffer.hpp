#include"D3DUtil.hpp"
#include"d3dx12.h"
#include<D3D12.h>
#include<cstdint>
#include<wrl.h>

namespace AcornEngine{
    
    template<typename DataType, bool isConstantBuffer>
    class UploadBuffer{
    public:
        UploadBuffer(ID3D12Device* device, uint16_t elementCount)
            : 
            m_pMappedData(nullptr),
            m_uElementByteSize(0),
            m_bIsConstantBuffer(isConstantBuffer)
        {   
            m_uElementByteSize = constexpr(isConstantBuffer 
                ? D3DUtil::CalcAlignment(sizeof(DataType)) : sizeof(DataType));

            device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, 
                &CD3DX12_RESOURCE_DESC::Buffer(m_uElementByteSize * elementCount),
                D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, 
                IID_PPV_ARGS(m_pUploadBuffer.GetAddressOf())
            );

            m_pUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_pMappedData));
        }


        UploadBuffer(const UploadBuffer& rhs) = delete;
        UploadBuffer& operator = (const UploadBuffer& rhs) = delete;

        ~UploadBuffer(){
            if(m_pUploadBuffer){
                m_pUploadBuffer->Unmap(0, nullptr);
            }
            m_pMappedData = nullptr;
        }

        ID3D12Resource *Resource() const{
            return m_pUploadBuffer.Get();
        }

        void CopyData(int elementIndex, const DataType& data){
            memcpy(&m_pMappedData[m_uElementByteSize * elementIndex], &data, sizeof(DataType));
        }
    private:
        Microsoft::WRL::ComPtr<ID3D12Resource> m_pUploadBuffer;
        BYTE* m_pMappedData;

        uint16_t m_uElementByteSize;
        bool m_bIsConstantBuffer;
    };

}