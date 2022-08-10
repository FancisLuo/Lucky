#pragma once

#include "../../../src/LuckyMacros.h"
#include <cstdint>

NAMESPACE_LUCKY_START

class UUID
{
public:
	UUID();
	UUID(uint64_t uuid);
	UUID(const UUID&) = default;

	operator uint64_t() const { return m_UUID; }

private:
	uint64_t m_UUID;
};

NAMESPACE_LUCKY_END
