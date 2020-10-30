#pragma once

#include <algorithm>
#include <array>
#include <cstdint>

namespace nano_engine::serialization
{
	enum class PlatformEndianness
	{
		BigEndian,
		LittleEndian
	};

	constexpr PlatformEndianness DetectEndianness()
	{
		union
		{
			uint32_t i;
			char c[4];
		} testEndian = {0x01020304};

		// little endian -> 04 03 02 01
		// big endian -> 01 02 03 04

		if (testEndian.c[0] == 0x01)
		{
			return PlatformEndianness::BigEndian;
		}
		else
		{
			return PlatformEndianness::LittleEndian;
		}
	}

	template<typename T>
	T SwapEndian(const T& val)
	{
		union
		{
			T val;
			std::array<std::uint8_t, sizeof(T)> raw;
		} src, dest;

		src.val = val;
		std::reverse_copy(src.raw.begin(), src.raw.end(), dest.raw.begin());

		return dest.val;
	}
}