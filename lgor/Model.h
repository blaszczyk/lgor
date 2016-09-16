#ifndef __LGOR_MODEL__
#define __LGOR_MODEL__

#include <vector>
#include "ModelData.h"
#include "Bunch.h"
#include "ModelSum.h"

class Model
{
	public:
	bool initialized;
	ModelData mdata;
	std::vector<Bunch> Bnch;
	ModelSum Sum;

	Model();
	void Clear();
	bool Initialize(char[256]);
	bool SaveResults();
	bool SaveResultsShort();
	bool ExistShortResults();
	bool LoadResults();
	void ScanAll();
	void PrintE6Summary();
	void PrintBunches(bool);
	void run();
};

#endif
