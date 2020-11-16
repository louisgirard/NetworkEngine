#include <cube.hpp>

Cube::Cube(std::weak_ptr<engine::World> world, const std::string& name, float size) :
	Base(world, name),
	m_size(size)
{
	m_collider = AddComponent<components::BoxCollider>(m_size, m_size, m_size);
	m_rigidBody = AddComponent<components::RigidBody>(m_collider.Get(), 0.0f, GetPosition());
	m_replication = AddComponent<components::ReplicationComponent>(this);
}

void Cube::SetPosition(float x, float y, float z)
{
	Base::SetPosition(x, y, z);
	m_rigidBody.Get().SetPosition(x, y, z);
}

void Cube::Write(serialization::OutputMemoryStream& stream) const
{
	stream.Write(m_size);
}

void Cube::Read(serialization::InputMemoryStream& stream)
{
	m_size = stream.Read<float>();
	// TODO: update collider
}
