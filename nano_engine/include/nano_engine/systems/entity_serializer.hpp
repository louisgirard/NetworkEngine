#pragma once

#include <nano_engine/common.hpp>

#include <nano_engine/systems/system.hpp>

namespace nano_engine::systems
{
	class EntitySerializerImpl;
	class EntitySerializer : public ISystem
	{
	public:
		EntitySerializer();
		~EntitySerializer();

		EntitySerializer(const EntitySerializer& other) = delete;
		EntitySerializer(EntitySerializer&& other) = delete;

		void Update(std::chrono::microseconds deltaTime, engine::World& world) override;
		void EndFrame() override;

	private:
		std::shared_ptr<EntitySerializerImpl> m_impl;
	};
}