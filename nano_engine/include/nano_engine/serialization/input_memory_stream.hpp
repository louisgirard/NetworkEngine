#pragma once

#include <nano_engine/serialization/memory_stream.hpp>
#include <nano_engine/serialization/output_memory_stream.hpp>

namespace nano_engine::serialization
{
	class InputMemoryStream
	{
	public:
		InputMemoryStream(OutputMemoryStream&& stream) : m_stream(std::move(stream.RawStream()))
		{
			m_stream.Size(stream.Size());
		}

		InputMemoryStream(const char* data, size_t size) : m_stream(data, size)
		{
			m_stream.Size(size);
		}

		size_t Size() const { return m_stream.Size(); }
		const char* Data() const { return m_stream.Data(); }

		template<typename T>
		T Read()
		{
			return m_stream.Read<T>();
		}

		void Read(char* data, size_t dataSize)
		{
			m_stream.Read(data, dataSize);
		}

	private:
		MemoryStream m_stream;
	};
}