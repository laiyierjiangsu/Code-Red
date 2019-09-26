#pragma once

#include <Core/CodeRedGraphics.hpp>

namespace CodeRed {

	class PipelineInfo final : Noncopyable {
	public:
		explicit PipelineInfo(
			const std::shared_ptr<GpuLogicalDevice>& device);

		void setRasterizationState(
			const std::shared_ptr<GpuRasterizationState>& state);

		void setInputAssemblyState(
			const std::shared_ptr<GpuInputAssemblyState>& state);

		void setVertexShaderState(
			const std::shared_ptr<GpuShaderState>& state);

		void setDepthStencilState(
			const std::shared_ptr<GpuDepthStencilState>& state);

		void setPixelShaderState(
			const std::shared_ptr<GpuShaderState>& state);

		void setResourceLayout(
			const std::shared_ptr<GpuResourceLayout>& layout);
		
		void setBlendState(
			const std::shared_ptr<GpuBlendState>& state);

		void updateState();
		
		auto graphicsPipeline() const noexcept -> std::shared_ptr<GpuGraphicsPipeline> { return mGraphicsPipeline; }
	private:
		std::shared_ptr<GpuRasterizationState> mRasterizationState;
		std::shared_ptr<GpuInputAssemblyState> mInputAssemblyState;
		std::shared_ptr<GpuShaderState> mVertexShaderState;
		std::shared_ptr<GpuDepthStencilState> mDepthStencilState;
		std::shared_ptr<GpuShaderState> mPixelShaderState;
		std::shared_ptr<GpuResourceLayout> mResourceLayout;
		std::shared_ptr<GpuBlendState> mBlendState;
		
		std::shared_ptr<GpuGraphicsPipeline> mGraphicsPipeline;
		std::shared_ptr<GpuPipelineFactory> mPipelineFactory;

		std::shared_ptr<GpuLogicalDevice> mDevice;
	};
	
}