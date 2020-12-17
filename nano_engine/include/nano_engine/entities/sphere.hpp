#pragma once

#include <nano_engine/common.hpp>
#include <nano_engine/entities/actor.hpp>

#include <nano_engine/components/rigid_body.hpp>
#include <nano_engine/components/colliders/sphere_collider.hpp>
#include <nano_engine/components/replication_component.hpp>

namespace nano_engine::entities
{
	class Sphere : public Actor
	{
		using Base = Actor;
	public:
		REPLICATED('SPHE', Sphere);

		Sphere(engine::World& world, const std::string& name, float radius);

		virtual void SetPosition(float x, float y, float z) override;

		virtual void Write(serialization::OutputMemoryStream& stream) const override;
		virtual void Read(serialization::InputMemoryStream& stream) override;

	private:
		float m_radius;

		Reference<components::SphereCollider> m_collider;
		Reference<components::RigidBody> m_rigidBody;

		Reference<components::ReplicationComponent> m_replication;
	};
}
