#include "stdafx.h"
#include "CppUnitTest.h"

#include <Boron/Engine.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BoronTest
{		
	TEST_CLASS(BoronTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Boron::Engine engine;

			std::function<void(void)> func_A([]()
			{
				// fiber_A

				// Unnecessary to switch to prime fiber,
				// it is done internaly.
			});

			Boron::Fiber fiber_A = engine.AddFiber(0, func_A);

			std::function<void(void)> func_B([&fiber_A]()
			{
				// fiber_B

				// Switch to fiber_A
				fiber_A.Switch();
			});

			Boron::Fiber fiber_B = engine.AddFiber(0, func_B);

			// Switch to fiber_B
			fiber_B.Switch();
		}

	};
}