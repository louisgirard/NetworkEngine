#include <nano_engine/entities/sphere.hpp>

namespace nano_engine::entities
{
	Sphere::Sphere(engine::World& world, const std::string& name, float radius) :
		Base(world, name),
		m_radius(radius)
	{
		m_collider = AddComponent<components::SphereCollider>(m_radius);
		m_rigidBody = AddComponent<components::RigidBody>(m_collider.Get(), 1.0f, GetPosition());
		m_replication = AddComponent<components::ReplicationComponent>(this);
	}

	void Sphere::SetPosition(float x, float y, float z)
	{
		Base::SetPosition(x, y, z);
		m_rigidBody.Get().SetPosition(x, y, z);
	}

	void Sphere::Write(serialization::OutputMemoryStream& stream) const
	{
		Base::Write(stream);
		stream.Write(m_radius);
	}

	void Sphere::Read(serialization::InputMemoryStream& stream)
	{
		Base::Read(stream);
		m_radius = stream.Read<float>();
		// TODO: update collider
	}
}
