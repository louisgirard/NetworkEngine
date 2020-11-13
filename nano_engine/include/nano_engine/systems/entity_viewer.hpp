#pragma once

#include <nano_engine/common.hpp>

#include <nano_engine/systems/system.hpp>

namespace nano_engine::systems
{
	class EntityViewerImpl;
	class EntityViewer : public ISystem
	{
	public:
		EntityViewer();
		virtual ~EntityViewer();

		EntityViewer(const EntityViewer& other) = delete;
		EntityViewer(EntityViewer&& other) = delete;

		virtual void Update(std::chrono::microseconds deltaTime, engine::World& world) override;

	private:
		std::unique_ptr<EntityViewerImpl> m_impl;
	};
}