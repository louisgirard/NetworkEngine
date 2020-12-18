#include <nano_engine/entities/cube.hpp>

namespace nano_engine::entities
{
	Cube::Cube(engine::World& world, const std::string& name, float size) :
		Base(world, name),
		m_size(size)
	{
		m_replicate = true;
		m_collider = std::make_shared<components::BoxCollider>(m_size, m_size, m_size);
		m_rigidBody = std::make_shared<components::RigidBody>(*m_collider.get(), 0.0f, GetPosition());
	}

	void Cube::SetPosition(float x, float y, float z)
	{
		Base::SetPosition(x, y, z);
		m_rigidBody->SetPosition(x, y, z);
	}

	void Cube::Write(serialization::OutputMemoryStream& stream) const
	{
		Base::Write(stream);
		stream.Write(m_size);
	}

	void Cube::Read(serialization::InputMemoryStream& stream)
	{
		Base::Read(stream);
		m_size = stream.Read<float>();
		// TODO: update collider
	}
}
