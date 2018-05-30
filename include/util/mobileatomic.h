#pragma once

#include <cstdint>
#include <atomic>

namespace util {

template<typename T>
struct MobileAtomic
{
	std::atomic<T> atomic;

	MobileAtomic() : atomic(T()) {}

	explicit MobileAtomic ( T const& v ) : atomic ( v ) {}
	explicit MobileAtomic ( std::atomic<T> const& a ) : atomic ( a.load() ) {}
	inline T load(const std::memory_order& order = std::memory_order_seq_cst) {
		return atomic.load(order);
	}

	inline void store(const T& desired, const std::memory_order& order = std::memory_order_seq_cst) {
		atomic.store(desired, order);
	}

	MobileAtomic ( MobileAtomic const&other ) : atomic( other.atomic.load() ) {}

	MobileAtomic& operator=( MobileAtomic const &other )
	{
		atomic.store( other.atomic.load() );
		return *this;
	}
};

}
