#include "pch.h"
#include "../UInt64ModOperation/uint64_mod_operation.h"


TEST(TestCaseName, uaddmod64) {
	uint64_t a, b, c, p, ans;

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
	EXPECT_EQ( c-1, usubmod64( 0xFFFFFFFFFFFFFFFE, 0xFFFFFFFFFFFFFFFF, c ) );
	EXPECT_EQ( 0x8000000000000000, usubmod64( 0xffffffffffffffc4, 0x7fffffffffffffc4, c ) );
	EXPECT_EQ( 0x8000000000000000, usubmod64( 0xffffffffffffffc6, 0x7fffffffffffffc6, c ) );
}
