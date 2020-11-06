#pragma once

#include <nano_engine/engine/entity.hpp>
#include <nano_engine/components/rigid_body.hpp>

#include <nano_engine/components/colliders/sphere_collider.hpp>

using namespace nano_engine::engine;

class SphereImpl;
class Sphere : public Entity
{
public:
	Sphere(World& world, const std::string& name, float size);
	~Sphere();

	void SetPosition(float x, float y, float z);

private:
	std::unique_ptr<SphereImpl> m_impl;
};