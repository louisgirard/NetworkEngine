#include <sphere.hpp>

struct SphereState
{
	nano_engine::components::SphereCollider& m_sphereCollider;
	nano_engine::components::RigidBody& m_rigidBody;

	SphereState(nano_engine::components::SphereCollider& sphereCollider, nano_engine::components::RigidBody& rigidBody)
		: m_sphereCollider(sphereCollider), m_rigidBody(rigidBody)
	{

	}
};

class SphereImpl : public Entity
{
public:
	SphereImpl(const Sphere& sphere, float size) : Entity(sphere)
	{
		auto& boxCollider = AddComponent<nano_engine::components::SphereCollider>(size);
		auto& rigidBody = AddComponent<nano_engine::components::RigidBody>(boxCollider, 1.f, Position());

		m_sphereState = std::make_shared<SphereState>(boxCollider, rigidBody);
	}

	void SetPosition(float x, float y, float z)
	{
		m_sphereState->m_rigidBody.Position(x, y, z);
		Position().x = x;
		Position().y = y;
		Position().z = z;
	}
private:
	std::shared_ptr<SphereState> m_sphereState;
};

Sphere::Sphere(World& world, const std::string& name, float size) : Entity(world, name)
{
	m_impl = std::make_unique<SphereImpl>(*this, size);
}

Sphere::~Sphere()
{
	m_impl = nullptr;
}

void Sphere::SetPosition(float x, float y, float z)
{
	m_impl->SetPosition(x, y, z);
}
