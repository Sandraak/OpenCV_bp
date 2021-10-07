#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>

namespace utility
{
	template<typename T>
	class Timer
	{
	public:
	    Timer(T interval = (T)0)
		    : interval(interval)
		{
	        Reset();
	    }

	    auto operator()() const
		{
	        return std::chrono::duration_cast<T>(std::chrono::steady_clock::now() - previous).count();
	    }

	    auto Time() const
	    {
	        return (*this)();
	    }

		void Reset()
		{
	        previous = std::chrono::steady_clock::now();
	    }

	    bool Finished() const
	    {
	        return std::chrono::duration_cast<T>(std::chrono::steady_clock::now() - previous) >= interval;
	    }


	private:
	    T interval;
	    std::chrono::time_point<std::chrono::steady_clock> previous;
	};

	typedef Timer<std::chrono::seconds> SecTimer;
	typedef Timer<std::chrono::milliseconds> MilliTimer;
	typedef Timer<std::chrono::nanoseconds> NanoTimer;
} // namespace utility

#endif // TIMER_H_
