#include <nano_engine/entities/actor.hpp>

namespace nano_engine::entities
{
	Actor::Actor(std::weak_ptr<engine::World> world, const std::string& name) : BaseClass(world, name)
	{
		m_position = AddComponent<components::Position>(0.f, 0.f, 0.f);
		m_rotation = AddComponent<components::Rotation>(0.f, 0.f, 0.f, 0.f);
		m_scale = AddComponent<components::Scale>(0.f, 0.f, 0.f);
	}

	Actor::~Actor()
	{

	}

	void Actor::Write(serialization::OutputMemoryStream& stream) const
	{
		BaseClass::Write(stream);
		m_position.Get().Write(stream);
		m_rotation.Get().Write(stream);
		m_scale.Get().Write(stream);
	}

	void Actor::Read(serialization::InputMemoryStream& stream)
	{
		BaseClass::Read(stream);
		m_position.Get().Read(stream);
		m_rotation.Get().Read(stream);
		m_scale.Get().Read(stream);
	}
}