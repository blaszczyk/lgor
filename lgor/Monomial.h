#ifndef __LGOR_MONOMIAL__
#define __LGOR_MONOMIAL__

#include <vector>
#include "ModelData.h"
#include "SField.h"


/* ****************************************************************************************************************************************

Monoooooomials

**************************************************************************************************************************************** */
class Monomial
{
  public:
  std::vector<int> BPower;
  std::vector<int> FPower;
  std::vector<int> BBPower;
  std::vector<int> BFPower;

  ModelData* mdata;
  Monomial(ModelData*);

  void ClearPowers();
  void MultiplyWith(Monomial*);
  int IsProductOf(Monomial*,Monomial*);
  void CopyFrom(Monomial*);
  void DivideBy(Monomial*);
  bool EqualTo(Monomial*);
  bool Zero();
  bool Holomorphic();
  bool Regular();
  bool RegularAndNonZero();
  
//  int FirstFermion();
//  int CountFermionsTo(int);
  
  int TotalPower();
  int FermionNumber();
  int Energy(SField*, int);
  int QuLeft(int);
  int QuRight(int);
  int Charge(int,int);
  void Print();
};

#endif
