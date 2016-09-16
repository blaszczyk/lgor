#ifndef __LGOR_COMPLEX__
#define __LGOR_COMPLEX__


#include <vector>
#include "ModelData.h"
#include "PolySpace.h"
#include "CohomSpace.h"
#include "Matrix.h"
#include "Polynomial.h"



/* ****************************************************************************************************************************************

A Complex contains 4 Polyspaces of q+-charge -3/2, -1/2, 1/2 , 3/2, with fixed q- charge, linked by the coboundary operator \bar Q
Actually, it turned out that we also need the -5/2 and +5/2 spaces (seen in (P^2[3])^3, so we make it six

**************************************************************************************************************************************** */



class Complex 
{
  
public:
std::vector<PolySpace> part;
std::vector<CohomSpace> cohomology;
std::vector<PolySpace> shrunk;
std::vector<Matrix> QBarMatrix;
std::vector<Matrix> QBarRemainder;
ModelData* mdata;

Complex(ModelData*);

int QBarAction(Polynomial* , Monomial* , Polynomial* );
int GGBarAction(Polynomial* , Polynomial* , Polynomial* );
void CreateQBarMatrices(Polynomial);
void ShrinkProblem();
void ReplaceByShrunk();
void FuseRemainderQBars();
void FindFusedQBarKernels();
void KillMonomialPreimages();
};


#endif 

