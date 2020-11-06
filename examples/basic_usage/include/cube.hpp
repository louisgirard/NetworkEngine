#pragma once

#include <nano_engine/engine/entity.hpp>
#include <nano_engine/components/rigid_body.hpp>

#include <nano_engine/components/colliders/box_collider.hpp>

using namespace nano_engine::engine;

class CubeImpl;
class Cube : public Entity
{
public:
	Cube(World& world, const std::string& name, float size);
	~Cube();

	void SetPosition(float x, float y, float z);

private:
	std::unique_ptr<CubeImpl> m_impl;
};