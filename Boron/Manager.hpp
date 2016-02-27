#ifndef BORON_MANAGER_HPP_INCLUDED
#define BORON_MANAGER_HPP_INCLUDED

#include <memory>
#include <functional>

namespace Boron
{
	class Manager;
	class Fiber;

	typedef size_t FiberUID;
	typedef std::function<void(Manager &)> FiberCallback;

	enum FiberFlags
	{
		FloatSwitch
	};

	class Manager
	{
	private:
		Manager();
		Manager(FiberFlags flags);

	public:
		Manager(const Manager & engine);
		Manager(const Manager && engine) noexcept;

		Manager & operator=(const Manager & engine);
		Manager & operator=(const Manager && engine) noexcept;

		Fiber AddFiber(FiberUID uid, const FiberCallback & callback, size_t szStack = 0);
		Fiber AddFiber(FiberUID uid, FiberFlags flags, const FiberCallback & callback, size_t szStackCommit = 0, size_t szStackReserve = 0);
		
		Fiber GetFiber(FiberUID uid) const;
		bool ContainsFiber(FiberUID uid) const;

		void RemoveFiber(FiberUID uid);
		void RemoveFiber(const Fiber & fiber);

		void SwitchToPrime() const;
		void SwitchTo(FiberUID uid) const;
		void SwitchTo(const Fiber & fiber) const;

	public:
		static Manager Create();
		static Manager Create(FiberFlags flags);

	private:
		struct Impl;
		Manager(const std::shared_ptr<Impl> & pImpl);
		std::shared_ptr<Impl> m_pImpl;
	};
}

#endif //!BORON_MANAGER_HPP_INCLUDED