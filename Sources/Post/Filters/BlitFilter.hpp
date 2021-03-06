#pragma once

#include <Post/PostFilter.hpp>

using namespace acid;

namespace acid {
class ACID_EXPORT BlitFilter : public PostFilter {
public:
	explicit BlitFilter(const Pipeline::Stage &pipelineStage);

	void Render(const CommandBuffer &commandBuffer) override;

private:
	bool lastFilter;
};
}
