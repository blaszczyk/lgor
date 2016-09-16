#include "Sector.h"
#include <iostream>
#include "PrintDef.h"



using namespace std;


Sector::Sector ( ModelData* mData ) : mdata ( mData ),printLatex ( false )
{
}

Sector::Sector ( int ktwist, vector<int> twist, ModelData* mData ) : mdata ( mData ),  Ktwist(ktwist), Twist(twist) ,printLatex ( false )
{
	if ( TrueMod ( ktwist , 2 ) == 0 )
	{
		Cmplx.resize ( 4,Complex ( mdata ) );
		Ramond = true;
	}
	else
	{
		Cmplx.resize ( 5,Complex ( mdata ) );
		Ramond = false;
	}
}



/* *******************************************************************************************
		INITIALIZATONAL METHODS
******************************************************************************************* */

void Sector::ReadTwists()
{
	cout << "Enter the K-twist and the " << mdata->NrDiscretes << " discrete twists: \n";
	cin >> Ktwist;
	Twist.resize ( mdata->NrDiscretes );
	for ( int i = 0; i<mdata->NrDiscretes; i++ )
		cin >> Twist[i];
	if ( TrueMod ( Ktwist , 2 ) == 0 )
	{
		Cmplx.resize ( 4,Complex ( mdata ) );
		Ramond = true;
	}
	else
	{
		Cmplx.resize ( 5,Complex ( mdata ) );
		Ramond = false;
	}
}



void Sector::SetTwists ( int a, vector<int> b )
{
	Ktwist = a;
	Twist.resize ( b.size() );
	for ( unsigned int i=0;i<b.size();i++ )
	{
		Twist[i]=b[i];
	}
	if ( TrueMod ( a , 2 ) == 0 )
	{
		Cmplx.resize ( 4,Complex ( mdata ) );
		Ramond = true;
	}
	else
	{
		Cmplx.resize ( 5,Complex ( mdata ) );
		Ramond = false;
	}
}

void Sector::CreateSFields()
{
	SField empty ( mdata );
	for ( int i=0;i<mdata->NrFields;i++ )
	{
		SFld.push_back ( empty );
		SFld[i].ComputeTwists ( mdata->Fld[i].Kcharge, mdata->Fld[i].Charge, Ktwist, Twist );
		SFld[i].Identifier = i;
	}
}


void Sector::PrintFields ( bool pTwist, bool pMaxPower )
{
	if ( pTwist )
	{
		cout << "Fields Twists in Sector ( " << Ktwist << " ; ";
		for ( int j = 0; j<mdata->NrDiscretes; j++ )
		{
			if ( j!=0 )
				cout << " , ";
			cout << Twist[j];
		}
		cout <<  " ) : \n";
		cout << " All to be divided by " << mdata->OrderHCF << "\n\n";
		for ( int i = 0; i<mdata->NrFields; i++ )
		{
			cout << SuperFieldName << mdata->Fld[i].Name << "\t\t" ;
			cout << SFld[i].BosonTwist << '\t' <<  SFld[i].BBosonTwist <<  '\t' <<  SFld[i].FermionTwist <<  '\t' <<  SFld[i].BFermionTwist << '\n';
		}
	}
	if ( pMaxPower )
	{
		cout << "Fields Maximum Powers in Sector ( " << Ktwist << " ; ";
		for ( int j = 0; j<mdata->NrDiscretes; j++ )
		{
			if ( j!=0 )
				cout << " , ";
			cout << Twist[j];
		}
		cout <<  " ) : \n";
		for ( int i = 0; i<mdata->NrFields; i++ )
		{
			cout << SuperFieldName << mdata->Fld[i].Name << "\t\t" ;
			cout << SFld[i].MaxPowerB << '\t' <<  SFld[i].MaxPowerBB <<  '\t' <<  SFld[i].MaxPowerF <<  '\t' <<  SFld[i].MaxPowerBF <<  '\t' <<  SFld[i].FakeMaxPowerF <<  '\t' <<  SFld[i].FakeMaxPowerBF << '\n';
		}
	}
	cout << '\n';
}





void  Sector::ComputeVacuumQuantumNumbers()
{
	int i,j;
	Energy = - mdata->OrderHCF * mdata->OrderHCF * 5 / 4;
	QuLeft = - mdata->KOrder * mdata->OrderHCF * 3 / 2 ;
	QuRight = -mdata->KOrder * mdata->OrderHCF * 3 / 2;
	Qu.resize ( mdata->NrDiscretes,0 );
	for ( i = 0; i < mdata->NrFields; i++ )
	{
		Energy += SFld[i].BosonTwist * ( mdata->OrderHCF - SFld[i].BosonTwist ) +  SFld[i].FermionTwist * ( mdata->OrderHCF + SFld[i].FermionTwist );
		QuLeft += ( mdata->Fld[i].Kcharge -  mdata->KOrder ) *  SFld[i].FermionTwist - mdata->Fld[i].Kcharge * SFld[i].BosonTwist;
		QuRight -= ( mdata->Fld[i].Kcharge -  mdata->KOrder ) * SFld[i].BosonTwist - mdata->Fld[i].Kcharge *  SFld[i].FermionTwist;
		for ( j = 0; j < mdata->NrDiscretes; j++ )
			Qu[j] +=  mdata->Fld[i].Charge[j] * ( SFld[i].FermionTwist - SFld[i].BosonTwist  + mdata->OrderHCF );
	}

	if ( Ktwist%2==0 )
		Energy =0;
	if ( Energy % ( 2 * mdata->OrderHCF ) != 0 )
		cout << " \n\n\n\n\n\n\n\n\n\n\n\n  Something is wrong with the quantization of the Energy!!!!!!!! \n " << Energy << " not divisible by " << 2 * mdata->OrderHCF << '\n';
	Energy /= 2 * mdata->OrderHCF;
	if ( ( 2 * QuLeft ) % mdata->OrderHCF != 0 )
		cout << " Buuuaaaeeeeeeeee  \n\n\n\n\n\n";
	if ( ( 2 * QuRight ) % mdata->OrderHCF != 0 )
		cout << " Buuuaaaeeeeeeeee  \n\n\n\n\n\n";
	QuLeft /= mdata->OrderHCF / 2 ;
	QuRight /= mdata->OrderHCF / 2 ;

	for ( j = 0; j < mdata->NrDiscretes; j++ )
	{
		if ( Qu[j] % mdata->OrderHCF != 0 ) cout << "Schabalababaeaeaeaeae HAHAHAHAHAHAHAHA\n\n\n\n\n";
		Qu[j] /= mdata->OrderHCF;
		Qu[j] = TrueMod ( Qu[j] , mdata->Order[j] );
	}
}


void Sector::ComputeMaxPowers()
{
	for ( int i = 0; i<mdata->NrFields; i++ )
		SFld[i].ComputeMaxPower ( Energy );
}

/* *******************************************************************************************
		PRINT METHODS
******************************************************************************************* */

void  Sector::PrintVacuumQuantumNumbers()
{
	cout << "Vacuum Energy \t\t" << Energy << " / " <<  mdata->OrderHCF << '\n';
	cout << "Left Moving R-Charge  \t" << QuLeft << " / " << 2 * mdata->KOrder <<  '\n';
	cout << "Right Moving R-Charge  \t" << QuRight << " / " << 2 * mdata->KOrder << '\n';
	cout << "Discrete Charges \t( ";
	for ( int j = 0; j<mdata->NrDiscretes; j++ )
	{
		if ( j!=0 )
			cout << " , ";
		cout << Qu[j] << " / " << mdata->Order[j] ;
	}
	cout << " ) \n";
}


void Sector::PrintDimensions()
{
	int i,j;
	cout << "Dimensions of Vector spaces before taking Cohomology \n";
	if ( Ramond )
	{
		cout << '\t';
		for ( j=0;j<6; j++ )
			cout << 2 * j - 5 << "/2 \t";
		cout << '\n';
		for ( i=0;i<4; i++ )
		{
			cout << 2 * i - 3 << "/2 \t";
			for ( j=0;j<6; j++ )
				cout << Cmplx[i].part[j].term.size() << '\t' ;
			cout << '\n';
		}
	}
	else
	{
		cout << '\t';
		for ( j=0;j<6; j++ )
			cout << 2 * j - 5 << "/2 \t";
		cout << '\n';
		for ( i=0;i<5; i++ )
		{
			cout <<  i - 2 << "\t";
			for ( j=0;j<6; j++ )
				cout << Cmplx[i].part[j].term.size() << '\t' ;
			cout << '\n';
		}
	}
	cout << '\n';
}



void Sector::PrintCohomologyDimensions()
{
	int i,j;
	cout << "Dimensions of Cohomology Spaces\n";
	if ( Ramond )
	{
		cout << "\t\t";
		for ( j=0;j<4; j++ )
			cout << 2 * j - 3 << "/2 \t";
		cout << '\n';
		for ( i=0;i<4; i++ )
		{
			cout << 2 * i - 3 << "/2 \t\t";
			for ( j=0;j<4; j++ )
				cout << Cmplx[i].cohomology[j].term.size() << '\t' ;
			cout << '\n';
		}
	}
	else
	{
		cout << "\t\t";
		for ( j=0;j<4; j++ )
			cout << 2 * j - 3 << "/2 \t";
		cout << '\n';
		for ( i=0;i<5; i++ )
		{
			cout <<  i - 2 << "\t\t";
			for ( j=0;j<4; j++ )
				cout << Cmplx[i].cohomology[j].term.size() << '\t' ;
			cout << '\n';
		}
	}
	cout << '\n';
}


void Sector::PrintModuliDimensions()
{
	cout << "Dimensions of Moduli Spaces\n";
	cout << "\t\t\t-1/2 \t1/2" << endl;
	cout << "Kaehler \t\t"<< ModuliSpace[0][0].term.size() << '\t' <<  ModuliSpace[0][1].term.size() << '\n';
	cout << "Complex Structure \t"<< ModuliSpace[1][0].term.size() << '\t' <<  ModuliSpace[1][1].term.size() << '\n';
	cout << "Bundle \t\t\t"<< ModuliSpace[2][0].term.size() << '\t' <<  ModuliSpace[2][1].term.size() << '\n';

}


void Sector::PrintStates ( int QuL,int QuR )
{
	if ( QuL < 0 || QuR < 0 || QuL >= ( int ) Cmplx.size() || QuR > 5 )
		return;
	cout << "All States with q_+ = " << 2 * QuR - 5 << " / 2 and q_- = ";
	if ( Ramond )
		cout << 2 * QuL - 3 << " / 2 \n";
	else
		cout << QuL - 2 << "\n";
	for ( int i = 0; i < ( int ) Cmplx[QuL].part[QuR].term.size(); i++ )
	{
		Cmplx[QuL].part[QuR].term[i].Print();
		cout << "\n";
	}
}


void Sector::PrintCohomologyStates ( int QuL,int QuR )
{
	if ( QuL < 0 || QuR < 0 || QuL >= ( int ) Cmplx.size() || QuR > 3 )
		return;
	cout << "All Cohomology States with q_+ = " << 2 * QuR - 3 << " / 2 and q_- = ";
	if ( Ramond )
		cout << 2 * QuL - 3 << " / 2 \n";
	else
		cout << QuL - 2 << "\n";
	for ( int i = 0; i < ( int ) Cmplx[QuL].cohomology[QuR].term.size(); i++ )
	{
		Cmplx[QuL].cohomology[QuR].term[i].Print ( false );
		cout << "\n";
	}
}


void Sector::PrintModuliStates ( int QuL,int QuR )
{
	if ( QuL < 0 || QuR < 0 || QuL >= 3 || QuR > 2 )
		return;
	cout << "All ";
	switch ( QuL )
	{
		case 0:
			cout << "Kaehler";
			break;
		case 1:
			cout << "Complex Structure";
			break;
		case 2:
			cout << "Bundle";
			break;
	}
	cout << " Moduli with q_+ = " << 2 * QuR - 1 << " / 2 \n";
	for ( unsigned int i = 0; i <  ModuliSpace[QuL][QuR].term.size(); i++ )
	{
		ModuliSpace[QuL][QuR].term[i].Print ( false );
		cout << "\n";
	}
}



void Sector::PrintFieldsLatex ( bool pTwist, bool pMaxPower )
{
	if ( pTwist )
	{
		cout << "Fields Twists in Sector ( " << Ktwist << " ; ";
		for ( int j = 0; j<mdata->NrDiscretes; j++ )
		{
			if ( j!=0 )
				cout << " , ";
			cout << Twist[j];
		}
		cout <<  " ) : \n";
		cout << " All already divided by " << mdata->OrderHCF << "\n\n";
		cout << "\\begin{array}{c|ccc} Fields & $k$-charge & $k_1$-charge & $k_2$-charge & $k_3$-charge \\\\ \\hline  \n";
		for ( int i = 0; i<mdata->NrFields; i++ )
		{
			cout << SuperFieldName << mdata->Fld[i].Name << "\t\t" << "& " ;
			cout << SFld[i].BosonTwist / mdata->OrderHCF << '\t' << "&" <<  SFld[i].BBosonTwist / mdata->OrderHCF <<  '\t' << "&" <<  SFld[i].FermionTwist /mdata->OrderHCF <<  '\t' << "&" <<  SFld[i].BFermionTwist  /mdata->OrderHCF << "\\\\"  << '\n';
		}
	}
	cout << "\\hline \\end{array}";
	if ( pMaxPower )
	{
		cout << "Fields Maximum Powers in Sector ( " << Ktwist << " ; ";
		for ( int j = 0; j<mdata->NrDiscretes; j++ )
		{
			if ( j!=0 )
				cout << " , ";
			cout << Twist[j];
		}
		cout <<  " ) : \n";
		for ( int i = 0; i<mdata->NrFields; i++ )
		{
			cout << SuperFieldName << mdata->Fld[i].Name << "\t\t" ;
			cout << SFld[i].MaxPowerB << '\t' <<  SFld[i].MaxPowerBB <<  '\t' <<  SFld[i].MaxPowerF <<  '\t' <<  SFld[i].MaxPowerBF <<  '\t' <<  SFld[i].FakeMaxPowerF <<  '\t' <<  SFld[i].FakeMaxPowerBF << '\n';
		}
	}
	cout << '\n';
}




void  Sector::PrintVacuumQuantumNumbersLatex()
{
	cout << "Vacuum Quantum Numbers: \n ";
	cout << "\\begin{array}{lc} \\\\ \\hline \n";
	cout << "Vacuum Energy \t\t &" << Energy << " / " <<  mdata->OrderHCF << " \\\\  \n";
	cout << "Left Moving R-Charge  \t &" << QuLeft << " / " << 2 * mdata->KOrder <<  " \\\\  \n";
	cout << "Right Moving R-Charge  \t &" << QuRight << " / " << 2 * mdata->KOrder <<  " \\\\ \n";
	cout << "Discrete Charges \t & (  ";
	for ( int j = 0; j<mdata->NrDiscretes; j++ )
	{
		if ( j!=0 )
			cout << " , ";
		cout << Qu[j] << " / " << mdata->Order[j] ;
	}
	cout << " ) \\\\ \\hline end{array} \n\n";
}



void Sector::PrintDimensionsLatex()
{
	int i,j;
	cout << " \n Dimensions of Vector spaces before taking Cohomology \n";
	cout << " \\begin{array}{c|cccccc} \n $q_- / q_+$  ";
	if ( Ramond )
	{
		cout << '\t';
		for ( j=0;j<6; j++ )
			cout << 2 * j - 5 << "/2 & \t";
		cout << '\n';
		for ( i=0;i<4; i++ )
		{
			cout << 2 * i - 3 << "/2 & \t";
			for ( j=0;j<6; j++ )
				cout << Cmplx[i].part[j].term.size() << "&" <<'\t' ;
			cout << '\n';
		}
	}
	else
	{
		cout << '\t';
		for ( j=0;j<6; j++ )
			cout << " & " << 2 * j - 5 << "/2  \t";
		cout << " \\\\"<< '\n';
		for ( i=0;i<5; i++ )
		{
			cout << i - 2 << " \t";
			for ( j=0;j<6; j++ )
				cout << "&" << Cmplx[i].part[j].term.size() << '\t' ;
			cout << " \\\\ ";
			cout << '\n';
		}
	}
	cout << '\n';
	cout << " \\end{array} \n ";
}


void Sector::PrintCohomologyDimensionsLatex()
{
	int i,j;
	cout << " \n Dimensions of Cohomology spaces \n";
	cout << " \\begin{array}{c|cccccc} \n $q_- / q_+$  ";
	if ( Ramond )
	{
		cout << '\t';
		for ( j=0;j<4; j++ )
			cout << 2 * j - 3 << "/2 & \t";
		cout << '\n';
		for ( i=0;i<4; i++ )
		{
			cout << 2 * i - 3 << "/2 & \t";
			for ( j=0;j<6; j++ )
				cout << Cmplx[i].cohomology[j].term.size() << "&" <<'\t' ;
			cout << '\n';
		}
	}
	else
	{
		cout << '\t';
		for ( j=0;j<4; j++ )
			cout << " & " << 2 * j - 3 << "/2  \t";
		cout << " \\\\"<< '\n';
		for ( i=0;i<5; i++ )
		{
			cout << i - 2 << " \t";
			for ( j=0;j<6; j++ )
				cout << "&" << Cmplx[i].cohomology[j].term.size() << '\t' ;
			cout << " \\\\ ";
			cout << '\n';
		}
	}
	cout << '\n';
	cout << " \\end{array} \n ";
}



/* *******************************************************************************************
		COMPUTATIONAL METHODS
******************************************************************************************* */



int Sector::FindMasslessNeutralMonomials()  // A rather non-trivial functon  ***********************************************
{
	if(Energy > 0)
		return 0;
	int i, NrNeutrals = 0;
	int QuR, QuL;

// First we divide Bosons and Fermions into Zero and NonZero Modes ********************************************************

	vector<SField> ZeroBoson;
	vector<SField> NonZeroBoson;
	vector<SField> ZeroFermion;
	vector<SField> NonZeroFermion;
	for ( i=0; i< mdata->NrFields; i++ )
	{
		if ( SFld[i].BosonTwist == 0 )
			ZeroBoson.push_back ( SFld[i] );
		else
			NonZeroBoson.push_back ( SFld[i] );
		if ( SFld[i].FermionTwist == 0 )
			ZeroFermion.push_back ( SFld[i] );
		else
			NonZeroFermion.push_back ( SFld[i] );
	}

	int NrZeroBosons=ZeroBoson.size();
	int NrNonZeroBosons=NonZeroBoson.size();
	int NrZeroFermions=ZeroFermion.size();
	int NrNonZeroFermions=NonZeroFermion.size();

// Alls Non Zero Fields may only appear MaxMaxPower times *************************************************************

	int MaxMaxPower=0;
	for ( i=0;i<NrNonZeroBosons;i++ )
	{
		if ( MaxMaxPower < NonZeroBoson[i].MaxPowerB )
			MaxMaxPower = NonZeroBoson[i].MaxPowerB;
		if ( MaxMaxPower < NonZeroBoson[i].MaxPowerBB )
			MaxMaxPower = NonZeroBoson[i].MaxPowerBB;
	}
	for ( i=0;i<NrNonZeroFermions;i++ )
	{
		if ( MaxMaxPower < NonZeroFermion[i].FakeMaxPowerF )
			MaxMaxPower = NonZeroFermion[i].FakeMaxPowerF;
		if ( MaxMaxPower < NonZeroFermion[i].FakeMaxPowerBF )
			MaxMaxPower = NonZeroFermion[i].FakeMaxPowerBF;
	}
	int NrNonZeros = 2 * ( NrNonZeroFermions + NrNonZeroBosons );

// Now we scan through all operators which are monomials in the non-zero fields of maximum total power MaxMaxPower *************

	vector<int> NZFieldPointer ( MaxMaxPower,-1 );
	vector<int> ZBosonPointer ( NrZeroBosons );
	vector<int> ZFermionPointer ( NrZeroFermions );

	int HighestCarry, Index, TesterEnergy;
	bool engage = true, ZFengage, ZBengage, FermiKill;
	bool Neutral;
	Monomial Tester ( mdata );
	Monomial ZeroTester ( mdata );



	while ( engage )    //   Here starts the Giant Loop  ****************************************************************************
	{
//     Create A Monomial in the Nonzero Modes  *************************************************************************************

		Tester.ClearPowers();
		for ( i=0;i<MaxMaxPower;i++ )
		{
			Index = NZFieldPointer[i];
			if ( Index >= 0 )
			{
				if ( Index < NrNonZeroBosons )
					Tester.BPower[NonZeroBoson[Index].Identifier]++;
				else
				{
					Index -= NrNonZeroBosons;

					if ( Index < NrNonZeroBosons )
						Tester.BBPower[NonZeroBoson[Index].Identifier]++;
					else
					{
						Index -= NrNonZeroBosons;

						if ( Index < NrNonZeroFermions )
							Tester.FPower[NonZeroFermion[Index].Identifier]++;
						else
						{
							Index -= NrNonZeroFermions;
							Tester.BFPower[NonZeroFermion[Index].Identifier]++;
						}
					}
				}
			}
		}

		//// Check if it has zero Energy -> Singlett  OR  Energy -1/2 -> Tenplet  OR energy -1 -> adjoint, gravitino, ....   ****************************

		FermiKill=false;
		for ( i=0; i<mdata->NrFields; i++ )
			if ( Tester.FPower[i] > SFld[i].MaxPowerF || Tester.BFPower[i] > SFld[i].MaxPowerBF )
				FermiKill = true;
		TesterEnergy = Tester.Energy ( &SFld[0],Energy ) ;
		if ( ( TesterEnergy ==0 || TesterEnergy == - mdata->OrderHCF/2 || TesterEnergy == - mdata->OrderHCF ) && ! FermiKill )
		{


			// Now go through monomial operators in the Zero Modes to create neutral States. For this start a less giant fermionic Loop   *******************
			ZFermionPointer.clear();
			ZFermionPointer.resize ( NrZeroFermions,0 );
			ZFengage=true;
			while ( ZFengage )
			{
				// And the same for the Zero Bosons *********************************************  *******************
				ZBosonPointer.clear();
				ZBosonPointer.resize ( NrZeroBosons,0 );
				ZBengage=true;
				while ( ZBengage )
				{

					//// Create a Monomial in Zero Modes

					ZeroTester.CopyFrom ( &Tester );
					for ( i=0; i<NrZeroFermions; i++ )
						ZeroTester.BFPower[ZeroFermion[i].Identifier] = ZFermionPointer[i];
					for ( i=0; i<NrZeroBosons; i++ )
						ZeroTester.BPower[ZeroBoson[i].Identifier] = ZBosonPointer[i];

					// Check if the Monomial is Neutral

					Neutral = true;
					for ( i=0; i<mdata->NrDiscretes; i++ )
					{

						if ( TrueMod ( ZeroTester.Charge ( i,Qu[i] ) , mdata->Order[i] ) != 0 )
						{
							Neutral = false;
						}
					}
					if ( Ramond && TrueMod ( ZeroTester.QuLeft ( QuLeft ) + mdata->KOrder , 2 * mdata->KOrder ) != 0 )
						Neutral = false;
					if ( !Ramond && TrueMod ( ZeroTester.QuLeft ( QuLeft ) , 2 * mdata->KOrder ) != 0 )
						Neutral = false;
					if ( TrueMod ( ZeroTester.QuRight ( QuRight ) + mdata->KOrder, 2 * mdata->KOrder ) != 0 )
						Neutral = false;
					if ( Neutral )  // If the Monomial is indeed Neutral, we add it to the respective place in the respective Complex
					{
						if ( TesterEnergy == - mdata->OrderHCF )
						{
							cout << "There are gravitini and E_8 + SO(10) gaugini of Q+ Charge " << ZeroTester.QuRight ( QuRight ) /  mdata->KOrder  <<  " / 2 in sector ( " << Ktwist ;
							for ( i=0; i<mdata->NrDiscretes; i++ )
								cout << " , " << Twist[i] ; 
							cout << " )" << endl;				
						}
						else
						{
							NrNeutrals ++;
							QuR = ( ZeroTester.QuRight ( QuRight ) + 5 * mdata->KOrder ) / ( 2 * mdata->KOrder );
							if ( Ramond )
								QuL = ( ZeroTester.QuLeft ( QuLeft ) + 3 * mdata->KOrder ) / ( 2 * mdata->KOrder );
							else
								QuL = ( ZeroTester.QuLeft ( QuLeft ) + 4 * mdata->KOrder ) / ( 2 * mdata->KOrder );
							if ( QuL<0 || QuR < 0 || QuL >= ( int ) Cmplx.size() || QuR > 5 )
							{
								cout << "Quantum Numbers exceed common range: " << QuL << ' ' << QuR << '\n';
								ZeroTester.Print();
								cout << '\n';
							}
							else
								Cmplx[QuL].part[QuR].term.push_back ( ZeroTester );
						}
					}

					//Yeah, Zero Boson loop ends   ****************************************

					if ( NrZeroBosons!=0 )
					{
						ZBosonPointer[0]++;
						for ( i=0; i<NrZeroBosons-1; i++ )
							if ( ZBosonPointer[i] > ZeroBoson[i].MaxPowerB )
							{
								ZBosonPointer[i]=0;
								ZBosonPointer[i+1]++;
							}
						if ( ZBosonPointer[NrZeroBosons-1] > ZeroBoson[NrZeroBosons-1].MaxPowerB )
							ZBengage = false;
					}
					else
						ZBengage=false;
				}

				// Increment the Zero Fermion MultiPointer and check if less Giant loop ends   ****************************************

				if ( NrZeroFermions!=0 )
				{
					ZFermionPointer[0]++;
					for ( i=0; i<NrZeroFermions-1; i++ )
						if ( ZFermionPointer[i] > ZeroFermion[i].MaxPowerBF )
						{
							ZFermionPointer[i]=0;
							ZFermionPointer[i+1]++;
						}
					if ( ZFermionPointer[NrZeroFermions-1] > ZeroFermion[NrZeroFermions-1].MaxPowerBF )
						ZFengage = false;
				}
				else
					ZFengage=false;
			}
		}

		//*********Increment the NonZeroField-MultiPointer    and check if the Giant Loop can end   ***********************************************

		if ( MaxMaxPower>0 )
		{
			NZFieldPointer[0]++;
			HighestCarry = -1;
			for ( i = 0; i < MaxMaxPower-1; i ++ )
			{
				if ( NZFieldPointer[i] == NrNonZeros )
				{
					NZFieldPointer[i+1]++;
					HighestCarry++;
				}
			}
			for ( i = HighestCarry; i>=0 ; i-- )
				NZFieldPointer[i] = NZFieldPointer[i+1];
			if ( NZFieldPointer[MaxMaxPower-1]==NrNonZeros )
				engage = false;
		}
		else
			engage = false;
	}

//	cout << "There are "<< NrNeutrals << " Neutral Fields in Sector ( " << Ktwist ;
//	for ( i=0; i<mdata->NrDiscretes; i++ )
//		cout << " , " << Twist[i] ;
//	cout << " )\n";
	return NrNeutrals;
}

void Sector::CreateGGbar()
{
	this->G.term.clear();
	this->GBar.term.clear();
	this->G.Koeff.clear();
	this->GBar.Koeff.clear();
	if ( Ramond )
	{
		cout << " You do not need this operator in a Ramond sector! \n";
		return;
	}
	G.term.reserve ( 2 * mdata->NrFields );
	G.Koeff.reserve ( 2 * mdata->NrFields );
	GBar.term.reserve ( 2 * mdata->NrFields );
	GBar.Koeff.reserve ( 2 * mdata->NrFields );
	int i;
	Monomial Pusher ( mdata );
	for ( i = 0 ; i < mdata->NrFields; i++ )
	{
		if ( SFld[i].BosonTwist <= mdata->OrderHCF/2 )
		{
			Pusher.ClearPowers();
			Pusher.BPower[i]++;
			Pusher.BFPower[i]++;
			GBar.term.push_back ( Pusher );
			GBar.Koeff.push_back ( 1 );
			Pusher.ClearPowers();
			Pusher.BBPower[i]++;
			Pusher.BFPower[i]--;
			G.term.push_back ( Pusher );
			G.Koeff.push_back ( 1 );
		}
		if ( SFld[i].BosonTwist < mdata->OrderHCF/2 )
		{
			Pusher.ClearPowers();
			Pusher.BPower[i]--;
			Pusher.FPower[i]++;
			G.term.push_back ( Pusher );
			G.Koeff.push_back ( 1 );
		}
		if ( SFld[i].BosonTwist > mdata->OrderHCF/2 )
		{
			Pusher.ClearPowers();
			Pusher.BBPower[i]++;
			Pusher.FPower[i]++;
			G.term.push_back ( Pusher );
			G.Koeff.push_back ( 1 );
			Pusher.ClearPowers();
			Pusher.BBPower[i]--;
			Pusher.BFPower[i]++;
			GBar.term.push_back ( Pusher );
			GBar.Koeff.push_back ( 1 );
			Pusher.ClearPowers();
			Pusher.BPower[i]++;
			Pusher.FPower[i]--;
			GBar.term.push_back ( Pusher );
			GBar.Koeff.push_back ( 1 );
		}
	}
	for ( i = 0 ; i < ( int ) G.term.size(); i++ )
		if ( G.term[i].Energy ( &SFld[0],0 ) != mdata->OrderHCF / 2 )
			cout << "\n\n\n A term in G_-1/2 has wrong energy\n\n\n";
	for ( i = 0 ; i < ( int ) GBar.term.size(); i++ )
		if ( GBar.term[i].Energy ( &SFld[0],0 ) != mdata->OrderHCF / 2 )
			cout << "\n\n\n A term in \barG_-1/2 has wrong energy\n\n\n";
}

void Sector::IdentifySinglets()
{
	if ( Ramond )
		return;
	unsigned int i,j,k;
	unsigned int NrBundleModuli,NrModuli;
	Polynomial Pusher;
	vector<int> MatrixLine;
	Matrix Mat1,Mat2,Mat3;
	PolySpace Basis;
	for ( k = 0 ; k < 2 ; k ++ )
	{
		for ( i = 0 ; i < 3 ; i++ )
			ModuliSpace[i][k].term.clear();
		for ( i = 0 ; i < Cmplx[1].cohomology[k+1].term.size(); i++ )
		{
			Cmplx[1].GGBarAction ( &GBar , &Cmplx[1].cohomology[k+1].term[i] , &Pusher );
			ModuliSpace[0][k+0].term.push_back ( Pusher );
		}// Find Kaehler Moduli
		for ( i = 0 ; i < Cmplx[3].cohomology[k+1].term.size(); i++ )
		{
			Cmplx[3].GGBarAction ( &G , &Cmplx[3].cohomology[k+1].term[i] , &Pusher );
			ModuliSpace[1][k+0].term.push_back ( Pusher );
		}// Find Cmplx Str Moduli
		Basis.term.clear();
		for ( i = 0 ; i < Cmplx[2].cohomology[k+1].term.size(); i++ )
			for ( j = 0 ; j < Cmplx[2].cohomology[k+1].term[i].term.size() ; j++ )
				Basis.term.push_back ( Cmplx[2].cohomology[k+1].term[i].term[j] );
		Basis.ReduceBasis();
		for ( i = 0 ; i < Basis.term.size() ; i++ )
			Basis.term[i].Print();
		Mat1.SetNrRows ( Basis.term.size() );
		for ( i = 0 ; i < Cmplx[2].cohomology[k+1].term.size(); i++ )
		{
			Cmplx[2].cohomology[k+1].term[i].VectorForm ( &Basis.term,&MatrixLine );
			Mat1.push_col ( MatrixLine );
		}
		if ( !Mat1.BuildLeftDiagonalizer ( &Mat2 ) )
		{
			cout << " Something is horribly wrong!!! \n\n\n\n" << j << endl;
			Mat1.Print();
		}
		Mat1.Clear();
		Mat1.SetNrRows ( Basis.term.size() );
		for ( i = 0 ; i < ModuliSpace[0][k+0].term.size(); i++ )
		{
			ModuliSpace[0][k+0].term[i].VectorForm ( &Basis.term,&MatrixLine );
			Mat1.push_col ( MatrixLine );
		}
		for ( i = 0 ; i < ModuliSpace[1][k+0].term.size(); i++ )
		{
			ModuliSpace[1][k+0].term[i].VectorForm ( &Basis.term,&MatrixLine );
			Mat1.push_col ( MatrixLine );
		}
		if ( !Mat3.IsProductOf ( &Mat2,&Mat1 ) )
			cout << "Matrix Dimensions Do not Match" << endl;
		Mat3.Transpose();
		NrBundleModuli = Mat3.FindKernel ( &Mat1 );
		NrModuli = Cmplx[2].cohomology[k+1].term.size();
		for ( i = 0 ; i < NrBundleModuli; i++ )
		{
			Mat1.GiveRow ( i,&MatrixLine );
			MatrixLine.resize ( NrModuli );
			Pusher.FromVector ( &Cmplx[2].cohomology[k+1].term, &MatrixLine );
			ModuliSpace[2][k+0].term.push_back ( Pusher );
		}
	}
}

void Sector::CreateQBar()
{
	this->QBar.term.clear();
	this->QBar.Koeff.clear();
	int i,j,k;
	vector<int> MultiPointer;
	Monomial Tester ( mdata ),Basic ( mdata );
	bool engage;
	vector<int> Power ( mdata->NrFields );
	int NrDestroyers, highestCarry, Koeff;
	for ( i = 0; i < mdata->NrSuperpotTerms; i++ )
	{
		for ( j=0; j < mdata->NrFields ; j++ )
			Power[j] = mdata->SuperPotential.term[i].Power[j];
		for ( j = 0; j < mdata->NrFields ; j++ )
		{
			if ( Power[j] > 0 )
			{
				Basic.ClearPowers();
				for ( k=0; k<mdata->NrFields; k++ )
					Basic.BPower[k] = Power[k];
				Basic.BPower[j]--;
				if ( SFld[j].FermionTwist == 0 )
					Basic.BFPower[j]--;
				else
					Basic.FPower[j]++;
// Initialize a giant loop, searching for Qbar zero modes
				NrDestroyers = Basic.Energy ( &SFld[0],0 ) / mdata->OrderHCF;
				MultiPointer.resize ( 0 );
				MultiPointer.resize ( NrDestroyers,0 );
				engage = true;
				while ( engage )
				{
					Tester.CopyFrom ( &Basic );
					for ( k = 0 ; k < NrDestroyers ; k++ )
					{
						Tester.BPower[MultiPointer[k]]--;
						Tester.BBPower[MultiPointer[k]]--;
// Note: the convention is that a negative power corresponds to a positive power in the conjugated operator
					}
					if ( Tester.Holomorphic() )
					{
						QBar.term.push_back ( Tester );
						Koeff = Power[j] * mdata->SuperPotential.term[i].Koeff;
						for ( k = 0; k < mdata->NrFields; k ++ )
							Koeff *= choose ( Tester.BPower[k] - Tester.BBPower[k] , Tester.BPower[k] );
						QBar.Koeff.push_back ( Koeff );
					}
					if ( NrDestroyers > 0 ) // Increment Multipointer
					{
						MultiPointer[0]++;
						highestCarry = -1;
						for ( k = 0; k < NrDestroyers-1; k++ )
						{
							if ( MultiPointer[k] == mdata->NrFields )
							{
								MultiPointer[k+1]++;
								highestCarry++;
							}
						}
						for ( k = highestCarry; k >=0 ; k-- )
							MultiPointer[k] = MultiPointer[k+1];
						if ( MultiPointer[NrDestroyers-1] == mdata->NrFields )
							engage = false;
					}
					else
						engage = false;
				}
// If the Fermion Does not have a zero mode, its destroyer mode has negative energy and has to be added separately
				if ( SFld[j].FermionTwist != 0 )
				{
					Basic.FPower[j]--;
					Basic.BFPower[j]--;
					NrDestroyers = Basic.Energy ( &SFld[0],0 ) / mdata->OrderHCF;
					MultiPointer.resize ( 0 );
					MultiPointer.resize ( NrDestroyers,0 );
					engage = true;
					while ( engage )
					{
						Tester.CopyFrom ( &Basic );
						for ( k = 0 ; k < NrDestroyers ; k++ )
						{
							Tester.BPower[MultiPointer[k]]--;
							Tester.BBPower[MultiPointer[k]]--;
						}
						if ( Tester.Holomorphic() )
						{
							QBar.term.push_back ( Tester );
							Koeff = Power[j] * mdata->SuperPotential.term[i].Koeff;
							for ( k = 0; k < mdata->NrFields; k ++ )
								Koeff *= choose ( Tester.BPower[k] - Tester.BBPower[k] , Tester.BPower[k] );
							QBar.Koeff.push_back ( Koeff );
						}
						if ( NrDestroyers > 0 ) // Increment Multipointer
						{
							MultiPointer[0]++;
							highestCarry = -1;
							for ( k = 0; k < NrDestroyers-1; k++ )
							{
								if ( MultiPointer[k] == mdata->NrFields )
								{
									MultiPointer[k+1]++;
									highestCarry++;
								}
							}
							for ( k = highestCarry; k >=0 ; k-- )
								MultiPointer[k] = MultiPointer[k+1];
							if ( MultiPointer[NrDestroyers-1] == mdata->NrFields )
								engage = false;
						}
						else
							engage = false;
					}
				}
			}
		}
	}
}

void Sector::CreateQBarMatrices()
{
	int i;
	for ( i = 0; i < ( int ) Cmplx.size() ; i ++ )
		Cmplx[i].CreateQBarMatrices ( QBar );
}

bool Sector::ShrinkProblems()
{
	unsigned int i,j;
	for ( i = 0; i <  Cmplx.size() ; i ++ )
		Cmplx[i].ShrinkProblem();
	for ( i = 0 ; i < Cmplx.size() ; i ++ )
		for ( j = 0 ; j < Cmplx.size(); j++ )
			if ( Cmplx[i].shrunk[j].term.size() != 0 )
				return false;
	return true;
}


void Sector::ReplaceByShrunks()
{
	int i;
	for ( i = 0; i < ( int ) Cmplx.size() ; i ++ )
		Cmplx[i].ReplaceByShrunk();
}

void Sector::FuseQBars()
{
	int i;
	for ( i = 0; i < ( int ) Cmplx.size() ; i ++ )
		Cmplx[i].FuseRemainderQBars();
}

void Sector::KillMonomialPreimages()
{
	int i;
	for ( i = 0; i < ( int ) Cmplx.size() ; i ++ )
		Cmplx[i].KillMonomialPreimages();
}

void Sector::KillToDeath()
{
	FuseQBars();
	bool DimsEqual= false;
	int i,j;
	while ( !DimsEqual )
	{
		ReplaceByShrunks();
		KillMonomialPreimages();
		DimsEqual=true;
		for ( i=0; i < ( int ) Cmplx.size(); i++ )
			for ( j = 0; j < 6; j ++ )
				if ( Cmplx[i].part[j].term.size() != Cmplx[i].shrunk[j].term.size() )
					DimsEqual = false;
	}
}

void Sector::FindCohomology()
{
	unsigned int NrPhys = 0;
	unsigned int i,j;
	if ( !ShrinkProblems() )
	{
		FuseQBars();
//		KillToDeath();
		ReplaceByShrunks();
		for ( unsigned int i = 0 ; i < Cmplx.size(); i++ )
		{
			Cmplx[i].FindFusedQBarKernels();
		}
	}
	for ( i = 0 ; i < Cmplx.size(); i++ )
	{
		for ( j = 0 ; j < 6 ; j++ )
		{
			Cmplx[i].shrunk[j].term.clear();
			Cmplx[i].QBarRemainder[j].Clear();
//			Cmplx[i].part[j].term.clear();
//			Cmplx[i].QBarMatrix[j].Clear();
		}
		for ( j = 0 ; j < 4 ; j++ )
			NrPhys += Cmplx[i].cohomology[j].term.size();
	}
//	int k;
//	cout << "There are " << NrPhys << " physical Fields in Sector ( " << Ktwist ;
//	for ( k=0; k<mdata->NrDiscretes; k++ )
//		cout << " , " << Twist[k] ;
//	cout << " )" << endl;
}

void Sector::Initialize()
{
	CreateSFields();
	ComputeVacuumQuantumNumbers();
	ComputeMaxPowers();
	FindMasslessNeutralMonomials();
	CreateQBar();
	if ( !Ramond )
		CreateGGbar();
	CreateQBarMatrices();
	FindCohomology();
}

void Sector::run ( bool initialize )
{
	if ( initialize )
	{
		ReadTwists();
		Initialize();
	}
	int Row=0,Col=0;
	string command;
	bool engage = true;
	while ( engage )
	{
		cout << "lgor:/" << mdata->Name << "/" << Ktwist;
		for ( unsigned int i = 0 ; i < Twist.size(); i++ )
			cout << ',' << Twist[i];
		cout <<  ">";
		cin >> command;
		switch ( command[0] )
		{
			case 'x':
				engage = false;
				break;
			case 'h':
				cout << "Enter v to print vacuum properties \n";
				cout << "Enter f to print fields \n";
				cout << "Enter d to print dimensions of vector spaces \n";
				cout << "Enter b to print basis in vector space \n";
				cout << "Enter q to print QBar \n";
				if ( !Ramond )
					cout << "Enter g to print G and GBar \n";
				cout << "Enter m to print QBar in MatrixForm \n";
				cout << "Enter l to switch to ";
				if ( printLatex )
					cout << "normal";
				else
					cout << "LaTeX";
				cout << " print mode \n";
//				cout << "Enter c to compute Cohomology \n";
//				if ( !Ramond )
//					cout << "Enter s to identify singlets \n";

				cout << "Enter x to go back \n";
				break;
			case 'l':
				printLatex = ! printLatex;
				break;
			case 'v':
				if ( printLatex )
					PrintVacuumQuantumNumbersLatex();
				else
					PrintVacuumQuantumNumbers();
				break;
			case 'f':
				if ( printLatex )
					PrintFieldsLatex ( true, true );
				else
					PrintFields ( true, false );
				break;
			case 'd':
				if ( command.length() == 1 )
				{
					cout << "Enter dr for Raw Vector spaces \n";
					cout << "Enter dc for Cohomology spaces \n";
					cout << "Enter dm for Moduli spaces \n";
				}
				else
				{
					switch ( command[1] )
					{
						case 'r':
							if ( printLatex )
								PrintDimensionsLatex();
							else
								PrintDimensions();
							break;
						case 'c':
							if ( printLatex )
								PrintCohomologyDimensionsLatex();
							else
								PrintCohomologyDimensions();
							break;
						case 'm':
							PrintModuliDimensions();
					}
				}
				break;
			case 'b':
				if ( command.length() == 1 )
				{
					cout << "Enter br for Raw Vector spaces \n";
					cout << "Enter bc for Cohomology spaces \n";
					cout << "Enter bm for Moduli spaces \n";
				}
				else
				{
					if ( command.length() > 3 )
					{
						Row = ( int ) command[2] - '0';
						Col = ( int ) command[3] - '0';
					}
					else
					{
						cout << "Enter Row and Column: ";
						cin >> Row >> Col;
					}
					switch ( command[1] )
					{
						case 'r':
							PrintStates ( Row-1,Col-1 );
							break;
						case 'c':
							PrintCohomologyStates ( Row-1,Col-1 );
							break;
						case 'm':
							PrintModuliStates ( Row-1,Col-1 );
					}
				}
				break;
			case 'm':
				if ( command.length() > 2 )
				{
					Row = ( int ) command[1] - '0';
					Col = ( int ) command[2] - '0';
				}
				else
				{
					cout << "Enter Row and Column: ";
					cin >> Row >> Col;
				}
				Cmplx[Row-1].QBarMatrix[Col-1].Print();
				break;
			case 'q':
				QBar.Print ( true );
				break;
			case 'g':
				if ( Ramond )
					break;
				cout << " G = \n";
				G.Print ( true );
				cout << "\n GBar = \n";
				GBar.Print ( true );
				break;
//			case 'c':
//				FindCohomology();
//				break;
//			case 's':
//				IdentifySinglets();
//				break;
		}
	}
}
