#include <btBulletDynamicsCommon.h>

#include <nano_engine/systems/physic.hpp>

#include <nano_engine/engine/entity.hpp>

namespace nano_engine::systems
{
	class PhysicsImpl
	{
	public:
		PhysicsImpl(float gravityX, float gravityY, float gravityZ)
		{
			m_collisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
			m_collisionDispatcher = std::make_shared<btCollisionDispatcher>(m_collisionConfiguration.get());
			m_overlappingPairCache = std::make_shared<btDbvtBroadphase>();
			m_solver = std::make_shared<btSequentialImpulseConstraintSolver>();
			m_dynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(m_collisionDispatcher.get(), m_overlappingPairCache.get(), m_solver.get(), m_collisionConfiguration.get());

			m_dynamicsWorld->setGravity(btVector3(gravityX, gravityY, gravityZ));
		}

		virtual ~PhysicsImpl()
		{

		}

		PhysicsImpl(const PhysicsImpl& other) = delete;
		PhysicsImpl(PhysicsImpl&& other) = delete;

		void Update(std::chrono::microseconds deltaTime, engine::World& world)
		{
			m_lastStepTime += deltaTime;
			if (m_lastStepTime < 1'000'000us / 60) return;

			for (auto it = world.Entities().begin(); it != world.Entities().end(); it++)
			{
				auto body = (*it)->GetRigidBody();
				if (body != nullptr && !body->IsInWorld())
				{
					m_dynamicsWorld->addRigidBody(body->GetRigidBody());
					body->IsInWorld(true);
				}
			}

			//Update the physic world
			m_dynamicsWorld->stepSimulation(m_lastStepTime.count()/1'000'000.0f, 10);
			for (auto it = world.Entities().begin(); it != world.Entities().end(); it++)
			{
				btTransform trans;
				auto body = (*it)->GetRigidBody();
				if (body == nullptr) return;

				if (body->GetRigidBody() && body->GetRigidBody()->getMotionState())
				{
					body->GetRigidBody()->getMotionState()->getWorldTransform(trans);
				}
				else
				{
					trans = body->GetRigidBody()->getWorldTransform();
				}
				components::Position pos;
				pos.x = trans.getOrigin().getX();
				pos.y = trans.getOrigin().getY();
				pos.z = trans.getOrigin().getZ();

				(*it)->SetPosition(pos.x, pos.y, pos.z);
			}

			m_lastStepTime = 0ms;
		}

	private:
		std::shared_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
		std::shared_ptr<btCollisionDispatcher> m_collisionDispatcher;
		std::shared_ptr<btBroadphaseInterface> m_overlappingPairCache;
		std::shared_ptr<btSequentialImpulseConstraintSolver> m_solver;
		std::shared_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;

		std::chrono::microseconds m_lastStepTime = 0ms;
	};

	Physics::Physics(float gravityX, float gravityY, float gravityZ)
	{
		m_impl = std::make_unique<PhysicsImpl>(gravityX, gravityY, gravityZ);
	}

	Physics::~Physics()
	{
		m_impl = nullptr;
	}
	
	void Physics::Update(std::chrono::microseconds deltaTime, engine::World& world)
	{
		m_impl->Update(deltaTime, world);
	}
}