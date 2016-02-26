#include <Boron/Engine.hpp>

#include <map>

namespace Boron
{
	struct Engine::Impl
	{
		Impl()
			: m_prime()
		{
		}

		Impl(Fiber::Flags flags)
			: m_prime(flags)
		{
		}

		Fiber m_prime;
		std::map<FiberDescriptor, Fiber> m_fibers;
	};

	Engine::Engine()
		: m_pImpl(new Impl())
	{
	}

	Engine::Engine(Fiber::Flags flags)
		: m_pImpl(new Impl(flags))
	{
	}

	Engine::Engine(const Engine & engine)
		: m_pImpl(engine.m_pImpl)
	{
	}

	Engine::Engine(const Engine && engine) noexcept
		: m_pImpl(std::move(engine.m_pImpl))
	{
	}

	Engine & Engine::operator=(const Engine & engine)
	{
		m_pImpl = engine.m_pImpl;
		return *this;
	}

	Engine & Engine::operator=(const Engine && engine) noexcept
	{
		m_pImpl = std::move(engine.m_pImpl);
		return *this;
	}

	Fiber Engine::AddFiber(const Fiber & fiber) const
	{
		FiberDescriptor descriptor = fiber.GetDescriptor();
		m_pImpl->m_fibers.emplace(descriptor, fiber);
		return fiber;
	}

	Fiber Engine::AddFiber(size_t szStack, const FiberCallback & callback)
	{
		Fiber fiber(szStack, callback, GetPrimeFiber());
		AddFiber(fiber);
		return fiber;
	}

	Fiber Engine::AddFiber(size_t szStackCommit, size_t szStackReserve, Fiber::Flags flags, const FiberCallback & callback)
	{
		Fiber fiber(szStackCommit, szStackReserve, flags, callback, GetPrimeFiber());
		AddFiber(fiber);
		return fiber;
	}

	void Engine::RemoveFiber(const Fiber & fiber)
	{
		m_pImpl->m_fibers.erase(fiber.GetDescriptor());
	}

	Fiber Engine::GetPrimeFiber() const
	{
		return m_pImpl->m_prime;
	}
}