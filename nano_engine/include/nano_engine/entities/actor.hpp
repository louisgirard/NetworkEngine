#pragma once

#include <type_traits>

#include <nano_engine/common.hpp>

#include <nano_engine/engine/entity.hpp>

#include <nano_engine/components/position.hpp>
#include <nano_engine/components/rotation.hpp>
#include <nano_engine/components/scale.hpp>

namespace nano_engine::entities
{
	class Actor : public engine::Entity
	{
		using BaseClass = Entity;
	public:
		Actor(std::weak_ptr<engine::World> world, const std::string& name);
		~Actor();

		virtual void Write(serialization::OutputMemoryStream& stream) const override;
		virtual void Read(serialization::InputMemoryStream& stream) override;

		const components::Position& GetPosition() const { return *m_position; }
		virtual void SetPosition(float x, float y, float z);

		components::Rotation& GetRotation() { return *m_rotation; }
		const components::Rotation& GetRotation() const { return *m_rotation; }

		components::Scale& GetScale() { return *m_scale; }
		const components::Scale& GetScale() const { return *m_scale; }

	private:
		Reference<components::Position> m_position;
		Reference<components::Rotation> m_rotation;
		Reference<components::Scale> m_scale;
	};
}