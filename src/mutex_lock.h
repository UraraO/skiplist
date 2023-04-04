#pragma once

#include <pthread.h>
#include <sys/types.h>
#include <cassert>

// base noncopyable
class noncopyable {
public:
	noncopyable(const noncopyable &) = delete;

	noncopyable &operator=(const noncopyable &) = delete;

protected:
	noncopyable() = default;

	~noncopyable() = default;
};

// mutex_lock
class mutex_lock : noncopyable {
public:
	mutex_lock() : m_holder(0) { pthread_mutex_init(&m_mutex, nullptr); }

	~mutex_lock() {
		assert(m_holder == 0);
		pthread_mutex_destroy(&m_mutex);
	}

	bool is_locked_by_this_thread() { return m_holder == pthread_self(); }

	void lock() {
		pthread_mutex_lock(&m_mutex);
		m_holder = pthread_self();
	}

	void unlock() {
		m_holder = 0;
		pthread_mutex_unlock(&m_mutex);
	}

	pthread_mutex_t *get_pthread_mutex() { return &m_mutex; }

private:
	pthread_mutex_t m_mutex;
	pid_t m_holder;
};

// lock guard
// *** the CORRECT usage is :
// mutex_lock_guard lock(mutex);
// WRONG :
// mutex_lock_guard(mutex);
class mutex_lock_guard : noncopyable {
public:
	explicit mutex_lock_guard(mutex_lock &mutex)
			: m_mutex(mutex) {
		m_mutex.lock();
	}

	~mutex_lock_guard() {
		m_mutex.unlock();
	}

private:
	mutex_lock &m_mutex;
};

#define mutex_lock_guard(x) static_assert(false, "missing mutex guard var name")
// this macro is to prevent no guard var name

class condition : noncopyable {
public:
	explicit condition(mutex_lock &mutex)
			: m_mutex(mutex) {
		pthread_cond_init(&m_pcond, nullptr);
	}

	~condition() {
		pthread_cond_destroy(&m_pcond);
	}

	void wait() {
		pthread_cond_wait(&m_pcond, m_mutex.get_pthread_mutex());
	}

	void notify() { pthread_cond_signal(&m_pcond); }

	void notify_all() {pthread_cond_broadcast(&m_pcond); }

private:
	mutex_lock &m_mutex;
	pthread_cond_t m_pcond;
};















