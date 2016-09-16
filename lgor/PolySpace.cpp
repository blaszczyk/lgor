#include "PolySpace.h"

void PolySpace::ReduceBasis()
{
	unsigned int i,j;
	for( i = 0 ; i < term.size(); i++ )
	{
		for( j = 0 ; j < i ; j++ )
			if(term[i].EqualTo(&term[j]))
			{
				term.erase(term.begin()+i);
				i--;
			}
	}
}
