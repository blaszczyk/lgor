#ifndef __LGOR_POLYNOMIAL__
#define __LGOR_POLYNOMIAL__

#include <vector>
#include "Monomial.h"


/* ****************************************************************************************************************************************

What do we do with Monomials??? Of course, build Polynomails

**************************************************************************************************************************************** */


class Polynomial {

 public:
std::vector<Monomial> term;
std::vector<int> Koeff;

void Print();
void Print(bool);
void Clear();
void Simplify();
void Add(Polynomial*);
void Subtract(Polynomial*);
bool IsZero();
void VectorForm(std::vector<Monomial>*, std::vector<int>*);
void FromVector(std::vector<Polynomial>*, std::vector<int>*);
void FromVector(std::vector<Monomial>*, std::vector<int>*);
void FromMonomial(Monomial*);
};





#endif
