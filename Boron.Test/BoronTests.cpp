#include "stdafx.h"
#include "CppUnitTest.h"

#include <Boron/Fiber.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Boron;

namespace BoronTest
{		
	enum FiberUIDs
	{
		fiber_A,
		fiber_B,
		fiber_C,
	};

	TEST_CLASS(BoronTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Manager manager = Manager::Create();

			manager.AddFiber(fiber_A, [](Manager & manager)
			{
				// fiber_A

				// Switch to fiber_B
				manager.SwitchTo(fiber_B);
			});

			manager.AddFiber(fiber_B, [](Manager & manager)
			{
				// fiber_B

				// Switch to prime fiber
				manager.SwitchToPrime();
			});

			manager.AddFiber(fiber_C, [](Manager &)
			{
				// fiber_C

				// Unnecessary to switch to prime fiber,
				// it is done internaly.
			});

			// Switch to fiber_BB
			manager.SwitchTo(fiber_B);
			manager.SwitchTo(fiber_C);
		}

	};
}