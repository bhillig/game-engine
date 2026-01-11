#pragma once

#include <memory>
#include <mutex>
#include <queue>

template<typename T>
class threadsafe_queue
{
public:
	threadsafe_queue() = default;
	~threadsafe_queue() = default;

	// Delete copy constructor
	threadsafe_queue(const threadsafe_queue& other) = delete;

	// Delete copy assignment
	threadsafe_queue& operator =(const threadsafe_queue& other) = delete;

	// Move constructor
	threadsafe_queue(threadsafe_queue&& other) noexcept
	{
		m_queue = std::move(other.m_queue);
	}

	// Move assignment
	threadsafe_queue& operator =(threadsafe_queue&& other) noexcept
	{
		if (this != &other)
		{
			std::scoped_lock lock(m_mutex);
			m_queue = std::move(other.m_queue);
		}
		return *this;
	}


	// Push by copy
	void push(const T& value)
	{
		std::scoped_lock lock(m_mutex);
		m_queue.push(value);
		m_dataCondition.notify_one();
	}

	// Push by move
	void push(T&& value)
	{
		std::scoped_lock lock(m_mutex);
		m_queue.push(std::move(value));
		m_dataCondition.notify_one();
	}

	// Create in place
	template<typename... Args>
	void emplace(Args&&... args)
	{
		std::scoped_lock lock(m_mutex);
		m_queue.emplace(std::forward<Args>(args)...);
		m_dataCondition.notify_one();
	}

	bool try_pop(T& value)
	{
		std::scoped_lock lock(m_mutex);
		if (m_queue.empty())
		{
			return false;
		}

		value = std::move(m_queue.front());
		m_queue.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
		std::scoped_lock lock(m_mutex);
		if (m_queue.empty())
		{
			return nullptr;
		}

		std::shared_ptr<T> data(std::make_shared<T>(std::move(m_queue.front())));
		m_queue.pop();
		return data;
	}

	bool wait_and_pop(T& value, const std::atomic<bool>& stopFlag)
	{
		std::unique_lock lock(m_mutex);
		m_dataCondition.wait(lock, [this, &stopFlag] { return !m_queue.empty() || stopFlag.load(); });

		// If the queue is empty then the stop flag was set
		if (m_queue.empty())
		{
			return false;
		}

		value = std::move(m_queue.front());
		m_queue.pop();
		return true;
	}

	void wait_and_pop(T& value)
	{
		std::unique_lock lock(m_mutex);
		m_dataCondition.wait(lock, [this] { return !m_queue.empty(); });
		value = std::move(m_queue.front());
		m_queue.pop();
	}

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock lock(m_mutex);
		m_dataCondition.wait(lock, [this] { return !m_queue.empty(); });
		std::shared_ptr<T> data(std::make_shared<T>(std::move(m_queue.front())));
		m_queue.pop();
		return data;
	}

	void notify_stop()
	{
		m_dataCondition.notify_all();
	}

	size_t size() const
	{
		std::scoped_lock lock(m_mutex);
		return m_queue.size();
	}

	bool empty() const
	{
		std::scoped_lock lock(m_mutex);
		return m_queue.empty();
	}

private:
	mutable std::mutex m_mutex;
	std::queue<T> m_queue;
	std::condition_variable m_dataCondition;
};