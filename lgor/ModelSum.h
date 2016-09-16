#ifndef __LGOR_MODELSUM__
#define __LGOR_MODELSUM__

#include <fstream>
#include <vector>
#include "SpectrumSum.h"
#include "Bunch.h"


class ModelSum
{
	public:
	SpectrumSum MdlSum;
	std::vector< SpectrumSum > BnchSum;
	std::vector< unsigned int > BnchMulti;


	ModelSum();
	bool IsEqualTo(ModelSum*);
	bool CPTInvariant();
	void Create( std::vector< Bunch >* );
	void Clear();
	void Print();
	bool PrintToFile(std::ofstream &);
	bool ReadFromFile( char* );
	bool IsCorrect();

};

#endif
