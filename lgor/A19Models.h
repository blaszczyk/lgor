#ifndef __LGOR_A19MODELS__
#define __LGOR_A19MODELS__

#include <vector>

struct S9Permutation
{
	unsigned int To[9];
};

struct S9SubGrp
{
	std::vector< struct S9Permutation > Element;
};

struct Z3GrpElement
{
	unsigned int Pow[9];
};

struct Z3SubGrp
{
	std::vector<struct Z3GrpElement> Element;
};


class A19Models
{
	private:
/*
	bool S9ElementEqual(struct S9Permutation* , struct S9Permutation* );
	struct S9SubGrp MultiplyS9SubGrps(struct S9SubGrp* , struct S9SubGrp* );
	struct S9SubGrp CommonS9SubGrp(struct S9SubGrp* , struct S9SubGrp* );
//*/
	struct S9SubGrp S9;	

	void GenerateFullS9( struct S9SubGrp * );
	void GenerateSn( unsigned int , S9Permutation * , S9Permutation * , struct S9SubGrp * );

	void GenerateZ3SubGroup( struct Z3SubGrp* );
	bool Z3GrpElementEqual( struct Z3GrpElement* , struct Z3GrpElement* );
	bool Z3GroupsEqual( struct Z3SubGrp* , struct Z3SubGrp* );
	void SortGroupElements( struct Z3SubGrp* , std::vector< Z3SubGrp >* );
	bool Z3SubGrpConjugate( struct Z3SubGrp * , struct Z3SubGrp * , struct S9Permutation * ); 

	public:
	
	
	
	A19Models();
	void CreateA19Models( unsigned int );
};

#endif

