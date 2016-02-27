#ifndef BORON_MANAGER_IMPL_HPP_INCLUDED
#define BORON_MANAGER_IMPL_HPP_INCLUDED

#include <Boron/Manager.hpp>

namespace Boron
{
	struct ManagerInstance
	{
	public:
		static Manager GetInstance();
		static void SetInstance(const std::function<Manager()> & accessor);

	private:
		static Manager GetSetInstance(const std::function<Manager()> & accessor);
	};
}

#endif //!BORON_MANAGER_IMPL_HPP_INCLUDED