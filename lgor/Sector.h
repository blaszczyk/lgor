#ifndef __LGOR_SECTOR__
#define __LGOR_SECTOR__



#include <string>
#include <vector>
#include "SField.h"
#include "Polynomial.h"
#include "Complex.h"
#include "SimpleMath.h"
#include "ModelData.h"


/* ****************************************************************************************************************************************

Next Class: The Sector!

**************************************************************************************************************************************** */


class Sector
{

	public:

		ModelData* mdata;

		int Ktwist;
		std::vector<int> Twist;

		bool Ramond;
		bool printLatex;

		std::vector<SField> SFld;

		int Energy;
		int QuLeft;
		int QuRight;
		std::vector<int> Qu;
		int MaxMaxPower;

		Polynomial QBar;
		Polynomial G;
		Polynomial GBar;

		std::vector<Complex> Cmplx;
		CohomSpace ModuliSpace[3][2];

		Sector ( ModelData* );
		Sector ( int,std::vector<int>, ModelData* );

		void ReadTwists();
		void SetTwists ( int,std::vector<int> );
		void CreateSFields();
		void ComputeVacuumQuantumNumbers();
		void ComputeMaxPowers();
		int FindMasslessNeutralMonomials();
		void CreateQBar();
		void CreateGGbar();
		void IdentifySinglets();

		void CreateQBarMatrices();
		bool ShrinkProblems();
		void ReplaceByShrunks();
		void FuseQBars();
		void KillMonomialPreimages();
		void KillToDeath();
		void FindCohomology();

		void run ( bool );
		void Initialize();

		void PrintFields ( bool,bool );
		void PrintVacuumQuantumNumbers();

		void PrintDimensions();
		void PrintCohomologyDimensions();
		void PrintModuliDimensions();

		void PrintStates ( int,int );
		void PrintCohomologyStates ( int,int );
		void PrintModuliStates ( int,int );

		void PrintFieldsLatex ( bool, bool );
		void PrintVacuumQuantumNumbersLatex();
		void PrintDimensionsLatex();
		void PrintCohomologyDimensionsLatex();

};



#endif
