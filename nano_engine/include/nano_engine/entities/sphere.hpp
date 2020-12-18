#pragma once

#include <nano_engine/common.hpp>
#include <nano_engine/engine/entity.hpp>

#include <nano_engine/components/rigid_body.hpp>
#include <nano_engine/components/colliders/sphere_collider.hpp>

namespace nano_engine::entities
{
	class Sphere : public engine::Entity
	{
		using Base = engine::Entity;
	public:
		REPLICATED('SPHE', Sphere);

		Sphere(engine::World& world, const std::string& name, float radius);

		virtual void SetPosition(float x, float y, float z) override;

		virtual void Write(serialization::OutputMemoryStream& stream) const override;
		virtual void Read(serialization::InputMemoryStream& stream) override;

		virtual components::RigidBody* GetRigidBody() override { return m_rigidBody.get(); }
	private:
		float m_radius;

		std::shared_ptr<components::SphereCollider> m_collider;
		std::shared_ptr<components::RigidBody> m_rigidBody;
	};
}
