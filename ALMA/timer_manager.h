#pragma once
#ifndef TIMER_MANAGER_H_
#define TIMER_MANAGER_H_

#include <functional>
#include <memory>
#include <unordered_set>
#include <chrono>

class timer_manager
{
public:

	class timer;

	using timer_ptr_t = std::shared_ptr<timer>;
	using func_t = const std::function<void(const timer_ptr_t&)>;
	using timer_set_t = std::unordered_set<timer_ptr_t>;

	static timer_ptr_t emotion;
	static timer_ptr_t mood;
	timer_manager(void) = delete;

	static void setemo(timer_ptr_t emo)
	{
		emotion = emo;
	}
	static timer_ptr_t getemo() { return emotion; }


	static void setmood(timer_ptr_t emo)
	{
		emotion = emo;
	}
	static timer_ptr_t getmood() { return mood; }
	class timer : public std::enable_shared_from_this<timer>
	{
	public:

		timer(void) = delete;
		timer(long long delay, const func_t &func, bool repeat) :
			_time(std::chrono::steady_clock::now()),
			_delay(delay),
			_callback(func),
			_enable(true),
			_repeat(repeat),
			_remove_me(false) {};

		void enable(void)
		{
			if (!_enable)
			{
				_time = std::chrono::steady_clock::now();

				_enable = true;
			}
		}

		void disable(void)
		{
			if (_enable)
				_enable = false;
		}

		void process(void)
		{
			if (_enable)
			{
				auto time_span = std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::steady_clock::now() - _time);

				if (time_span.count() > _delay)
				{
					_time = std::chrono::steady_clock::now();

					_callback(shared_from_this());

					if (!_repeat)
						_remove_me = true;
				}
			}
		}

	private:

		friend class timer_manager;

		std::chrono::steady_clock::time_point _time;
		long long _delay;
		func_t _callback;
		bool _enable, _repeat, _remove_me;
	};

	static timer_set_t &get_timers(void)
	{
		static timer_set_t timers;

		return timers;
	}

	static void clear(void)
	{
		get_timers().clear();
	}

	static timer_ptr_t add_timer(long long delay, const func_t &func, bool repeat, std::string type)
	{
		timer_ptr_t new_timer = std::make_shared<timer>(delay, func, repeat);

		if (type == "emo")
		{
			setemo(new_timer);
		}
		else
		{
			setmood(new_timer);
		}
		get_timers().insert(new_timer);

		return new_timer;
	}

	static void remove_timer(const timer_ptr_t &timer_ptr)
	{
		timer_ptr->_remove_me = true;
	}

	static bool process(void)
	{
		auto &timers = get_timers();
		timer_set_t remove_timers;

		for (auto &timer : timers)
		{
			timer->process();

			if (timer->_remove_me)
				remove_timers.insert(timer);
		}

		for (auto &timer : remove_timers)
			timers.erase(timer);

		return !timers.empty();
	}
};


timer_manager::timer_ptr_t timer_manager::emotion = std::shared_ptr<timer>();
timer_manager::timer_ptr_t timer_manager::mood = std::shared_ptr<timer>();

#endif // TIMER_MANAGER_HPP_