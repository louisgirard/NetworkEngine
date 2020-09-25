#pragma once

#include <nano_engine/common.hpp>

namespace nano_engine::systems
{
	class ISystem
	{
	public:
		virtual void Update(std::chrono::milliseconds deltaTime) = 0;
	};
}