#ifndef _MPRNG_H_
#define _MPRNG_H_
#include <uC++.h>
#include "PRNG.h"

_Monitor MPRNG : private PRNG {
	PRNG _prng;
	public:
		MPRNG( uint32_t s = 362436069 ) {
			_prng.seed(s);
		}	
		uint32_t seed() {
			return _prng.seed();
		}
		void seed( uint32_t  s ) {				// reset seed
			_prng.seed(s);
		}
		uint32_t operator()() {
			return _prng();
		}
		uint32_t operator()( uint32_t u ) {
			return _prng(u);
		}
		uint32_t operator()( uint32_t l, uint32_t u ) {
			return _prng( l, u );
		}
};

#endif
