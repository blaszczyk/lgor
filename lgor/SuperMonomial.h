#ifndef __LGOR_SUPERMONOMIAL__
#define __LGOR_SUPERMONOMIAL__



#include <vector>

/* ****************************************************************************************************************************************

SUPERMonoooooomials  ,  basically only needed for the SuperPolynomial in the Superpotential

**************************************************************************************************************************************** */

class SuperMonomial
{
 public:
  std::vector<int> Power;
  int Koeff;
  
  SuperMonomial(int);
};

#endif 
