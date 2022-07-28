#pragma once

#include "Core.h"
#include "../Lucky.h"

NAMESPACE_LUCKY_START

class LUCKY_API Application
{
public:
	Application();
	virtual ~Application();

	void Run();
};

NAMESPACE_LUCKY_END

