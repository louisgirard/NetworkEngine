#pragma once

#include <nano_engine/common.hpp>

#include <nano_engine/engine/entity.hpp>

namespace nano_engine::components
{
	class ReplicationComponent
	{
	public:
		explicit ReplicationComponent(engine::Entity* parent) : m_parent(parent)
		{

		} 

		engine::Entity* Entity() { return m_parent; }
	private:
		engine::Entity* m_parent;
	};
}