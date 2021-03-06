#pragma once

#include "../Shared/Noncopyable.hpp"

#include <memory>
#include <vector>

namespace CodeRed {

	class GpuDisplayAdapter;
	
	class GpuSystemInfo : public Noncopyable {
	protected:
		GpuSystemInfo() = default;
		
		~GpuSystemInfo() = default;
	public:
		virtual auto selectDisplayAdapter() const->std::vector<std::shared_ptr<GpuDisplayAdapter>> = 0;
	};
	
}