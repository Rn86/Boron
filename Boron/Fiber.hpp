#ifndef BORON_FIBER_HPP_INCLDUED
#define BORON_FIBER_HPP_INCLDUED

#include <memory>
#include <functional>

namespace Boron
{
	typedef size_t FiberDescriptor;
	typedef std::function<void(void)> FiberCallback;

	class Fiber
	{
	public:
		enum Flags
		{
			FloatSwitch
		};

	private:
		Fiber();
		Fiber(Flags flags);

	public:
		Fiber(size_t szStack, const FiberCallback & callback, const Fiber & prime);
		Fiber(size_t szStackCommit, size_t szStackReserve, Flags flags, const FiberCallback & callback, const Fiber & prime);
		Fiber(const Fiber & fiber);
		Fiber(const Fiber && fiber) noexcept;

		Fiber & operator=(const Fiber & fiber);
		Fiber & operator=(const Fiber && fiber) noexcept;

		FiberDescriptor GetDescriptor() const;

		void Switch() const;

	private:
		friend class Engine;

	private:
		struct Impl;
		std::shared_ptr<Impl> m_pImpl;
	};
}

#endif //!BORON_FIBER_HPP_INCLDUED