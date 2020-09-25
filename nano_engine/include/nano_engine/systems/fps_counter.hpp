#pragma once

#include <nano_engine/common.hpp>
#include <nano_engine/systems/system.hpp>

namespace nano_engine::systems
{
	class FPSCounterImpl;
	class FPSCounter : public ISystem
	{
	public:
		FPSCounter();
		virtual ~FPSCounter();

		FPSCounter(const FPSCounter& other) = delete;
		FPSCounter(FPSCounter&& other) = delete;

		void Update(std::chrono::milliseconds deltaTime) override;

	private:
		std::unique_ptr<FPSCounterImpl> m_impl;
	};
}