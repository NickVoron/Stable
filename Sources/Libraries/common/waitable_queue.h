// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
//
// This library is distributed under the MIT License. See notice at the end
// of this file.
//
// This work is based on the RedStar project
//

#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <stack>
#include <atomic>
#include <algorithm>
#include <chrono>

namespace mt
{
	
	template<class T> struct queue_impl : public std::queue<T> {};
	template<class T> struct stack_impl : public std::stack<T> {};
	template<class T> struct priority_impl : public std::priority_queue<T> {};

	template<class T, template<class> class container_impl, class result_container>
	struct container : public container_impl<T>
	{
		auto begin() { return this->c.begin(); }
		auto end() { return this->c.end(); }
		auto begin() const { return this->c.begin(); }
		auto end() const { return this->c.end(); }

		void send(const T& d) { this->push(d); }

		template<typename... Args>
		void emplace_send(Args&&... args) { this->emplace(args...); }
		
		bool receive(T& d)
		{
			if (!this->empty())
			{
				d = std::move(((result_container*)this)->first());
				return true;
			}

			return false;
		}

		void clear() { this->c.clear(); }
	};

	template<class T>
	struct fifo : public container<T, queue_impl, fifo<T>>
	{
		decltype(auto) first() { return fifo::front(); }
	};

	template<class T>
	struct lifo : public container<T, stack_impl, lifo<T>>
	{
		decltype(auto) first() { return lifo::top(); }
	};

	template<class T>
	struct priority : public container<T, priority_impl, priority<T>>
	{
		decltype(auto) first() { return priority::top(); }
	};

	template <typename T, template<class> class Cont = fifo, class Mutex = std::mutex >
	class waitable_queue
	{
	public:
		using mutex = Mutex;

		waitable_queue() : canceled(false) {	}
		~waitable_queue() { clear(true); }

		decltype(auto) begin() { return buffer.begin(); }
		decltype(auto) end() { return buffer.end(); }
		decltype(auto) begin() const { return buffer.begin(); }
		decltype(auto) end() const { return buffer.end(); }

		void lock() { monitor.lock(); }
		void unlock() { monitor.unlock(); }

		void send(const T& d)
		{
			{
				std::unique_lock<mutex> lock(monitor);
				buffer.send(d);
			}
			
			buffer_not_empty.notify_one();
		}

		template<typename... Args>
		void emplace(Args&&... args)
		{
			{
				std::unique_lock<mutex> lock(monitor);
				buffer.emplace_send(args...);
			}

			buffer_not_empty.notify_one();
		}

		bool receive(T& d)
		{
			std::unique_lock<mutex> lock(monitor);
			while (buffer.empty() && !canceled)
				buffer_not_empty.wait(lock);
			
			if (!canceled)
			{
				if(buffer.receive(d))
				{
					buffer.pop();
				}				
			}

			return !canceled;
		}

		template<class Function>
		bool exec_receive(T& d, Function&& func)
		{
			std::unique_lock<mutex> lock(monitor);
			while (buffer.empty() && !canceled)
				buffer_not_empty.wait(lock);
			
			if (!canceled)
			{
				buffer.receive(d);
				func(d);
				buffer.pop();
			}

			return !canceled;
		}


		template<class Function>
		bool exec_receive(Function&& func)
		{
			T value;
			return exec_receive(value, std::forward<Function>(func));
		}

		bool try_receive(T& d)
		{
			std::unique_lock<mutex> lock(monitor);

			if (buffer.empty())
				return false;

			buffer.receive(d);
			buffer.pop();

			return true;
		}

		template<class Function>
		bool try_exec_receive(T& d, Function&& func)
		{
			std::unique_lock<mutex> lock(monitor);

			if (buffer.empty())
				return false;

			buffer.receive(d);
			func(d);
			buffer.pop();

			return true;
		}

		template<class Function>
		bool try_exec_receive(Function&& func)
		{
			T value;
			return try_exec_receive(value, std::forward<Function>(func));
		}

		bool empty() const
		{
			std::unique_lock<mutex> lock(monitor);
			return buffer.empty();
		}

		void cancel()
		{
			canceled = true;
			buffer_not_empty.notify_all();
		}

		void restart()
		{
			canceled = false;
		}

		bool exists(const T& d) const
		{
			std::unique_lock<mutex> lock(monitor);
			return std::find(buffer.begin(), buffer.end(), d) != buffer.end();
		}

		void replace(const T& src, const T& dst)
		{
			std::unique_lock<mutex> lock(monitor);
			auto it = std::find(buffer.begin(), buffer.end(), src);

			if (it != buffer.end())
			{
				*it = dst;
			}
		}

		void clear(bool _cancel)
		{
			std::unique_lock<mutex> lock(monitor);
			buffer.clear();
			buffer_not_empty.notify_all();
			canceled = _cancel;
		}

		auto size() const
		{
			std::unique_lock<mutex> lock(monitor);
			return buffer.size();
		}

	protected:
		Cont<T> buffer;
		std::condition_variable buffer_not_empty;
		mutable mutex monitor;
		std::atomic_bool canceled;
	};

	template<class Content>
	struct timing_entry
	{
		std::chrono::high_resolution_clock::time_point fence;
		Content content;

		friend bool operator<(const timing_entry& e0, const timing_entry& e1)
		{
			return e0.fence > e1.fence;
		}
	};

	template<class Content, class Mutex = std::mutex>
	class timed_queue : protected waitable_queue<timing_entry<Content>, mt::priority, Mutex>
	{
	public:
		using queue = waitable_queue<timing_entry<Content>, mt::priority, Mutex>;

		template<typename DurationInternalType, typename DurationPeriod>
		void send(const Content& content, std::chrono::duration<DurationInternalType, DurationPeriod> dt)
		{
			queue::send({ std::chrono::high_resolution_clock::now() + dt, content });
		}

		template<class Function>
		void update(Function&& function)
		{
			std::vector<Content> content;
			{
				timing_entry<Content> entry;
				std::unique_lock<typename queue::mutex> lock(queue::monitor);
				auto time = std::chrono::high_resolution_clock::now();
				while (!queue::buffer.empty())
				{
					auto fence = queue::buffer.first().fence;
					if (time >= fence)
					{
						queue::buffer.receive(entry);
						content.push_back(entry.content);
						queue::buffer.pop();
						time = std::chrono::high_resolution_clock::now();
					}
					else
					{
						break;
					}
				}
			}

			for (auto& element : content)
			{
				function(element);
			}			
		}
	};

}




// Copyright (C) 2013-2018 Voronetskiy Nikolay <nikolay.voronetskiy@yandex.ru>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
// and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions 
// of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.