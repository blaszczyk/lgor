#ifndef __LGOR_SFIELD__
#define __LGOR_SFIELD__


#include <vector>
#include "Field.h"
#include "ModelData.h"

/* ****************************************************************************************************************************************

Here Comes the Class for the fields in the sectros

**************************************************************************************************************************************** */


class SField {
public:

ModelData *mdata;

int BosonTwist;
int FermionTwist;
int BBosonTwist;
int BFermionTwist;

int MaxPowerB ;
int MaxPowerF ;
int MaxPowerBB ;
int MaxPowerBF ;

int FakeMaxPowerF ;
int FakeMaxPowerBF ;

int NrMonomials ; 

int Identifier;

SField(ModelData*);
void ComputeTwists(int , std::vector<int>, int, std::vector<int>);
int ComputeMaxPower(int);
};





#endif
