#include <nano_engine/entities/sphere.hpp>

namespace nano_engine::entities
{
	Sphere::Sphere(engine::World& world, const std::string& name, float radius) :
		Base(world, name),
		m_radius(radius)
	{
		m_replicate = true;
		m_collider = std::make_shared<components::SphereCollider>(m_radius);
		m_rigidBody = std::make_shared<components::RigidBody>(*m_collider.get(), 1.0f, GetPosition());
	}

	void Sphere::SetPosition(float x, float y, float z)
	{
		Base::SetPosition(x, y, z);
		m_rigidBody->SetPosition(x, y, z);
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
