#pragma once

namespace nano_engine::components
{
	struct Velocity
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