#ifndef BORON_FIBER_HPP_INCLDUED
#define BORON_FIBER_HPP_INCLDUED

#include <Boron/Manager.hpp>

namespace Boron
{
	class Fiber
	{
	private:
		Fiber(FiberUID uid);
		Fiber(FiberUID uid, FiberFlags flags);
		Fiber(FiberUID uid, const FiberCallback & callback, size_t szStack);
		Fiber(FiberUID uid, FiberFlags flags, const FiberCallback & callback, size_t szStackCommit, size_t szStackReserve);

	public:
		Fiber(const Fiber & fiber);
		Fiber(const Fiber && fiber) noexcept;

		Fiber & operator=(const Fiber & fiber);
		Fiber & operator=(const Fiber && fiber) noexcept;

	public:
		void Switch() const;
		FiberUID GetUID() const;

	private:
		friend Manager;

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;
	};
}

#endif //!BORON_FIBER_HPP_INCLDUED