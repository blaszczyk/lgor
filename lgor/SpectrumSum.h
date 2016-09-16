#ifndef __LGOR_SPECTRUMSUM__
#define __LGOR_SPECTRUMSUM__

#include <fstream>




class SpectrumSum
{
	public:
	unsigned int NrModuli[4];
	unsigned int Nr27Plets[4];
	unsigned int Nr27BarPlets[4];
	unsigned int NrAdjoints[4];

	SpectrumSum();
	bool IsEqualTo(SpectrumSum*);
	bool Empty();
	bool IsSelfCPT();
	bool IsCPTOf(SpectrumSum*);
	void Add(SpectrumSum*);
	void Clear();
	void Print();
	bool PrintToFile(std::ofstream &);

	
};

#endif
