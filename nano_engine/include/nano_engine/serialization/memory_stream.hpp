#pragma once

#include <memory>

#include <nano_engine/serialization/endianness.hpp>

namespace nano_engine::serialization
{
	class MemoryStream
	{
	public:
		MemoryStream()
		{
			m_buffer = static_cast<char*>(std::malloc(32));
			m_capacity = 32;
		}

		MemoryStream(size_t initialCapacity)
		{
			m_buffer = static_cast<char*>(std::malloc(initialCapacity));
			m_capacity = initialCapacity;
		}

		~MemoryStream()
		{
			std::free(m_buffer);
		}

		size_t Size() const { return m_size; }
		const char* Data() const { return m_buffer; }

		template<typename T>
		void Write(T data)
		{
			// Check during compilation, if T is a number or enum
			static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "Generic write only supports primitive data types");
			
			if (DetectEndianness() == PlatformEndianness::BigEndian)
			{
				//Convert to little endian
				auto dateLittleEndian = SwapEndian(data);
				Write(reinterpret_cast<char*>(&dateLittleEndian), sizeof(dateLittleEndian));
			}
			else
			{
				Write(reinterpret_cast<char*>(&data), sizeof(data));
			}
		}

		void Write(const char* data, size_t dataSize)
		{
			if (m_size + dataSize >= m_capacity)
			{
				Resize(2 * m_capacity);
			}

			std::memcpy(m_buffer + m_size, data, dataSize);
			m_size += dataSize;
		}

	private:
		void Resize(size_t capacity)
		{
			m_buffer = static_cast<char*>(std::realloc(m_buffer, capacity));
			m_capacity = capacity;
		}

		char* m_buffer;
		size_t m_capacity;
		size_t m_size;
	};
}