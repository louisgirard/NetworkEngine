#pragma once

#include <memory>

namespace nano_engine
{
	class EngineImpl;
	class Engine
	{
	public:
		static Engine& Instance()
		{
			static Engine ms_engine;
			return ms_engine;
		}

		~Engine();
		Engine(const Engine& other) = delete;
		Engine(Engine&& other) = delete;

		void Engine::ParseCommandLine(int argc, char* argv[]);
		void Engine::Run();
		void Engine::Stop();

	private:
		Engine();

		std::unique_ptr<EngineImpl> m_impl;
	};
}