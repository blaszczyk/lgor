#ifndef __LGOR_MODELDATA__
#define __LGOR_MODELDATA__

#include <vector>
#include <string>
#include "Field.h"
#include "SuperPolynomial.h"



class ModelData {
public:
      
	int NrFields;
	int NrDiscretes;
	int NrSuperpotTerms;

	std::string Name;	
	char filename[256];	
	char resfilename[256];

	int KOrder;
	std::vector<int> Order;
	
	int KOrderF;
	std::vector<int> OrderF;

	int OrderHCF;
	std::vector<Field> Fld;

	SuperPolynomial SuperPotential;

	ModelData();
	void Clear();
	void Print();
	bool Initialize(char[256]);
	bool InitFromFile();
	void InitFromHeader();
	void InitHCF();
	bool CheckConsistency();
};




#endif
