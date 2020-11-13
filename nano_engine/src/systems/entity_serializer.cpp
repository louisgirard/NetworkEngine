#include <nano_engine/systems/entity_serializer.hpp>

#include <entt/entt.hpp>

#include <nano_engine/serialization/output_memory_stream.hpp>

#include <nano_engine/components/position.hpp>
#include <nano_engine/components/velocity.hpp>
#include <nano_engine/components/rotation.hpp>

namespace nano_engine::systems
{
	class EntitySerializerImpl
	{
	public:
		EntitySerializerImpl()
		{

		}

		~EntitySerializerImpl()
		{

		}

		void Update(std::chrono::microseconds deltaTime, engine::World& world)
		{
			m_stream.Reset();
			m_world = &world;
			auto worldView = world.Registry().view<components::Position,
													components::Rotation>();
			for (entt::entity entity : worldView)
			{
				m_stream.Write(entity);
				worldView.get<components::Position>(entity).Write(m_stream);
				worldView.get<components::Rotation>(entity).Write(m_stream);
			}
		}

		void EndFrame()
		{
			serialization::InputMemoryStream stream(m_stream.Data(), m_stream.Size());

			auto worldView = m_world->Registry().view<components::Position,components::Rotation>();

			auto entityID = stream.Read<entt::entity>();
			worldView.get<components::Position>(entityID).Read(stream);
			worldView.get<components::Rotation>(entityID).Read(stream);

			entityID = stream.Read<entt::entity>();
			worldView.get<components::Position>(entityID).Read(stream);
			worldView.get<components::Rotation>(entityID).Read(stream);
		}

	private:
		serialization::OutputMemoryStream m_stream;
		engine::World* m_world;
	};

	EntitySerializer::EntitySerializer()
	{
		m_impl = std::make_shared<EntitySerializerImpl>();
	}

	EntitySerializer::~EntitySerializer()
	{
		m_impl = nullptr;
	}

	void EntitySerializer::Update(std::chrono::microseconds deltaTime, engine::World& world)
	{
		m_impl->Update(deltaTime, world);
	}

	void EntitySerializer::EndFrame()
	{
		m_impl->EndFrame();
	}
}