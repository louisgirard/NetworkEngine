#pragma once

#include <btBulletDynamicsCommon.h>

#include <nano_engine/common.hpp>

#include <nano_engine/components/position.hpp>

namespace nano_engine::components
{
	class RigidBody
	{
	public:
		RigidBody(btCollisionShape* collisionShape, float mass, const components::Position& pos) :
			m_collisionShape(collisionShape),
			m_mass(mass),
			m_localInertia(0, 0, 0)
		{
			btScalar btMass(mass);

			// Compute Inertia
			if (btMass != 0.f)
				m_collisionShape->calculateLocalInertia(btMass, m_localInertia);

			// Create Transform
			btTransform startTransform;
			startTransform.setIdentity();
			startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
			m_motionState = new btDefaultMotionState(startTransform);

			// Create RigidBody
			btRigidBody::btRigidBodyConstructionInfo rbInfo(btMass, m_motionState, m_collisionShape, m_localInertia);
			m_rigidBody = new btRigidBody(rbInfo);
		}

		~RigidBody()
		{
			delete m_motionState;
			delete m_collisionShape;
			delete m_rigidBody;
		}

		bool IsInWorld() const { return m_isInWorld; }
		void IsInWorld(bool inWorld) { m_isInWorld = inWorld; }

		btRigidBody* GetRigidBody() { return m_rigidBody; }

	private:
		bool m_isInWorld = false;

		btRigidBody* m_rigidBody;
		btCollisionShape* m_collisionShape;
		float m_mass;
		btDefaultMotionState* m_motionState;
		btVector3 m_localInertia;
	};
}