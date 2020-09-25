#include <iostream>
#include <nano_engine/systems/fps_counter.hpp>

namespace nano_engine::systems
{
	class FPSCounterImpl
	{
	public:
		void Update(std::chrono::milliseconds deltaTime)
		{
			m_frames++;
			m_totalMilli += deltaTime;

			if (m_totalMilli >= 1s)
			{
				std::cout << "FPS : " << m_frames << std::endl;
				m_frames = 0;
				m_totalMilli = 0ms;
			}
		}

	private:
		int m_frames = 0;
		std::chrono::milliseconds m_totalMilli = 0ms;
	};

	FPSCounter::FPSCounter() : m_impl(std::make_unique<FPSCounterImpl>())
	{
	}

	FPSCounter::~FPSCounter()
	{
		m_impl = nullptr;
	}

	void FPSCounter::Update(std::chrono::milliseconds deltaTime)
	{
		m_impl->Update(deltaTime);
	}
}