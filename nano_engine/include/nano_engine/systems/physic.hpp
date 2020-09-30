#pragma once

#include <nano_engine/common.hpp>
#include <nano_engine/systems/system.hpp>

namespace nano_engine::systems
{
	class PhysicsImpl;
	class Physics : public ISystem
	{
	public:
		Physics();
		virtual ~Physics();

		Physics(const Physics& other) = delete;
		Physics(Physics&& other) = delete;

		void Update(std::chrono::milliseconds deltaTime, engine::World& world) override;

	private:
		std::unique_ptr<PhysicsImpl> m_impl;
	};
}