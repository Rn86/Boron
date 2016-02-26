#ifndef BORON_ENGINE_HPP_INCLUDED
#define BORON_ENGINE_HPP_INCLUDED

#include <Boron/Fiber.hpp>

namespace Boron
{
	class Engine
	{
	public:
		Engine();
		Engine(Fiber::Flags flags);
		Engine(const Engine & engine);
		Engine(const Engine && engine) noexcept;

		Engine & operator=(const Engine & engine);
		Engine & operator=(const Engine && engine) noexcept;

	private:
		Fiber AddFiber(const Fiber & fiber) const;

	public:
		Fiber AddFiber(size_t szStack, const FiberCallback & callback);
		Fiber AddFiber(size_t szStackCommit, size_t szStackReserve, Fiber::Flags flags, const FiberCallback & callback);
		void RemoveFiber(const Fiber & fiber);

		Fiber GetPrimeFiber() const;

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;
	};
}

#endif //!BORON_ENGINE_HPP_INCLUDED