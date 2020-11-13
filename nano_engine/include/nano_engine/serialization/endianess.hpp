#pragma once

#include <algorithm>
#include <array>
#include <cstdint>

namespace nano_engine::serialization
{
	enum class PlatformEndianess
	{
		BigEndian,
		LittleEndian
	};

	constexpr PlatformEndianess DetectEndianess()
	{
		union {
			uint32_t i;
			char c[4];
		} testEndian = {0x01020304};

		return (testEndian.c[0] == 0x01) ? PlatformEndianess::BigEndian : PlatformEndianess::LittleEndian;
	}

	template<typename T>
	T SwapEndian(const T& val)
	{
		union {
			T val;
			std::array<std::uint8_t, sizeof(T)> raw;
		} src, dst;

		src.val = val;
		std::reverse_copy(src.raw.begin(), src.raw.end(), dst.raw.begin());
		return dst.val;
	}
}