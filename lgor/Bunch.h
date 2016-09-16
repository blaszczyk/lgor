#ifndef __LGOR_BUNCH__
#define __LGOR_BUNCH__


#include "Sector.h"
#include "ModelData.h"
#include "SpectrumSum.h"
/* ****************************************************************************************************************************************

A Bunch Of Sectors contains 2k sectors that form full E6 Multiplets

**************************************************************************************************************************************** */


class Bunch
{
	public:
		SpectrumSum Sum;
		ModelData* mdata;
		std::vector<int> Twist;
		std::vector<Matrix> Spectrum;
		std::vector<Sector> Sctr;

		bool empty;

		Bunch ( ModelData* );
		Bunch ( std::vector<int>,ModelData* );

		void ReadTwists();
		void CreateSectors();
		void SummarizeSpectrum();
		void Initialize();
		void CheckE6Consistency();
		void PrintSpectrum();
		void run ( bool );

};


#endif
