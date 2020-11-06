#include <cube.hpp>

struct CubeState
{
	nano_engine::components::BoxCollider& m_boxCollider;
	nano_engine::components::RigidBody& m_rigidBody;

	CubeState(nano_engine::components::BoxCollider& boxCollider, nano_engine::components::RigidBody& rigidBody)
		: m_boxCollider(boxCollider), m_rigidBody(rigidBody)
	{

	}
};

class CubeImpl : public Entity
{
public:
	CubeImpl(const Cube& cube, float size) : Entity(cube)
	{
		auto& boxCollider = AddComponent<nano_engine::components::BoxCollider>(size, size, size);
		auto& rigidBody = AddComponent<nano_engine::components::RigidBody>(boxCollider, 0.f, Position());
	
		m_cubeState = std::make_shared<CubeState>(boxCollider, rigidBody);
	}

	void SetPosition(float x, float y, float z)
	{
		m_cubeState->m_rigidBody.Position(x, y, z);
		Position().x = x;
		Position().y = y;
		Position().z = z;
	}
private:
	std::shared_ptr<CubeState> m_cubeState;
};

Cube::Cube(World& world, const std::string& name, float size) : Entity(world, name)
{
	m_impl = std::make_unique<CubeImpl>(*this, size);
}

Cube::~Cube()
{
	m_impl = nullptr;
}

void Cube::SetPosition(float x, float y, float z)
{
	m_impl->SetPosition(x, y, z);
}
