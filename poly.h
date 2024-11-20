#ifndef POLY_H
#define POLY_H

#include "hash_list.h"

class poly{
		public:
		hash_list coeffs;
		

		poly();
		poly(poly& other);
		~poly();

			

};

#endif
