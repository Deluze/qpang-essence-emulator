#pragma once

#include <functional>
#include <thread>

class TimeHelper
{
public:
	// Templated member function bodies can't be put in .cpp files, if you don't use explicit template specialisation.
	// That's why this is in the header file :P
	template <typename... ParamTypes>
	static void setTimeOut(int milliseconds, std::function<void(ParamTypes...)> func, ParamTypes... parames)
	{
		std::thread t([=]()
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
				func(parames...);
			});

		t.detach();
	};
};