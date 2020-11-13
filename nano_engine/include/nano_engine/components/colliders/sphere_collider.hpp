#pragma once

#include <BulletCollision/CollisionShapes/btSphereShape.h>

#include <nano_engine/components/colliders/collider.hpp>

#include <nano_engine/common.hpp>

namespace nano_engine::components
{
	class SphereCollider : public Collider
	{
	public:
		SphereCollider(float radius)
		{
			m_collisionShape = new btSphereShape(btScalar(radius));
		}
	};
}