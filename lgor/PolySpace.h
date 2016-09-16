#ifndef __LGOR_POLYSPACE__
#define __LGOR_POLYSPACE__

#include <vector>
#include "Monomial.h"

/* ****************************************************************************************************************************************

A Polyspace is a polynomial vector space of definite charge q+ and q-

**************************************************************************************************************************************** */

class PolySpace {
public:
	std::vector<Monomial> term;
	void ReduceBasis();
};




#endif
