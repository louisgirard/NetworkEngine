#pragma once

#include <nano_engine/common.hpp>

#include <nano_engine/systems/system.hpp>

namespace nano_engine::replication
{
	class ReplicationManager : public systems::ISystem
	{
		void BeginFrame() override;
		void Update(std::chrono::microseconds deltaTime, engine::World& world) override;
		void EndFrame() override;
	};
}