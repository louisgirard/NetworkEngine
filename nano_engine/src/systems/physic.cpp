#include <nano_engine/systems/physic.hpp>

#include <nano_engine/components/position.hpp>
#include <nano_engine/components/velocity.hpp>

namespace nano_engine::systems
{
	class PhysicsImpl
	{
	public:
		PhysicsImpl()
		{

		}

		~PhysicsImpl()
		{

		}

		PhysicsImpl(const PhysicsImpl& other) = delete;
		PhysicsImpl(PhysicsImpl&& other) = delete;

		void Update(std::chrono::milliseconds deltaTime, engine::World& world)
		{
			world.Registry().view<components::Position, components::Velocity>().each([deltaTime](auto& pos, auto& vel)
				{
					pos.x += vel.x * (deltaTime.count() / 1000.f);
					pos.y += vel.y * (deltaTime.count() / 1000.f);
					pos.z += vel.z * (deltaTime.count() / 1000.f);
				});
		}
	};

	Physics::Physics() : m_impl(std::make_unique<PhysicsImpl>())
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