#include <Boron/Fiber.hpp>
#include <Boron/LocalManager.hpp>

#include <map>

namespace Boron
{
	/*
	 * Manager::Impl
	 */
	struct Manager::Impl
	{
	public:
		Impl()
			: m_prime(Fiber((FiberUID)-1))
		{
		}

		Impl(FiberFlags flags)
			: m_prime(Fiber((FiberUID)-1, flags))
		{
		}

	private:
		Fiber AddFiber(FiberUID uid, const Fiber & fiber)
		{
			m_fibers.emplace(uid, fiber);
			return fiber;
		}

	public:
		Fiber AddFiber(FiberUID uid, const FiberCallback & callback, size_t szStack)
		{
			return AddFiber(uid, Fiber(uid, callback, szStack));
		}

		Fiber AddFiber(FiberUID uid, FiberFlags flags, const FiberCallback & callback, size_t szStackCommit, size_t szStackReserve)
		{
			return AddFiber(uid, Fiber(uid, flags, callback, szStackCommit, szStackReserve));
		}

		Fiber GetFiber(FiberUID uid) const
		{
			return m_fibers.at(uid);
		}

		bool ContainsFiber(FiberUID uid) const
		{
			return m_fibers.find(uid) != m_fibers.end();
		}

		void RemoveFiber(FiberUID uid)
		{
			m_fibers.erase(uid);
		}

		void RemoveFiber(const Fiber & fiber)
		{
			m_fibers.erase(fiber.GetUID());
		}

		void SwitchToPrime() const
		{
			m_prime.Switch();
		}

		void SwitchTo(FiberUID uid) const
		{
			m_fibers.at(uid).Switch();
		}

		void SwitchTo(const Fiber & fiber) const
		{
			m_fibers.at(fiber.GetUID()).Switch();
		}

	private:
		Fiber m_prime;
		std::map<FiberUID, Fiber> m_fibers;
	};

	/*
	 * Manager
	 */
	Manager::Manager(const std::shared_ptr<Impl> & pImpl)
		: m_pImpl(pImpl)
	{
	}

	Manager::Manager()
		: Manager(std::make_shared<Impl>())
	{
	}

	Manager::Manager(FiberFlags flags)
		: Manager(std::make_shared<Impl>(flags))
	{
	}

	Manager::Manager(const Manager & manager)
		: m_pImpl(manager.m_pImpl)
	{
	}

	Manager::Manager(const Manager && manager) noexcept
		: m_pImpl(std::move(manager.m_pImpl))
	{
	}

	Manager & Manager::operator=(const Manager & manager)
	{
		m_pImpl = manager.m_pImpl;
		return *this;
	}

	Manager & Manager::operator=(const Manager && manager) noexcept
	{
		m_pImpl = std::move(manager.m_pImpl);
		return *this;
	}

	Fiber Manager::AddFiber(FiberUID uid, const FiberCallback & callback, size_t szStack)
	{
		return m_pImpl->AddFiber(uid, callback, szStack);
	}

	Fiber Manager::AddFiber(FiberUID uid, FiberFlags flags, const FiberCallback & callback, size_t szStackCommit, size_t szStackReserve)
	{
		return m_pImpl->AddFiber(uid, flags, callback, szStackCommit, szStackReserve);
	}

	Fiber Manager::GetFiber(FiberUID uid) const
	{
		return m_pImpl->GetFiber(uid);
	}

	bool Manager::ContainsFiber(FiberUID uid) const
	{
		return m_pImpl->ContainsFiber(uid);
	}

	void Manager::RemoveFiber(FiberUID uid)
	{
		m_pImpl->RemoveFiber(uid);
	}

	void Manager::RemoveFiber(const Fiber & fiber)
	{
		m_pImpl->RemoveFiber(fiber);
	}

	void Manager::SwitchToPrime() const
	{
		m_pImpl->SwitchToPrime();
	}

	void Manager::SwitchTo(FiberUID uid) const
	{
		m_pImpl->SwitchTo(uid);
	}

	void Manager::SwitchTo(const Fiber & fiber) const
	{
		m_pImpl->SwitchTo(fiber);
	}

	Manager Manager::Create()
	{
		ManagerInstance::SetInstance([]()
		{
			return Manager();
		});
		return ManagerInstance::GetInstance();
	}

	Manager Manager::Create(FiberFlags flags)
	{
		ManagerInstance::SetInstance([flags]()
		{
			return Manager(flags);
		});
		return ManagerInstance::GetInstance();
	}

	/*
	 * ManagerInstance
	 */
	Manager ManagerInstance::GetInstance()
	{
		return GetSetInstance(nullptr);
	}

	void ManagerInstance::SetInstance(const std::function<Manager()> & accessor)
	{
		GetSetInstance(accessor);
	}

	Manager ManagerInstance::GetSetInstance(const std::function<Manager()> & accessor)
	{
		static thread_local bool assign = false;
		static thread_local Manager s_manager = accessor();
		if (assign && accessor != nullptr) s_manager = accessor();
		else if (!assign) assign = true;
		return s_manager;
	}
}