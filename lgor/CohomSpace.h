#ifndef __LGOR_COHOMSPACE__
#define __LGOR_COHOMSPACE__

#include <vector>
#include "Polynomial.h"

/* ****************************************************************************************************************************************

A Polyspace is a polynomial vector space of definite charge q+ and q-

**************************************************************************************************************************************** */

class CohomSpace {
public:
	std::vector<Polynomial> term;
};




#endif
