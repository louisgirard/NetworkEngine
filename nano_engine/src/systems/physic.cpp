#include <nano_engine/systems/physic.hpp>

#include <btBulletDynamicsCommon.h>

#include <nano_engine/components/position.hpp>
#include <nano_engine/components/rigid_body.hpp>

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

		~PhysicsImpl()
		{

		}

		PhysicsImpl(const PhysicsImpl& other) = delete;
		PhysicsImpl(PhysicsImpl&& other) = delete;

		void Update(std::chrono::milliseconds deltaTime, engine::World& world)
		{
			m_totalMilli += deltaTime;
			if (m_totalMilli < 1000ms / 60) return;

			// Update physic world
			m_dynamicsWorld->stepSimulation(m_totalMilli.count() / 1000.f, 10);

			world.Registry().view<components::Position, components::RigidBody>().each([this](auto& pos, auto& body)
				{
					if (!body.IsInWorld())
					{
						m_dynamicsWorld->addRigidBody(body.GetRigidBody());
						body.IsInWorld(true);
					}

					btTransform trans;
					if (body.GetRigidBody() && body.GetRigidBody()->getMotionState())
					{
						body.GetRigidBody()->getMotionState()->getWorldTransform(trans);
					}
					else
					{
						trans = body.GetRigidBody()->getWorldTransform();
					}

					pos.x = trans.getOrigin().getX();
					pos.y = trans.getOrigin().getY();
					pos.z = trans.getOrigin().getZ();
				});
			m_totalMilli = 0ms;
		}

	private:
		std::shared_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
		std::shared_ptr<btCollisionDispatcher> m_collisionDispatcher;
		std::shared_ptr<btBroadphaseInterface> m_overlappingPairCache;
		std::shared_ptr<btSequentialImpulseConstraintSolver> m_solver;
		std::shared_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;

		std::chrono::milliseconds m_totalMilli = 0ms;
	};

	Physics::Physics(float gravityX, float gravityY, float gravityZ) : m_impl(std::make_unique<PhysicsImpl>(gravityX, gravityY, gravityZ))
	{
	}

	Physics::~Physics()
	{
		m_impl = nullptr;
	}

	void Physics::Update(std::chrono::milliseconds deltaTime, engine::World& world)
	{
		m_impl->Update(deltaTime, world);
	}
}