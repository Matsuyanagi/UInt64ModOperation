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
	uint64_t hi, rem = 0;
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

	// a => b を保証する
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
 * uint64_t powmod64( const uint64_t a, const uint64_t e, const uint64_t mod )
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
	uint64_t te = e;
	if ( te >= mod ) {
		te = te % mod;
	}
	if ( te == 0 ) {
		return 1;
	}
	if ( a == mod - 1 ) {  // a ≡ -1 % mod
		return ( te & 1 ) ? mod - 1 : 1;
	}

	uint64_t ans = 1;
	uint64_t t = a;

	while ( te ) {
		if ( te & 1 ) {
			ans = umulmod64( ans, t, mod );
		}
		te >>= 1;
		if ( te == 0 ) {
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
	uint64_t gcd, x, y;
	gcd = extended_eucledian( b % a, a, mod, &x, &y );
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
 *  @return a / b mod
 */
uint64_t udivmod64( const uint64_t a, const uint64_t b, const uint64_t mod ) {
	if ( b == 0 ) {
		throw std::overflow_error( "Divide by Zero." );
	}
	return umulmod64( a, umodinv64( b, mod ), mod );
}
