#pragma once

#include <nano_engine/serialization/input_memory_stream.hpp>
#include <nano_engine/serialization/output_memory_stream.hpp>

namespace nano_engine::components
{
	struct Rotation
	{
		float x;
		float y;
		float z;
		float w;

		void Write(serialization::OutputMemoryStream& stream) const
		{
			stream.Write(x);
			stream.Write(y);
			stream.Write(z);
			stream.Write(w);
		}

		void Read(serialization::InputMemoryStream& stream)
		{
			x = stream.Read<float>();
			y = stream.Read<float>();
			z = stream.Read<float>();
			w = stream.Read<float>();
		}
	};
}