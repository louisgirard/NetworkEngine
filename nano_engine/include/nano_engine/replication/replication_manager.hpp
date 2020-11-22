#pragma once

#include <nano_engine/common.hpp>

#include <nano_engine/systems/system.hpp>

namespace nano_engine::replication
{
	class ReplicationManagerImpl;
	class ReplicationManager : public systems::ISystem
	{
	public:
		ReplicationManager(bool server, const std::string& ip, uint16_t port);
		~ReplicationManager();

		void BeginFrame() override;
		void Update(std::chrono::microseconds deltaTime, engine::World& world) override;
		void EndFrame() override;

	private:
		std::shared_ptr<ReplicationManagerImpl> m_impl;
	};
}