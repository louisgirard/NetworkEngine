#include <atomic>
#include <nano_engine/engine.hpp>

namespace nano_engine
{
	class EngineImpl
	{
	public:
		EngineImpl()
		{

		}

		~EngineImpl()
		{
			Stop(); 
		}

		void ParseCommandLine(int argc, char* argv[])
		{

		}

		void Run()
		{
			if (!m_stop) return;

			m_stop = false;
			while (!m_stop)
			{

			}
		}

		void Stop()
		{
			m_stop = true;
		}

	private:
		std::atomic_bool m_stop = true;
	};

	Engine::Engine() : m_impl(std::make_unique<EngineImpl>())
	{

	}

	Engine::~Engine()
	{
		m_impl = nullptr;
	}

	void Engine::ParseCommandLine(int argc, char* argv[])
	{
		m_impl->ParseCommandLine(argc, argv);
	}

	void Engine::Run()
	{
		m_impl->Run();
	}

	void Engine::Stop()
	{
		m_impl->Stop();
	}
}