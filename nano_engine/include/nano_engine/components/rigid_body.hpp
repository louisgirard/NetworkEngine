#pragma once

#include <nano_engine/components/position.hpp>
#include <nano_engine/components/colliders/collider.hpp>

#include <btBulletDynamicsCommon.h>

#include <nano_engine/common.hpp>

namespace nano_engine::components
{
	struct RigidBodyHolder
	{
		btRigidBody* m_rigidBody;
		btCollisionShape* m_collisionShape;
		btDefaultMotionState* m_motionState;

		~RigidBodyHolder()
		{
			delete m_rigidBody;
			delete m_collisionShape;
			delete m_motionState;
		}
	};

	class RigidBody
	{
	public:
		RigidBody(const Collider& collider, float mass, const Position& pos) : m_localInertia(0,0,0)
		{
			//Create the state holder
			m_stateHolder = std::make_shared<RigidBodyHolder>();

			//Save values
			m_stateHolder->m_collisionShape = collider.CollisionShape();
			m_mass = mass;

			//Create the bullet mass
			btScalar btMass(mass);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			m_isDynamic = (mass != 0.f);

			//Compute inertia
			if (m_isDynamic)
			{
				m_stateHolder->m_collisionShape->calculateLocalInertia(btMass, m_localInertia);
			}

			//Create the transform
			btTransform startTransform;
			startTransform.setIdentity();
			startTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
			m_stateHolder->m_motionState = new btDefaultMotionState(startTransform);

			//Create the rigidbody
			btRigidBody::btRigidBodyConstructionInfo rbInfo(btMass, m_stateHolder->m_motionState, m_stateHolder->m_collisionShape, m_localInertia);
			m_stateHolder->m_rigidBody = new btRigidBody(rbInfo);
		}

		~RigidBody()
		{
			m_stateHolder = nullptr;
		}

		bool IsInWorld() const { return m_isInWorld; }
		void IsInWorld(bool inWorld) { m_isInWorld = inWorld; }

		btRigidBody* GetRigidBody() { return m_stateHolder->m_rigidBody; }

		void SetPosition(float x, float y, float z)
		{
			auto trans = m_stateHolder->m_rigidBody->getWorldTransform();
			trans.setOrigin(btVector3(x, y, z));
			m_stateHolder->m_motionState->setWorldTransform(trans);
			m_stateHolder->m_rigidBody->setWorldTransform(trans);
		}
	private:
		bool m_isInWorld = false;
		bool m_isDynamic = false;

		std::shared_ptr<RigidBodyHolder> m_stateHolder;
		float m_mass;
		btVector3 m_localInertia;
	};
}