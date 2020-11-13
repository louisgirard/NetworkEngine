#pragma once

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include <nano_engine/common.hpp>

namespace nano_engine::components
{
	class Collider
	{
	public:
		virtual btCollisionShape* CollisionShape() const { return m_collisionShape; }

	protected:
		btCollisionShape* m_collisionShape;
	};
}