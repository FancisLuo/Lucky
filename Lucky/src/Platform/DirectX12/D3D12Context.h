#pragma once

#include "./../../Lucky/Renderer/GraphicsContext.h"
#include "../../Lucky.h"

NAMESPACE_LUCKY_START

class D3D12Context : public GraphicsContext
{
public:
	D3D12Context(void* handle);

	virtual void Init() override;
	virtual void SwapBuffers() override;
};

NAMESPACE_LUCKY_END