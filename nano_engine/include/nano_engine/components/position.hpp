#pragma once

#include <nano_engine/serialization/input_memory_stream.hpp>
#include <nano_engine/serialization/output_memory_stream.hpp>

namespace nano_engine::components
{
	struct Position
	{
		float x;
		float y;
		float z;

		void Write(serialization::OutputMemoryStream& stream) const
		{
			stream.Write(x);
			stream.Write(y);
			stream.Write(z);
		}

		void Read(serialization::InputMemoryStream& stream)
		{
			x = stream.Read<float>();
			y = stream.Read<float>();
			z = stream.Read<float>();
		}
	};
}