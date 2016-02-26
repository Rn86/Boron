#include <Boron/Fiber.hpp>

#include <exception>

#include <windows.h>

namespace Boron
{
	static FiberDescriptor MakeDescriptor(LPVOID handle)
	{
		return (FiberDescriptor)handle;
	}

	static DWORD MakeFlags(Fiber::Flags flags)
	{
		return (flags & Fiber::Flags::FloatSwitch) ? FIBER_FLAG_FLOAT_SWITCH : 0;
	}

	struct Fiber::Impl
	{
	private:
		struct FiberDispatcher;

	public:
		Impl(LPVOID handle)
			: m_handle(handle),
			m_descriptor(MakeDescriptor(handle))
		{
			if (!m_handle)
				throw std::exception("Fiber handle is nullptr");
		}

		Impl()
			: Impl(ConvertThreadToFiber(this))
		{
		}

		Impl(Flags flags)
			: Impl(ConvertThreadToFiberEx(this, MakeFlags(flags)))
		{
		}

		Impl(size_t szStack, const FiberCallback & callback, const Fiber & prime)
			: Impl(CreateFiber(szStack, (LPFIBER_START_ROUTINE)FiberDispatcher::Dispatch, new FiberDispatcher(callback, prime)))
		{
		}

		Impl(size_t szStackCommit, size_t szStackReserve, Flags flags, const FiberCallback & callback, const Fiber & prime)
			: Impl(CreateFiberEx(szStackCommit, szStackReserve, MakeFlags(flags), (LPFIBER_START_ROUTINE)FiberDispatcher::Dispatch, new FiberDispatcher(callback, prime)))
		{
		}

	private:
		struct FiberDispatcher
		{
		public:
			FiberDispatcher(const FiberCallback & callback, const Fiber & prime)
				: m_callback(callback),
				m_prime(prime)
			{
			}

			static void Dispatch(FiberDispatcher * pDispatcher)
			{
				if (pDispatcher)
				{
					pDispatcher->m_callback();
					Fiber prime = pDispatcher->m_prime;
					delete pDispatcher;
					prime.Switch();
				}
			}

		private:
			FiberCallback m_callback;
			Fiber m_prime;
		};

	public:
		LPVOID m_handle;
		FiberDescriptor m_descriptor;
	};

	Fiber::Fiber()
		: m_pImpl(new Impl())
	{
	}

	Fiber::Fiber(Flags flags)
		: m_pImpl(new Impl(flags))
	{
	}

	Fiber::Fiber(size_t szStack, const FiberCallback & callback, const Fiber & prime)
		: m_pImpl(new Impl(szStack, callback, prime))
	{
	}

	Fiber::Fiber(size_t szStackCommit, size_t szStackReserve, Flags flags, const FiberCallback & callback, const Fiber & prime)
		: m_pImpl(new Impl(szStackCommit, szStackReserve, flags, callback, prime))
	{
	}

	Fiber::Fiber(const Fiber & fiber)
		: m_pImpl(fiber.m_pImpl)
	{
	}

	Fiber::Fiber(const Fiber && fiber) noexcept
		: m_pImpl(std::move(fiber.m_pImpl))
	{
	}

	Fiber & Fiber::operator=(const Fiber & fiber)
	{
		m_pImpl = fiber.m_pImpl;
		return *this;
	}

	Fiber & Fiber::operator=(const Fiber && fiber) noexcept
	{
		m_pImpl = std::move(fiber.m_pImpl);
		return *this;
	}

	FiberDescriptor Fiber::GetDescriptor() const
	{
		return m_pImpl->m_descriptor;
	}

	void Fiber::Switch() const
	{
		SwitchToFiber(m_pImpl->m_handle);
	}
}