#include <spdlog/spdlog.h>

#include <nano_engine/components/name.hpp>
#include <nano_engine/components/position.hpp>
#include <nano_engine/components/rotation.hpp>
#include <nano_engine/components/velocity.hpp>

#include <nano_engine/serialization/input_memory_stream.hpp>
#include <nano_engine/serialization/output_memory_stream.hpp>

#include <nano_engine/systems/entity_serializer.hpp>

namespace nano_engine::systems
{
	class EntitySerializerImpl
	{
	public:
		EntitySerializerImpl()
		{

		}

		virtual ~EntitySerializerImpl()
		{

		}

		EntitySerializerImpl(const EntitySerializerImpl& other) = delete;
		EntitySerializerImpl(EntitySerializerImpl&& other) = delete;

		void Update(std::chrono::milliseconds deltaTime, engine::World& world)
		{
			m_world = &world;

			auto worldView = world.Registry().view<components::Position,
				components::Rotation,
				components::Velocity>();
			for (auto entity : worldView)
			{
				m_stream.Write(entity);
				auto position = worldView.get<components::Position>(entity);
				position.Write(m_stream);
				auto rotation = worldView.get<components::Rotation>(entity);
				rotation.Write(m_stream);
				auto velocity = worldView.get<components::Velocity>(entity);
				velocity.Write(m_stream);
			}
		}

		void EndFrame()
		{
			/*serialization::InputMemoryStream stream(m_stream.Data(), m_stream.Size());

			auto worldView = m_world->Registry().view<components::Position,
				components::Rotation,
				components::Velocity>();

			for (auto entity : worldView)
			{
				auto entityID = stream.Read<entt::entity>();
				worldView.get<components::Position>(entityID).Read(stream);
				worldView.get<components::Rotation>(entityID).Read(stream);
				worldView.get<components::Velocity>(entityID).Read(stream);
			}*/
		}

	private:
		serialization::OutputMemoryStream m_stream;
		engine::World* m_world;
	};

	EntitySerializer::EntitySerializer() : m_impl(std::make_unique<EntitySerializerImpl>())
	{
	}

	EntitySerializer::~EntitySerializer()
	{
		m_impl = nullptr;
	}

	void EntitySerializer::Update(std::chrono::milliseconds deltaTime, engine::World& world)
	{
		m_impl->Update(deltaTime, world);
	}

	void EntitySerializer::EndFrame()
	{
		m_impl->EndFrame();
	}
}