#ifndef __LGOR_LGOR__
#define __LGOR_LGOR__


#include "Model.h"



struct lgostring
{
	char at[256];
};


class Lgor
{
	public:
	Model Mdl;

	bool scanFolder(char[256], std::vector<lgostring> *, std::vector<lgostring>*);
	void SortStrings(std::vector<lgostring> *);
	void scanModels( char* , std::vector<lgostring>* );
	unsigned int FindInequivalentShortSpectra( char* , std::vector<lgostring>* );
	void run();

	void CreateA19Models();
};

#endif
