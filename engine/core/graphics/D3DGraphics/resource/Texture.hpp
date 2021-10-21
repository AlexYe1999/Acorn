#pragma once
#include<string>
#include<wrl.h>
#include<D3D12.h>

namespace Acorn{
	struct Texture{
		std::string Name;
		std::wstring FileName;

		Microsoft::WRL::ComPtr<ID3D12Resource> Resource = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> UploadHeap = nullptr;
	};


}