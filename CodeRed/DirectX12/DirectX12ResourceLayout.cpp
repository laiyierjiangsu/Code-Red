#include "../Shared/Exception/InvalidException.hpp"
#include "../Shared/Exception/FailedException.hpp"

#include "DirectX12Resource/DirectX12Texture.hpp"
#include "DirectX12Resource/DirectX12Sampler.hpp"
#include "DirectX12Resource/DirectX12Buffer.hpp"
#include "DirectX12ResourceLayout.hpp"
#include "DirectX12LogicalDevice.hpp"

#include <algorithm>

#undef max

#ifdef __ENABLE__DIRECTX12__

using namespace CodeRed::DirectX12;

CodeRed::DirectX12ResourceLayout::DirectX12ResourceLayout(
	const std::shared_ptr<GpuLogicalDevice>& device,
	const std::vector<ResourceLayoutElement>& elements,
	const std::vector<SamplerLayoutElement>& samplers,
	const std::optional<Constant32Bits>& constant32Bits)
	: GpuResourceLayout(device, elements, samplers, constant32Bits)
{
	std::vector<D3D12_STATIC_SAMPLER_DESC> samplerArrays;
	std::vector<D3D12_DESCRIPTOR_RANGE> ranges(elements.size());

	size_t index = 0;

	for (auto& element : mElements) {
		ranges[index++] = {
			enumConvert(element.Type),
			1,
			static_cast<UINT>(element.Binding),
			static_cast<UINT>(element.Space),
			static_cast<UINT>(index)
		};
	}

	for (auto& sampler : mSamplers) {
		auto desc = static_cast<DirectX12Sampler*>(sampler.Sampler.get())->sampler();

		desc.ShaderVisibility = enumConvert(sampler.Visibility);
		desc.ShaderRegister = static_cast<UINT>(sampler.Binding);
		desc.RegisterSpace = static_cast<UINT>(sampler.Space);

		samplerArrays.push_back(desc);
	}

	std::vector<D3D12_ROOT_PARAMETER> parameters;
	D3D12_ROOT_SIGNATURE_DESC desc = {};

	//we disable the shader visibility property
	//so the visibility is always all
	if (mElements.empty() == false) {
		//get the root parameters index of elements(descriptor table)
		//it should be zero.
		mElementsIndex = parameters.size();
		
		D3D12_ROOT_PARAMETER parameter;
		
		parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		parameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		parameter.DescriptorTable.NumDescriptorRanges = static_cast<UINT>(ranges.size());
		parameter.DescriptorTable.pDescriptorRanges = ranges.data();

		parameters.push_back(parameter);
	}

	//if we use the constant bits, we need add it to root parameter
	if (mConstant32Bits.has_value()) {
		//get the root parameters index of constant 32bits
		//if we do not have elements the index should be zero
		//else it should ne one
		mConstant32BitsIndex = parameters.size();
		
		D3D12_ROOT_PARAMETER parameter;

		parameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		parameter.ShaderVisibility = enumConvert(mConstant32Bits->Visibility);
		parameter.Constants.Num32BitValues = static_cast<UINT>(mConstant32Bits->Count);
		parameter.Constants.ShaderRegister = static_cast<UINT>(mConstant32Bits->Binding);
		parameter.Constants.RegisterSpace = static_cast<UINT>(mConstant32Bits->Space);

		parameters.push_back(parameter);
	}
	
	desc.NumStaticSamplers = static_cast<UINT>(samplerArrays.size());
	desc.NumParameters = static_cast<UINT>(parameters.size());
	desc.pStaticSamplers = samplerArrays.data();
	desc.pParameters = parameters.data();
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	WRL::ComPtr<ID3DBlob> rootBlob;
	WRL::ComPtr<ID3DBlob> errorBlob;

	CODE_RED_THROW_IF_FAILED(
		D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1,
			rootBlob.GetAddressOf(), errorBlob.GetAddressOf()),
		FailedException(DebugType::Create, { "SerializeRootSignature" })
	);

	const auto dxDevice = std::static_pointer_cast<DirectX12LogicalDevice>(mDevice)->device();

	CODE_RED_THROW_IF_FAILED(
		dxDevice->CreateRootSignature(0,
			rootBlob->GetBufferPointer(),
			rootBlob->GetBufferSize(),
			IID_PPV_ARGS(&mRootSignature)),
		FailedException(DebugType::Create, { "ID3D12RootSignature" })
	);
}

#endif
