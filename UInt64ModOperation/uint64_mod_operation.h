#pragma once

#include <intrin.h>
#include <stdint.h>

#include <limits>

uint64_t uadd64( uint64_t a, uint64_t b, uint64_t p );
uint64_t usub64( uint64_t a, uint64_t b, uint64_t p );
uint64_t mulmod64( const uint64_t a, const uint64_t b, const uint64_t mod );
uint64_t powmod64( const uint64_t a, const uint64_t e, const uint64_t mod );
uint64_t umodinv64( uint64_t a, uint64_t m );

