#include "Monomial.h"
#include "PrintDef.h"
#include <iostream>
#include "SimpleMath.h"

using namespace std;



Monomial::Monomial ( ModelData* mData )
{
	mdata = mData;
	BPower.resize ( mdata->NrFields,0 );
	FPower.resize ( mdata->NrFields,0 );
	BBPower.resize ( mdata->NrFields,0 );
	BFPower.resize ( mdata->NrFields,0 );
}

void Monomial::ClearPowers()
{
	for ( int i = 0; i < mdata->NrFields; i ++ )
	{
		BPower[i]=0;
		FPower[i]=0;
		BBPower[i]=0;
		BFPower[i]=0;
	}
}

void Monomial::MultiplyWith ( Monomial* Factor )
{
	for ( int i = 0; i < mdata->NrFields; i ++ )
	{
		BPower[i]+= Factor->BPower[i];
		FPower[i]+= Factor->FPower[i];
		BBPower[i]+= Factor->BBPower[i];
		BFPower[i]+= Factor->BFPower[i];
	}
}

void Monomial::CopyFrom ( Monomial* Preimage )
{
	mdata = Preimage->mdata;
	for ( int i = 0; i < mdata->NrFields; i ++ )
	{
		BPower[i]= Preimage->BPower[i];
		FPower[i]= Preimage->FPower[i];
		BBPower[i]= Preimage->BBPower[i];
		BFPower[i]= Preimage->BFPower[i];
	}
}

void Monomial::DivideBy ( Monomial* Factor )
{
	for ( int i = 0; i < mdata->NrFields; i ++ )
	{
		BPower[i]-= Factor->BPower[i];
		FPower[i]-= Factor->FPower[i];
		BBPower[i]-= Factor->BBPower[i];
		BFPower[i]-= Factor->BFPower[i];
	}
}

int Monomial::IsProductOf ( Monomial* Factor1, Monomial* Factor2 )
{
	mdata = Factor1->mdata;
	int i,j,factor = 1, NrSigns=0, F2FNr = 0;
	for ( i = 0; i < mdata->NrFields; i ++ )
	{
		BPower[i] = Factor1->BPower[i] + Factor2->BPower[i] ;
		FPower[i] = Factor1->FPower[i] + Factor2->FPower[i];
		BBPower[i] = Factor1->BBPower[i] + Factor2->BBPower[i];
		BFPower[i] = Factor1->BFPower[i] + Factor2->BFPower[i];

		if ( Factor1->BPower[i] < 0 &&  Factor2->BPower[i] > 0 )
			factor *= choosesort ( Factor2->BPower[i] , - Factor1->BPower[i] );
		if ( Factor2->BPower[i] < 0 && Factor1->BPower[i] > 0 )
			factor *= choosesort ( Factor1->BPower[i] , - Factor2->BPower[i] );
		if ( Factor1->BBPower[i] < 0 && Factor2->BBPower[i] > 0 )
			factor *= choosesort ( Factor2->BBPower[i] , - Factor1->BBPower[i] );
		if ( Factor2->BBPower[i] < 0 && Factor1->BBPower[i] > 0 )
			factor *= choosesort ( Factor1->BBPower[i] , - Factor2->BBPower[i] );

		if ( Factor1->BFPower[i] != 0 )
			NrSigns++;
		if ( Factor2->FPower[i] != 0 )
			F2FNr ++;
	}
	NrSigns *= F2FNr;
	for ( i = 1 ; i < mdata->NrFields; i++ )
		for ( j = 0 ; j < i ; j++ )
		{
			if ( Factor1->FPower[i] != 0 && Factor2->FPower[j] != 0 )
				NrSigns++;
			if ( Factor1->BFPower[i] != 0 && Factor2->BFPower[j] != 0 )
				NrSigns++;
		}
	if ( TrueMod ( NrSigns,2 ) == 1 )
		factor = - factor;
	return factor;
}

bool Monomial::EqualTo ( Monomial* Comparator )
{
	for ( int i = 0; i < mdata->NrFields; i ++ )
	{
		if ( BPower[i] != Comparator->BPower[i] )
			return false;
		if ( FPower[i] != Comparator->FPower[i] )
			return false;
		if ( BBPower[i] != Comparator->BBPower[i] )
			return false;
		if ( BFPower[i] != Comparator->BFPower[i] )
			return false;
	}
	return true;
}

bool Monomial::Holomorphic()
{
	for ( int i = 0; i < mdata->NrFields; i ++ )
	{
		if ( BPower[i]<0 ) return false;
		if ( FPower[i]<0 ) return false;
		if ( BBPower[i]>0 ) return false;
		if ( BFPower[i]>0 ) return false;
	}
	return true;
}

bool Monomial::Zero()
{
	for ( int i = 0; i < mdata->NrFields; i ++ )
	{
		if ( BPower[i]!=0 ) return false;
		if ( BBPower[i]!=0 ) return false;
		if ( FPower[i]==1 ) return false;
		if ( BFPower[i]==1 ) return false;
	}
	return true;
}


bool Monomial::Regular()
{
	for ( int i = 0; i < mdata->NrFields; i ++ )
	{
		if ( BPower[i]<0 ) return false;
		if ( FPower[i]<0 ) return false;
		if ( BBPower[i]<0 ) return false;
		if ( BFPower[i]<0 ) return false;
	}
	return true;
}


bool Monomial::RegularAndNonZero()
{
	for ( int i = 0; i < mdata->NrFields; i ++ )
	{
		if ( BPower[i]<0 ) return false;
		if ( FPower[i]<0 || FPower[i] > 1 ) return false;
		if ( BBPower[i]<0 ) return false;
		if ( BFPower[i]<0 || BFPower[i] > 1 ) return false;
	}
	return true;
}

/*
int Monomial::FirstFermion()
{
  int i;
  for( i = 0 ; i < mdata->NrFields ; i ++)
    if (FPower[i] == 1 || FPower[i] == -1)
      return i;
  for( i = 0 ; i < mdata->NrFields ; i ++)
    if (BFPower[i] == 1 || BFPower[i] == -1)
      return i + mdata->NrFields;
  return -1;
}

int Monomial::CountFermionsTo(int position)
{
  int i;
  if(position < 0)
    return 0;
  int result = 0;
  if(position < mdata->NrFields)
  {
    for( i = 0 ; i < position ; i ++)
      if (FPower[i] == 1 || FPower[i] == -1)
        result ++;
  }
  else
  {
    position -= mdata->NrFields;
    for( i = 0 ; i < mdata->NrFields ; i ++)
      if (FPower[i] == 1 || FPower[i] == -1)
        result ++;
    for( i = 0 ; i < position ; i ++)
      if (BFPower[i] == 1 || BFPower[i] == -1)
        result ++;
  }
  return result;
}
//*/

int Monomial::TotalPower()
{
	int result = 0;
	for ( int i = 0; i < mdata->NrFields; i ++ )
		result += BPower[i] + FPower[i] + BBPower[i] + BFPower[i];
	return result;
}

int Monomial::FermionNumber()
{
	int result = 0;
	for ( int i = 0; i < mdata->NrFields; i ++ )
		result +=  FPower[i] - BFPower[i];
	return result;
}

int Monomial::Energy ( SField* SFeld, int VacuumEnergy )
{
	for ( int i = 0; i < mdata->NrFields; i ++ )
	{
		VacuumEnergy += BPower[i] * SFeld[i].BosonTwist;
		VacuumEnergy += BBPower[i] * SFeld[i].BBosonTwist;
		VacuumEnergy -= FPower[i] * SFeld[i].BFermionTwist;
		VacuumEnergy -= BFPower[i] * SFeld[i].FermionTwist;
	}
	return VacuumEnergy;
}

int Monomial::QuLeft ( int VacuumCharge )
{
	for ( int i = 0; i < mdata->NrFields; i ++ )
	{
		VacuumCharge += 2 * ( BPower[i] - BBPower[i] ) * mdata->Fld[i].Kcharge;
		VacuumCharge += 2 * ( FPower[i] - BFPower[i] ) * ( mdata->Fld[i].Kcharge - mdata->KOrder );
	}
	return VacuumCharge;
}

int Monomial::QuRight ( int VacuumCharge )
{
	for ( int i = 0; i < mdata->NrFields; i ++ )
	{
		VacuumCharge += 2 * ( BPower[i] - BBPower[i] + FPower[i] - BFPower[i] ) * mdata->Fld[i].Kcharge;
	}
	return VacuumCharge;
}

int Monomial::Charge ( int DiscreteSymmetry, int VacuumCharge )
{
	for ( int i = 0; i < mdata->NrFields; i ++ )
	{
//	mdata->Print();
		VacuumCharge += ( BPower[i] - BBPower[i] + FPower[i] - BFPower[i] ) * mdata->Fld[i].Charge[DiscreteSymmetry];
	}
	return VacuumCharge;
}

void Monomial::Print()
{
	if ( this->Zero() )
	{
		cout << 1;
		return;
	}
	int i;
	for ( i=0; i<mdata->NrFields; i++ )
	{
		if ( BPower[i]!=0 )
		{
			cout << BosonName << mdata->Fld[i].Name;
			if ( BPower[i]!=1 )
				cout << "^" << BPower[i];
			cout << ' ';
		}
	}
	for ( i=0; i<mdata->NrFields; i++ )
	{
		if ( BBPower[i]!=0 )
		{
			cout << BBosonName << mdata->Fld[i].Name;
			if ( BBPower[i]!=1 )
				cout << "^" << BBPower[i];
			cout << ' ';
		}
	}
	for ( i=0; i<mdata->NrFields; i++ )
	{
		if ( FPower[i]!=0 )
		{
			cout << FermionName << mdata->Fld[i].Name;
			if ( FPower[i]!=1 )
				cout << "^" << FPower[i];
			cout << ' ';
		}
	}
	for ( i=0; i<mdata->NrFields; i++ )
	{
		if ( BFPower[i]!=0 )
		{
			cout << BFermionName << mdata->Fld[i].Name;
			if ( BFPower[i]!=1 )
				cout << "^" << BFPower[i];
			cout << ' ';
		}
	}
}
