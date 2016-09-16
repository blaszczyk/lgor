#include "SField.h"
#include "SimpleMath.h"
#include <iostream>

using namespace std;


SField::SField(ModelData* mData) 
{
                 mdata = mData;
}

void SField::ComputeTwists(int Kchrg, vector<int> chrg, int Ksctr, vector<int> sctr)
{
int kiqi = 0;
for( unsigned int i = 0; i<chrg.size(); i++)
{
  kiqi += chrg[i] * sctr[i] * mdata->OrderF[i];
}
BosonTwist = TrueMod( kiqi + Ksctr * Kchrg * mdata->KOrderF /2 , mdata->OrderHCF);
FermionTwist = TrueMod( kiqi + Ksctr * (Kchrg - mdata->KOrder ) * mdata->KOrderF /2 , mdata->OrderHCF);
if(FermionTwist > 0 ) 
  FermionTwist-=mdata->OrderHCF;
if(BosonTwist == 0)
  BBosonTwist=0;
else
  BBosonTwist= mdata->OrderHCF-BosonTwist;
if(FermionTwist == 0)
  BFermionTwist=0;
else
  BFermionTwist= -mdata->OrderHCF-FermionTwist;
}

int SField::ComputeMaxPower(int Enrgy)
{
  MaxPowerB=0;
  MaxPowerF=0;
  MaxPowerBB=0;
  MaxPowerBF=0;
  if( Enrgy > 0 )
    return true;
  if( Enrgy - BFermionTwist <= 0 )
    MaxPowerF = 1;
  if( Enrgy - FermionTwist <= 0 )
    MaxPowerBF = 1;
  if(FermionTwist == 0)
  {
    MaxPowerF = 0;   // these become anihilators
    MaxPowerBF = 1;
    FakeMaxPowerF =  0;
    FakeMaxPowerBF =  0;
  }
  else
  {
    FakeMaxPowerF =  Enrgy / BFermionTwist;
    FakeMaxPowerBF =  Enrgy / FermionTwist;
  }
  if(BosonTwist == 0)
  {
    MaxPowerB = mdata->OrderHCF / 2 / mdata->Fld[Identifier].Kcharge - 2;   // Otherwise in the image of Qbar
    MaxPowerBB = 0;   // these become anihilators
    MaxPowerBF = 0;  // Otherwise not in the kernel of Qbar
  }
  else
  {
    MaxPowerB = - Enrgy / BosonTwist;
    MaxPowerBB = - Enrgy / BBosonTwist;
  }
  NrMonomials = (1 + MaxPowerB ) * (1 + MaxPowerF ) * (1 + MaxPowerBB ) * (1 + MaxPowerBF );
  return NrMonomials;
}
