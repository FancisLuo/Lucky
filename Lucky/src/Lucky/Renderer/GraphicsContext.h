#pragma once

#include "../../Lucky.h"

#include <memory>

NAMESPACE_LUCKY_START

class GraphicsContext
{
public:
	virtual ~GraphicsContext() = default;

	virtual void Init() = 0;
	virtual void SwapBuffers() = 0;

	static std::unique_ptr<GraphicsContext> Create(void* window);
};

NAMESPACE_LUCKY_END
