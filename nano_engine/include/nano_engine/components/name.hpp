#pragma once

#include <nano_engine/common.hpp>
#include <nano_engine/serialization/input_memory_stream.hpp>
#include <nano_engine/serialization/output_memory_stream.hpp>

#include <iostream>
namespace nano_engine::components
{
	struct Name
	{
		std::string name;

		void Write(serialization::OutputMemoryStream& stream) const
		{
			stream.Write(name.size());
			stream.Write(name.c_str(), name.size());
		}

		void Read(serialization::InputMemoryStream& stream)
		{
			size_t size = stream.Read<size_t>();
			char* tempName = (char*)malloc(size);
			memset(tempName, 0, size);
			stream.Read(tempName, size);
			name.assign(tempName, size);
			free(tempName);
		}
	};
}