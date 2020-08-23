#include "uint64_mod_operation.h"

/**
 * uaddmod64( uint64_t a, uint64_t b, uint64_t mod )
 * @param a
 * @param b
 * @param mod modular
 * @return ( a + b ) % mod
 */
uint64_t uaddmod64( uint64_t a, uint64_t b, uint64_t mod ) {
	uint64_t ans;
	if ( is_add_overflow( a, b ) ) {
		// a + b : overflow
		ans = std::numeric_limits<uint64_t>::max() % mod;
		ans += ( a - ( std::numeric_limits<uint64_t>::max() - b ) ) % mod;
		if ( ans >= mod ) {
			ans -= mod;
		}
	} else {
		ans = ( a + b ) % mod;
	}
	return ans;
}

/**
 * uint64_t usubmod64( uint64_t a, uint64_t b, uint64_t mod )
 * @param a
 * @param b
 * @param mod modular
 * @return ( a - b ) % mod
 */
uint64_t usubmod64( uint64_t a, uint64_t b, uint64_t mod ) {
	uint64_t ans;
	if ( a < b ) {
		ans = ( b - a ) % mod;
		if ( ans != 0 ) {
			ans = mod - ans;
		}
	} else {
		ans = ( a - b ) % mod;
	}
	return ans;
}

/**
 * uint64_t umulmod64( const uint64_t a, const uint64_t b, const uint64_t mod )
 * @param a
 * @param b
 * @param mod modular
 * @return ( a * b ) % mod
 */
uint64_t umulmod64( uint64_t a, uint64_t b, const uint64_t mod ) {
	if ( mod == 0 ) {
		throw std::overflow_error( "Divide by Zero." );
	}

	//
	if ( a >= mod ) {
		a %= mod;
	}
	if ( b >= mod ) {
		b %= mod;
	}

	if ( a == 0 || b == 0 || mod == 1 ) {
		return 0;
	}

	//	_umul128(), _udiv128() can be used.
	// The conditions for using _umul128() and _udiv128() are bitlen( a * b ) - bitlen( mod ) < sizeof( uint64_t ).
	// The quotient does not overflow.
	if ( static_cast<int>( sizeof( uint64_t ) ) * 8 - __lzcnt64( a ) - __lzcnt64( b ) + __lzcnt64( mod ) <
	     static_cast<int>( sizeof( uint64_t ) * 8 ) ) {
		uint64_t hi = 0, rem = 0;
		const uint64_t lo = _umul128( a, b, &hi );
		_udiv128( hi, lo, mod, &rem );
		return rem;
	}

	// Ensure that a >= b.
	if ( a < b ) {
		uint64_t t = a;
		a = b;
		b = t;
	}

	uint64_t ans = 0;
	uint64_t x = a;
	while ( b ) {
		if ( b & 1 ) {
			if ( is_add_overflow( ans, x ) ) {
				ans = uaddmod64( ans, x, mod );
			} else {
				// ans < mod , x < mod : ans + x < 2 * mod
				ans += x;
				if ( ans > mod ) {
					ans -= mod;
				}
			}
		}
		b >>= 1;
		if ( b == 0 ) {
			break;
		}

		if ( is_add_overflow( x, x ) ) {
			x = uaddmod64( x, x, mod );
		} else {
			// x < mod : x + x < 2 * mod
			x += x;
			if ( x > mod ) {
				x -= mod;
			}
		}
	}

	return ans;
}

/**
 * uint64_t powmod64( uint64_t a, uint64_t e, const uint64_t mod )
 * @param a base
 * @param e exponent
 * @param mod modular
 * @return ( a ** e ) % mod
 */
uint64_t powmod64( uint64_t a, uint64_t e, const uint64_t mod ) {
	if ( a >= mod ) {
		a %= mod;
	}
	if ( a == 1 ) {
		return 1;
	}
	if ( e >= mod ) {
		e = e % mod;
	}
	if ( e == 0 ) {
		return 1;
	}
	if ( a == mod - 1 ) {                // a ≡ -1 % mod
		return ( e & 1 ) ? mod - 1 : 1;  // Returns -1 if the exponent is odd and 1 if it is even.
	}

	uint64_t ans = 1;
	uint64_t t = a;

	while ( e ) {
		if ( e & 1 ) {
			ans = umulmod64( ans, t, mod );
		}
		e >>= 1;
		if ( e == 0 ) {
			break;
		}
		t = umulmod64( t, t, mod );
	}
	return ans;
}

uint64_t extended_eucledian( uint64_t a, uint64_t b, uint64_t mod, uint64_t *ox, uint64_t *oy ) {
	if ( a == 0 ) {
		*ox = 0;
		*oy = 1;
		return b;
	}
	uint64_t x, y;
	uint64_t t = b;
	if ( b >= a ){
		t = b % a;
	}
	uint64_t gcd = extended_eucledian( t, a, mod, &x, &y );
	// *x1 = y - ( b / a ) * x;
	uint64_t q = b / a;
	q = umulmod64( q, x, mod );
	*ox = usubmod64( y, q, mod );

	*oy = x;
	return gcd;
}

/**
 * uint64_t umodinv64( uint64_t a, uint64_t mod )
 * @param a
 * @param mod
 * @return a^-1 % mod
 */
uint64_t umodinv64( uint64_t a, uint64_t mod ) {
	uint64_t x, y;
	if ( a >= mod ) {
		a %= mod;
	}
	uint64_t gcd = extended_eucledian( a, mod, mod, &x, &y );
	if ( gcd != 1 ) {
		throw std::overflow_error( "The inverse does not exist." );
	} else {
		y = x % mod;
	}
	return y;
}

/**
 * udivmod64( const uint64_t a, const uint64_t b, const uint64_t mod )
 *  @param a Dividend
 *  @param b divisor
 *  @param mod modular
 *  @return ( a / b ) % mod
 */
uint64_t udivmod64( const uint64_t a, const uint64_t b, const uint64_t mod ) {
	if ( b == 0 ) {
		throw std::overflow_error( "Divide by Zero." );
	}
	return umulmod64( a, umodinv64( b, mod ), mod );
}

/**
 *	is_prime( uint64_t x )
 *
 */
const static std::map<uint64_t, uint64_t> prime_and_max = std::map<uint64_t, uint64_t>{
    { 2, 8321 },  //	2047. Strong pseudoprimes to base 2, [ 2047( = 23x89 ),
                  // 3277( = 29x113 ), 4033( = 37x109 ), 4681( = 31x151 ), 8321(
                  //= 53x157 )
    { 3, 1373653 },
    { 5, 25326001 },
    { 7, 3215031751 },
    { 11, 2152302898747 },
    { 13, 3474749660383 },
    { 17, 341550071728321 },
    { 19, 341550071728321 },
    { 23, 3825123056546413051 },
    { 29, 3825123056546413051 },
    { 31, 3825123056546413051 },
    { 37, 0xFFFF'FFFF'FFFF'FFFF },
    { 41, 0xFFFF'FFFF'FFFF'FFFF },
    // { 37, 318665857834031151167461 },
    // { 41, 3317044064679887385961981 },
};

bool is_prime( uint64_t target ) {
	if ( target < 2 ) {
		return false;
	}
	if ( target < 4 ) {
		return true;
	}
	if ( ( target & 1 ) == 0 ) {
		return false;
	}

	for ( const auto &[ p, m ] : prime_and_max ) {
		if ( target == p ) {
			return true;
		}
		if ( target % p == 0 ) {
			return false;
		}
	}

	uint64_t p_1 = target - 1;
	uint64_t d = p_1;

	while ( ( d & 1 ) == 0 ) {
		d >>= 1;
	}

	for ( const auto &[ p, m ] : prime_and_max ) {
		uint64_t x = powmod64( p, d, target );
		if ( x == 1 ) {
			if ( target < m ) {
				return true;
			}
			continue;
		}

		uint64_t td = d;
		while ( td != p_1 && x != p_1 ) {
			x = powmod64( x, 2, target );
			td <<= 1;
		}

		if ( td == p_1 ) {
			return false;
		} else {
			if ( x == p_1 && target < m ) {
				return true;
			}
		}
	}

	return true;
}

/**
 * isqrt( uint64_t x )
 * integer sqrt
 * Hacker's Delight
 * @param x number
 * @return sqrt( x ) integer
 */
uint64_t isqrt( uint64_t x ) {
	if ( x <= 3 ) {
		if ( x == 0 ) {
			return 0;
		}
		return 1;
	}

	// m = 0x4000'0000'0000'0000
	uint64_t m = ( __lzcnt64( x ) | 1ULL ) + 1ULL;
	m = 1ULL << ( sizeof( uint64_t ) * 8 - m );

	uint64_t y = 0;
	while ( m != 0 ) {
		const uint64_t b = y | m;
		y = y >> 1;
		if ( x >= b ) {
			x -= b;
			y |= m;
		}
		m = m >> 2;
	}
	return y;
}

/**
 * is_square( x )
 * @param x
 * @return Returns true if x is a square number.
 */
bool is_square( uint64_t x ) {
	const uint64_t root = isqrt( x );
	return x == root * root;
}
