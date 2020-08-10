#include "uint64_mod_operation.h"

/**
 * uadd64( uint64_t a, uint64_t b, uint64_t mod )
 * @param a
 * @param b
 * @param mod modular
 * @return ( a + b ) % mod
 */
uint64_t uadd64( uint64_t a, uint64_t b, uint64_t mod ) {
	uint64_t ans;
	if ( a > std::numeric_limits<uint64_t>::max() - b ) {
		// overflow
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
 * uint64_t usub64( uint64_t a, uint64_t b, uint64_t mod )
 * @param a
 * @param b
 * @param mod modular
 * @return ( a - b ) % mod
 */
uint64_t usub64( uint64_t a, uint64_t b, uint64_t mod ) {
	uint64_t ans;
	if ( a < b ) {
		ans = mod - ( b - a ) % mod;
	} else {
		ans = ( a - b ) % mod;
	}
	return ans;
}

/**
 * uint64_t mulmod64( const uint64_t a, const uint64_t b, const uint64_t mod )
 * @param a
 * @param b
 * @param mod modular
 * @return ( a * b ) % mod
 */
uint64_t mulmod64( const uint64_t a, const uint64_t b, const uint64_t mod ) {
	uint64_t hi, rem;
	if ( a == 0 || b == 0 ) {
		return 0;
	}
	const uint64_t lo = _umul128( a, b, &hi );
	_udiv128( hi, lo, mod, &rem );
	return rem;
}

/**
 * uint64_t powmod64( const uint64_t a, const uint64_t e, const uint64_t mod )
 * @param a base
 * @param e exponent
 * @param mod modular
 * @return ( a ** e ) % mod
 */
uint64_t powmod64( uint64_t a, uint64_t e, const uint64_t mod ) {
	if ( e == 0 ) {
		return 1;
	}
	a %= mod;
	if ( a == 1 ) {
		return 1;
	}
	uint64_t te = e % mod;
	if ( a == mod - 1 ){
		return ( te & 1 ) ? mod - 1 : 1;
	}
	
	uint64_t ans = 1;
	uint64_t t = a;

	while ( te ) {
		if ( te & 1 ) {
			ans = mulmod64( ans, t, mod );
		}
		te >>= 1;
		if ( te == 0 ) {
			break;
		}
		t = mulmod64( t, t, mod );
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
	q = mulmod64( q, x, mod );
	*ox = usub64( y, q, mod );

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
	if ( a > mod ) {
		a %= mod;
	}
	uint64_t gcd = extended_eucledian( a, mod, mod, &x, &y );
	if ( gcd != 1 ) {
		throw "Inverse doesn't exits\n";
	} else {
		y = x % mod;
	}
	return y;
}

/**
 * divmod64( const uint64_t a, const uint64_t b, const uint64_t mod )
 *  @param a Dividend
 *  @param b divisor
 *  @param mod modular
 *  @return a / b mod
 */
uint64_t divmod64( const uint64_t a, const uint64_t b, const uint64_t mod ) {
	uint64_t ans = mulmod64( a, umodinv64( b, mod ), mod );
	return ans;
}
