#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <vector>

#include "uint64_mod_operation.h"

int main() {
	uint64_t a, b, c, p, ans;

	a = 0xFFFF'FFFF'FFFF'F000;
	b = 0xFFFF'FFFF'FFFF'FF00;
	c = 0xFFFF'FFFF'FFFF'FFC5;

	ans = ( a + b ) % c;

	printf( "a   : %llx\n", a );
	printf( "b   : %llx\n", b );
	printf( "a+b : %llx\n", a + b );
	printf( "ans : %llx\n", ans );
	printf( "----\n" );

	ans = uaddmod64( a, b, c );

	printf( "ans : %llx\n", ans );
	printf( "----\n" );

	a = 0x05;
	b = 0xFFFF'FFFF'FFFF'FF00;
	c = 0x8000;

	if ( a < b ) {
		ans = c - ( b - a ) % c;

	} else {
		ans = ( a - b ) % c;
	}

	printf( "a   : %llx\n", a );
	printf( "b   : %llx\n", b );
	printf( "a+b : %llx\n", a + b );
	printf( "ans : %llx\n", ans );

	ans = usubmod64( a, b, c );
	printf( "ans : %llx\n", ans );

	printf( "----\n" );

	a = 0x05;
	b = 0xFFFF'FFFF'FFFF'FF00;
	c = 0x8000;
	p = 41;

	std::vector<uint64_t> prime_numbers{
		23,
		41,
		43,
		47,
	};

	for ( auto &&prime : prime_numbers ) {
		printf( "-- prime : %llu\n", prime );
		for ( size_t i = 2; i < prime - 1; i++ ) {
			uint64_t inv = umodinv64( i, prime );
			if ( inv != powmod64( i, prime - 2, prime ) ) {
				printf( "error : prime: %llu, i = %llu\n", prime, i );
			} else {
				printf( "prime: %llu, i = %llu, inv = %llu \n", prime, i, inv );
			}
		}
	}
	printf( "----\n" );

	prime_numbers.clear();
	prime_numbers.push_back( 18446744073709551557U );
	prime_numbers.push_back( 18446744073709551533U );
	prime_numbers.push_back( 18446744073709551521U );
	prime_numbers.push_back( 18446744073709551437U );
	prime_numbers.push_back( 18446744073709551427U );
	prime_numbers.push_back( 18446744073709551359U );

	for ( auto &&prime : prime_numbers ) {
		printf( "-- prime : %llu\n", prime );
		for ( size_t i = 2; i < 10; i++ ) {
			const uint64_t inv = umodinv64( i, prime );
			if ( inv != powmod64( i, prime - 2, prime ) ) {
				printf( "error : prime: %llu, i = %llu\n", prime, i );
			} else {
				printf( "prime: %llu, i = %llu, inv = %llu \n", prime, i, inv );
			}
		}
		for ( size_t i = prime - 10; i < prime - 1; i++ ) {
			uint64_t inv = umodinv64( i, prime );
			if ( inv != powmod64( i, prime - 2, prime ) ) {
				printf( "error : prime: %llu, i = %llu\n", prime, i );
			} else {
				printf( "prime: %llu, i = %llu, inv = %llu \n", prime, i, inv );
			}
		}
	}
	printf( "----\n" );
}
