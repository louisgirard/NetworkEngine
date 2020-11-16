#pragma once

#include <nano_engine/common.hpp>
#include <nano_engine/entities/actor.hpp>

#include <nano_engine/components/rigid_body.hpp>
#include <nano_engine/components/colliders/box_collider.hpp>
#include <nano_engine/components/replication_component.hpp>

using namespace nano_engine;

class Cube : public entities::Actor
{
	using Base = entities::Actor;
public:
	REPLICATED('CUBE', Cube);

	Cube(std::weak_ptr<engine::World> world, const std::string& name, float size);

	virtual void SetPosition(float x, float y, float z) override;

	virtual void Write(serialization::OutputMemoryStream& stream) const override;
	virtual void Read(serialization::InputMemoryStream& stream) override;

private:
	float m_size;

	Reference<components::BoxCollider> m_collider;
	Reference<components::RigidBody> m_rigidBody;

	Reference<components::ReplicationComponent> m_replication;
};