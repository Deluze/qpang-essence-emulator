#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>

//std
#include <functional>
#include <memory>

//Credits: Jeroen Bollen
template<typename Event>
class EventThreadPool
{
public:
	using Event_ptr = Event*;
	using EventHandler = std::function<void(Event_ptr)>;
	EventThreadPool(const size_t numThreads, const EventHandler& eventHandler) :
		_eventHandler{ eventHandler },
		threadCount{ numThreads },
		_running{ false },
		_work{ _ioService }
	{

	}

	void start()
	{
		if (!threadCount)
		{
			//todo: better exceptions...
			throw "invalid thread count";
			return;
		}
		if (_running)
		{
			throw "threads in this pool are already running";
			return;
		}

		for (size_t i{ 0 }; i < threadCount; ++i)
		{
			_threadGroup.create_thread(boost::bind(&EventThreadPool::Worker, this));
		}

		_running = true;
	}

	void stop()
	{
		_running = false;
		_ioService.stop();
		_threadGroup.join_all();
	}


	void post(Event_ptr evt)
	{
		_ioService.post(std::bind(_eventHandler, evt));
	}


	size_t threadCount;
protected:
	void Worker()
	{
		//for now
		_ioService.run();
	}

private:
	bool _running;
	boost::asio::io_service _ioService;
	boost::thread_group _threadGroup;
	boost::asio::io_service::work _work;
	EventHandler _eventHandler;
};
