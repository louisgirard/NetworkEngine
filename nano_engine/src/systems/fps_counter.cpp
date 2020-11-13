#include <iostream>

#include <spdlog/spdlog.h>

#include <nano_engine/systems/fps_counter.hpp>

#include <nano_engine/engine/engine.hpp>
#include <nano_engine/engine/world.hpp>

namespace nano_engine::systems
{
	class FPSCounterImpl
	{
	public:
		void Update(std::chrono::microseconds deltaTime, engine::World& world)
		{
			m_frames++;
			m_totalMilli += deltaTime;

			if (m_totalMilli >= 1s)
			{
				spdlog::debug("FPS : {}", m_frames);
				m_frames = 0;
				m_totalMilli = 0ms;
			}
		}

	private:
		int m_frames = 0;
		std::chrono::microseconds m_totalMilli = 0ms;
	};


	FPSCounter::FPSCounter()
	{
		m_impl = std::make_unique<FPSCounterImpl>();
	}

	FPSCounter::~FPSCounter()
	{
		m_impl = nullptr;
	}

	void FPSCounter::Update(std::chrono::microseconds deltaTime, engine::World& world)
	{
		m_impl->Update(deltaTime, world);
	}

}