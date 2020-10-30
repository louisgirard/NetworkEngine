#pragma once

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

#include <nano_engine/components/colliders/collider.hpp>

#include <nano_engine/common.hpp>

namespace nano_engine::components
{
	class CapsuleCollider : public Collider
	{
	public:
		CapsuleCollider(float radius, float height)
		{
			m_collisionShape = new btCapsuleShape(btScalar(radius), btScalar(height));
		}
	};
}
