#pragma once

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