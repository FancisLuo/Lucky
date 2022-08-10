#pragma once

#include <stdexcept>
#include <Windows.h>

std::string HrToString(HRESULT hr);

void ThrowIfFailed(HRESULT hr);

class HrException : public std::runtime_error
{
public:
	HrException(HRESULT hr): std::runtime_error(HrToString(hr)), m_hr(hr) {}

	HRESULT Error() const { return m_hr; }

private:
	const HRESULT m_hr;
};


