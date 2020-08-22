#include "pch.h"

#include "../UInt64ModOperation/uint64_mod_operation.h"

TEST( TestCaseName, uaddmod64 ) {
	uint64_t a, b, c;

	a = 0xFFFF'FFFF'FFFF'F000;
	b = 0xFFFF'FFFF'FFFF'FF00;
	c = 0xFFFF'FFFF'FFFF'FFC5;

	EXPECT_EQ( 0xffffffffffffef3b, uaddmod64( a, b, c ) );

	EXPECT_EQ( 0, uaddmod64( 0, 0, 2 ) );
	EXPECT_EQ( 1, uaddmod64( 1, 0, 2 ) );
	EXPECT_EQ( 1, uaddmod64( 0, 1, 2 ) );
	EXPECT_EQ( 1, uaddmod64( 0, 1, 11 ) );
	EXPECT_EQ( 2, uaddmod64( 1, 1, 11 ) );
	EXPECT_EQ( 0, uaddmod64( 5, 6, 11 ) );
	EXPECT_EQ( 1, uaddmod64( 6, 6, 11 ) );
	EXPECT_EQ( 6, uaddmod64( 36, 91, 11 ) );

	c = 0xFFFF'FFFF'FFFF'FFC5;
	EXPECT_EQ( 0, uaddmod64( c, c, c ) );
	EXPECT_EQ( 127, uaddmod64( 36, 91, c ) );
	EXPECT_EQ( 116, uaddmod64( 0xFFFF'FFFF'FFFF'FFFF, 0xFFFF'FFFF'FFFF'FFFF, c ) );
	EXPECT_EQ( 2, uaddmod64( 1, 1, c ) );
	EXPECT_EQ( 0x8000000000000000, uaddmod64( 0xffffffffffffffc4, 0x8000000000000001, c ) );
	EXPECT_EQ( 0x8000000000000000, uaddmod64( 0xffffffffffffffc6, 0x7fffffffffffffff, c ) );
}

TEST( TestCaseName, usubmod64 ) {
	uint64_t a, b, c, p, ans;

	a = 5;
	b = 0xFFFF'FFFF'FFFF'FF00;
	c = 0x8000;

	EXPECT_EQ( 261, usubmod64( a, b, c ) );

	EXPECT_EQ( 0, usubmod64( 0, 0, 2 ) );
	EXPECT_EQ( 1, usubmod64( 1, 0, 2 ) );
	EXPECT_EQ( 1, usubmod64( 0, 1, 2 ) );
	EXPECT_EQ( 10, usubmod64( 0, 1, 11 ) );
	EXPECT_EQ( 0, usubmod64( 1, 1, 11 ) );
	EXPECT_EQ( 10, usubmod64( 5, 6, 11 ) );
	EXPECT_EQ( 0, usubmod64( 6, 6, 11 ) );
	EXPECT_EQ( 0, usubmod64( 36, 91, 11 ) );
	EXPECT_EQ( 0, usubmod64( 91, 36, 11 ) );
	EXPECT_EQ( 9, usubmod64( 13, 15, 11 ) );

	c = 0xFFFF'FFFF'FFFF'FFC5;
	EXPECT_EQ( 0, usubmod64( c, c, c ) );
	EXPECT_EQ( 0, usubmod64( 0, 0, c ) );
	EXPECT_EQ( 0xffffffffffffff8e, usubmod64( 36, 91, c ) );
	EXPECT_EQ( 0, usubmod64( 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, c ) );
	EXPECT_EQ( 1, usubmod64( 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFE, c ) );
	EXPECT_EQ( c - 1, usubmod64( 0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFF, c ) );
	EXPECT_EQ( 0x8000000000000000, usubmod64( 0xffffffffffffffc4, 0x7fffffffffffffc4, c ) );
	EXPECT_EQ( 0x8000000000000000, usubmod64( 0xffffffffffffffc6, 0x7fffffffffffffc6, c ) );
}

TEST( TestCaseName, umodinv64 ) {
	std::vector<uint64_t> prime_numbers{ 3,   5,   7,   11,  13,  17,  19,  23,  29,  31,  37,  41,  43,  47,  53,  59,  61,
	                                     67,  71,  73,  79,  83,  89,  97,  101, 103, 107, 109, 113, 127, 131, 137, 139, 149,
	                                     151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229 };

	for ( auto &&prime : prime_numbers ) {
		for ( size_t i = 2; i < prime - 1; i++ ) {
			const uint64_t inv = umodinv64( i, prime );

			EXPECT_EQ( 1, umulmod64( i, inv, prime ) );
			EXPECT_EQ( powmod64( i, prime - 2, prime ), inv );
		}
	}
}

TEST( TestCaseName, umodinv64_large_number ) {
	std::vector<uint64_t> prime_numbers{ 18446744073709551557ULL, 18446744073709551533ULL, 18446744073709551521ULL,
	                                     18446744073709551437ULL, 18446744073709551427ULL, 18446744073709551359ULL,
	                                     18446744073709551337ULL, 18446744073709551293ULL, 18446744073709551263ULL,
	                                     18446744073709551253ULL };

	for ( auto &&prime : prime_numbers ) {
		for ( size_t i = 2; i < 1000; i++ ) {
			const uint64_t inv = umodinv64( i, prime );
			EXPECT_EQ( 1, umulmod64( i, inv, prime ) );
			EXPECT_EQ( powmod64( i, prime - 2, prime ), inv );
		}
		for ( size_t i = prime - 1000; i < prime - 1; i++ ) {
			const uint64_t inv = umodinv64( i, prime );
			EXPECT_EQ( 1, umulmod64( i, inv, prime ) );
			EXPECT_EQ( powmod64( i, prime - 2, prime ), inv );
		}
	}
}

TEST( TestCaseName, umulmod64 ) {
	uint64_t a, b, c, p, ans;

	a = 0xFFFF'FFFF'FFFF'F000;
	b = 0xFFFF'FFFF'FFFF'FF00;
	c = 0xFFFF'FFFF'FFFF'FFC5;

	//	EXPECT_EQ( 0xffffffffffffef3b, umulmod64( a, b, c ) );

	EXPECT_EQ( 0, umulmod64( 0, 0, 2 ) );
	EXPECT_EQ( 0, umulmod64( 1, 0, 2 ) );
	EXPECT_EQ( 0, umulmod64( 0, 1, 2 ) );
	EXPECT_EQ( 0, umulmod64( 0, 1, 11 ) );
	EXPECT_EQ( 1, umulmod64( 1, 1, 11 ) );
	EXPECT_EQ( 8, umulmod64( 5, 6, 11 ) );
	EXPECT_EQ( 3, umulmod64( 6, 6, 11 ) );
	EXPECT_EQ( 9, umulmod64( 36, 91, 11 ) );

	c = 0xFFFF'FFFF'FFFF'FFC5;
	EXPECT_EQ( 0, umulmod64( c, c, c ) );
	EXPECT_EQ( 1, umulmod64( 1, 1, c ) );
	EXPECT_EQ( 3276, umulmod64( 36, 91, c ) );
	EXPECT_EQ( 4137, umulmod64( 0xFFFF'FFFF'FFFF'FF00, 0xFFFF'FFFF'FFFF'FFB0, c ) );
	EXPECT_EQ( 4137, umulmod64( 0xFFFF'FFFF'FFFF'FFB0, 0xFFFF'FFFF'FFFF'FF00, c ) );
	// EXPECT_EQ( 3364, umulmod64( 0xFFFF'FFFF'FFFF'FFFE, 0xFFFF'FFFF'FFFF'FFFF, c ) );
	EXPECT_EQ( 1653, umulmod64( 0x7FFF'FFFF'FFFF'FFFF, 0xFFFF'FFFF'FFFF'FFFF, c ) );
	EXPECT_EQ( 1653, umulmod64( 0xFFFF'FFFF'FFFF'FFFF, 0x7FFF'FFFF'FFFF'FFFF, c ) );

	c = 0xfffffffffffffeff;
	EXPECT_EQ( 1, umulmod64( 2, 9223372036854775680, c ) );
	EXPECT_EQ( 1, umulmod64( 18446744073709551349U, 16602069666338596223U, c ) );
	EXPECT_EQ( 1, umulmod64( 3, umodinv64( 3, c ), c ) );
	EXPECT_EQ( 1, umulmod64( 5, umodinv64( 5, c ), c ) );
}

TEST( TestCaseName, is_prime ) {
	EXPECT_TRUE( is_prime( 2ULL ) );
	EXPECT_TRUE( is_prime( 3ULL ) );
	EXPECT_TRUE( is_prime( 5ULL ) );
	EXPECT_TRUE( is_prime( 7ULL ) );
	EXPECT_TRUE( is_prime( 41ULL ) );
	EXPECT_TRUE( is_prime( 43ULL ) );
	EXPECT_TRUE( is_prime( 47ULL ) );
	EXPECT_TRUE( is_prime( 97ULL ) );
	EXPECT_TRUE( is_prime( 65479ULL ) );
	EXPECT_TRUE( is_prime( 65497ULL ) );
	EXPECT_TRUE( is_prime( 18446744073709551557ULL ) );
	EXPECT_TRUE( is_prime( 18446744073709551533ULL ) );
	EXPECT_TRUE( is_prime( 18446744073709551521ULL ) );
	EXPECT_TRUE( is_prime( 18446744073709551437ULL ) );
	EXPECT_TRUE( is_prime( 18446744073709551427ULL ) );
	EXPECT_TRUE( is_prime( 18446744073709551359ULL ) );
	EXPECT_TRUE( is_prime( 18446744073709551337ULL ) );
	EXPECT_TRUE( is_prime( 18446744073709551293ULL ) );
	EXPECT_TRUE( is_prime( 18446744073709551263ULL ) );
	EXPECT_TRUE( is_prime( 18446744073709551253ULL ) );

	EXPECT_FALSE( is_prime( 0ULL ) );
	EXPECT_FALSE( is_prime( 1ULL ) );
	EXPECT_FALSE( is_prime( 4ULL ) );
	EXPECT_FALSE( is_prime( 10ULL ) );
	EXPECT_FALSE( is_prime( 199ULL - 1ULL ) );
	EXPECT_FALSE( is_prime( 997ULL * 1009ULL ) );
	EXPECT_FALSE( is_prime( 65479ULL * 65497ULL ) );
	EXPECT_FALSE( is_prime( 18446744073709551557ULL+2 ) );
	EXPECT_FALSE( is_prime( 18446744073709551533ULL+2 ) );
	EXPECT_FALSE( is_prime( 18446744073709551521ULL+2 ) );
	EXPECT_FALSE( is_prime( 18446744073709551437ULL+2 ) );
	EXPECT_FALSE( is_prime( 18446744073709551427ULL+2 ) );
	EXPECT_FALSE( is_prime( 18446744073709551359ULL+2 ) );
	EXPECT_FALSE( is_prime( 18446744073709551337ULL+2 ) );
	EXPECT_FALSE( is_prime( 18446744073709551293ULL+2 ) );
	EXPECT_FALSE( is_prime( 18446744073709551263ULL+2 ) );
	EXPECT_FALSE( is_prime( 18446744073709551253ULL+2 ) );
}
