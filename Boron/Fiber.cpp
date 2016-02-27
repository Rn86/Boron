#include <Boron/Fiber.hpp>
#include <Boron/LocalManager.hpp>

#include <exception>

#include <windows.h>

namespace Boron
{
	static DWORD MakeFlags(FiberFlags flags)
	{
		return (flags & FiberFlags::FloatSwitch) ? FIBER_FLAG_FLOAT_SWITCH : 0;
	}

	/*
	 * Fiber::Impl
	 */
	struct Fiber::Impl
	{
	private:
		struct FiberDispatcher;

	public:
		Impl(FiberUID uid, LPVOID handle)
			: m_uid(uid),
			m_handle(handle)
		{
			if (!m_handle)
				throw std::exception("Fiber handle is nullptr");
		}

		Impl(FiberUID uid)
			: Impl(uid, ConvertThreadToFiber(this))
		{
		}

		Impl(FiberUID uid, FiberFlags flags)
			: Impl(uid, ConvertThreadToFiberEx(this, MakeFlags(flags)))
		{
		}

		Impl(FiberUID uid, const FiberCallback & callback, size_t szStack)
			: Impl(uid, CreateFiber(szStack, (LPFIBER_START_ROUTINE)FiberDispatcher::Dispatch, new FiberDispatcher(callback)))
		{
		}

		Impl(FiberUID uid, FiberFlags flags, const FiberCallback & callback, size_t szStackCommit, size_t szStackReserve)
			: Impl(uid, CreateFiberEx(szStackCommit, szStackReserve, MakeFlags(flags), (LPFIBER_START_ROUTINE)FiberDispatcher::Dispatch, new FiberDispatcher(callback)))
		{
		}

		void Switch() const
		{
			SwitchToFiber(m_handle);
		}

		FiberUID GetUID() const
		{
			return m_uid;
		}

	private:
		struct FiberDispatcher
		{
		public:
			FiberDispatcher(const FiberCallback & callback)
				: m_callback(callback)
			{
			}

			static void Dispatch(FiberDispatcher * pDispatcher)
			{
				if (pDispatcher)
				{
					Manager manager = ManagerInstance::GetInstance();
					pDispatcher->m_callback(manager);
					delete pDispatcher;
					manager.SwitchToPrime();
				}
			}

		private:
			FiberCallback m_callback;
		};

	private:
		FiberUID m_uid;
		LPVOID m_handle;
	};

	/*
	 * Fiber
	 */
	Fiber::Fiber(FiberUID uid)
		: m_pImpl(new Impl(uid))
	{
	}

	Fiber::Fiber(FiberUID uid, FiberFlags flags)
		: m_pImpl(new Impl(uid, flags))
	{
	}

	Fiber::Fiber(FiberUID uid, const FiberCallback & callback, size_t szStack)
		: m_pImpl(new Impl(uid, callback, szStack))
	{
	}

	Fiber::Fiber(FiberUID uid, FiberFlags flags, const FiberCallback & callback, size_t szStackCommit, size_t szStackReserve)
		: m_pImpl(new Impl(uid, flags, callback, szStackCommit, szStackReserve))
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

	void Fiber::Switch() const
	{
		m_pImpl->Switch();
	}

	FiberUID Fiber::GetUID() const
	{
		return m_pImpl->GetUID();
	}
}