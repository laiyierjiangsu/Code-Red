#pragma once

#ifdef __CODE__RED__GLOBAL__INCLUDE__
#include <Interface/GpuGraphicsCommandList.hpp>
#include <DirectX12/DirectX12Utility.hpp>
#else
#include "../Interface/GpuGraphicsCommandList.hpp"
#include "DirectX12Utility.hpp"
#endif

#ifdef __ENABLE__DIRECTX12__

namespace CodeRed {

	class DirectX12GraphicsCommandList final : public GpuGraphicsCommandList {
	public:
		explicit DirectX12GraphicsCommandList(
			const std::shared_ptr<GpuLogicalDevice>& device,
			const std::shared_ptr<GpuCommandAllocator>& allocator);

		~DirectX12GraphicsCommandList() = default;

		void beginRecoding() override;

		void endRecoding() override;

		void reset() override;

		void setGraphicsPipeline(
			const std::shared_ptr<GpuGraphicsPipeline>& pipeline) override;

		void setResourceLayout(
			const std::shared_ptr<GpuResourceLayout>& layout) override;

		void setVertexBuffer(
			const std::shared_ptr<GpuBuffer>& buffer) override;

		void setIndexBuffer(
			const std::shared_ptr<GpuBuffer>& buffer) override;
		
		void setGraphicsConstantBuffer(
			const size_t index,
			const std::shared_ptr<GpuBuffer>& buffer) override;
		
		void setGraphicsTexture(
			const size_t index,
			const std::shared_ptr<GpuTexture>& texture) override;

		void setFrameBuffer(
			const std::shared_ptr<GpuFrameBuffer>& buffer) override;

		void setViewPort(
			const ViewPort& view_port) override;

		void setScissorRect(
			const ScissorRect& rect) override;

		void clearRenderTarget(
			const std::shared_ptr<GpuFrameBuffer>& buffer, 
			const Real color[4],
			const size_t index = 0) override;

		void clearDepthStencil(
			const std::shared_ptr<GpuFrameBuffer>& buffer, 
			const Real depth = 0, 
			const UInt8 stencil = 0) override;

		void layoutTransition(
			const std::shared_ptr<GpuTexture>& texture, 
			const ResourceLayout old_layout, 
			const ResourceLayout new_layout) override;

		void layoutTransition(
			const std::shared_ptr<GpuTexture>& texture, 
			const ResourceLayout layout) override;

		void layoutTransition(
			const std::shared_ptr<GpuBuffer>& buffer, 
			const ResourceLayout old_layout, 
			const ResourceLayout new_layout) override;

		void layoutTransition(
			const std::shared_ptr<GpuBuffer>& buffer, 
			const ResourceLayout layout) override;

		void copyBuffer(
			const std::shared_ptr<GpuBuffer>& source, 
			const std::shared_ptr<GpuBuffer>& destination, 
			const size_t size, 
			const size_t source_offset = 0, 
			const size_t destination_offset = 0) override;

		void copyTexture(
			const std::shared_ptr<GpuTexture>& source, 
			const std::shared_ptr<GpuTexture>& destination, 
			const Extent3D<UInt32>& region, 
			const size_t x = 0, 
			const size_t y = 0,
			const size_t z = 0) override;

		void draw(
			const size_t vertex_count, 
			const size_t instance_count = 1, 
			const size_t start_vertex_location = 0, 
			const size_t start_instance_location = 0) override;

		void drawIndexed(
			const size_t index_count, 
			const size_t instance_count, 
			const size_t start_index_location, 
			const size_t base_vertex_location, 
			const size_t start_instance_location) override;
		
		auto commandList() const noexcept -> WRL::ComPtr<ID3D12GraphicsCommandList> { return mGraphicsCommandList; }
	private:
		static D3D12_RESOURCE_BARRIER resource_barrier(
			ID3D12Resource* pResource,
			const D3D12_RESOURCE_STATES before,
			const D3D12_RESOURCE_STATES after);
	private:
		WRL::ComPtr<ID3D12GraphicsCommandList> mGraphicsCommandList;

		std::shared_ptr<GpuResourceLayout> mResourceLayout;
	};
	
}

#endif