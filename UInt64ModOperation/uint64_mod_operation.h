#pragma once

#include <intrin.h>
#include <stdint.h>

#include <limits>
#include <map>
#include <stdexcept>

uint64_t uaddmod64( uint64_t a, uint64_t b, uint64_t p );
uint64_t usubmod64( uint64_t a, uint64_t b, uint64_t p );
uint64_t umulmod64( const uint64_t a, const uint64_t b, const uint64_t mod );
uint64_t powmod64( const uint64_t a, const uint64_t e, const uint64_t mod );
uint64_t umodinv64( uint64_t a, uint64_t m );
bool is_prime( uint64_t self );
uint64_t isqrt( uint64_t x );
bool is_square( uint64_t x );

template <typename T>
bool is_add_overflow( const T a, const T b ) {
	return a > std::numeric_limits<T>::max() - b;
}
