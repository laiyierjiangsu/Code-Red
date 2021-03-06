#pragma once

#include "../../Interface/GpuResource/GpuSampler.hpp"
#include "../DirectX12Utility.hpp"

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12Sampler final : public GpuSampler {
	public:
		explicit DirectX12Sampler(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const SamplerInfo& info);

		~DirectX12Sampler() = default;

		auto sampler() const noexcept -> D3D12_STATIC_SAMPLER_DESC { return mSampler; }
	private:
		D3D12_STATIC_SAMPLER_DESC mSampler;
	};
	
}

#endif