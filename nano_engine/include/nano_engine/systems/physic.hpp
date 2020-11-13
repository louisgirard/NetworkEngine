#pragma once

#include <nano_engine/common.hpp>

#include <nano_engine/systems/system.hpp>

namespace nano_engine::systems
{
	class PhysicsImpl;
	class Physics : public ISystem
	{
	public:
		Physics(float gravityX, float gravityY, float gravityZ);
		virtual ~Physics();

		Physics(const Physics& other) = delete;
		Physics(Physics&& other) = delete;

		virtual void Update(std::chrono::microseconds deltaTime, engine::World& world) override;

	private:
		std::unique_ptr<PhysicsImpl> m_impl;
	};
}