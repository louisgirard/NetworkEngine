#pragma once

#include <nano_engine/common.hpp>

#include <nano_engine/engine/world.hpp>

namespace nano_engine::systems
{
	class ISystem
	{
	public:
		virtual void Update(std::chrono::milliseconds deltaTime, engine::World& world) = 0;
	};
}