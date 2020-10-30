#pragma once

#include <BulletCollision/CollisionShapes/btBoxShape.h>

#include <nano_engine/components/colliders/collider.hpp>

#include <nano_engine/common.hpp>

namespace nano_engine::components
{
	class BoxCollider : public Collider
	{
	public:
		BoxCollider(float X, float Y, float Z)
		{
			m_collisionShape = new btBoxShape(btVector3(btScalar(X / 2.0f), btScalar(Y / 2.0f), btScalar(Z / 2.0f)));
		}
	};
}
