#include "SpectrumSum.h"
#include <iostream>

using namespace std;







SpectrumSum::SpectrumSum() 
{
	Clear();
}

bool SpectrumSum::IsEqualTo(SpectrumSum* comparator)
{
	for( unsigned int i = 0 ; i < 4 ; i++ )
	{
		if(NrModuli[i] != comparator->NrModuli[i])
			return false;
		if(Nr27Plets[i] != comparator->Nr27Plets[i])
			return false;
		if(Nr27BarPlets[i] != comparator->Nr27BarPlets[i])
			return false;
		if(NrAdjoints[i] != comparator->NrAdjoints[i])
			return false;
	}
	return true;
}

bool SpectrumSum::IsCPTOf(SpectrumSum* comparator)
{
	for( unsigned int i = 0 ; i < 4 ; i++ )
	{
		if(NrModuli[i] != comparator->NrModuli[3-i])
			return false;
		if(Nr27Plets[i] != comparator->Nr27BarPlets[3-i])
			return false;
		if(Nr27BarPlets[i] != comparator->Nr27Plets[3-i])
			return false;
		if(NrAdjoints[i] != comparator->NrAdjoints[3-i])
			return false;
	}
	return true;
}

bool SpectrumSum::IsSelfCPT()
{
	for( unsigned int i = 0 ; i < 2 ; i++ )
	{
		if(NrModuli[i] != NrModuli[3-i])
			return false;
		if(Nr27Plets[i] != Nr27BarPlets[3-i])
			return false;
		if(Nr27BarPlets[i] != Nr27Plets[3-i])
			return false;
		if(NrAdjoints[i] != NrAdjoints[3-i])
			return false;
	}
	return true;
}

void SpectrumSum::Add(SpectrumSum* summand)
{
	for( unsigned int i = 0 ; i < 4 ; i++ )
	{
		NrModuli[i] += summand->NrModuli[i];
		Nr27Plets[i] += summand->Nr27Plets[i];
		Nr27BarPlets[i] += summand->Nr27BarPlets[i];
		NrAdjoints[i] += summand->NrAdjoints[i];
	}
	return;
}


void SpectrumSum::Clear()
{
	for( unsigned int i = 0 ; i < 4 ; i++ )
	{
		NrModuli[i] = 0;
		Nr27Plets[i] = 0;
		Nr27BarPlets[i] = 0;
		NrAdjoints[i] = 0;
	}
	return;
}

void SpectrumSum::Print()
{
	cout << "\t1\t\t27\t\t27Bar\t\t78\n";
	for(unsigned int i = 0 ; i < 4 ; i++)
	{
		cout << 2 * (int)i - 3 << "/2\t";
		cout << NrModuli[i] << "\t\t";
		cout << Nr27Plets[i] << "\t\t";
		cout << Nr27BarPlets[i] << "\t\t";
		cout << NrAdjoints[i] << "\n";
	}
	return;
}

bool SpectrumSum::PrintToFile(ofstream &ofile)
{
	if(!ofile.is_open())
		return false;
	ofile << "\t1\t\t27\t\t27Bar\t\t78\n";
	for(unsigned int i = 0 ; i < 4 ; i++)
	{
		ofile << 2 * (int)i - 3 << "/2\t";
		ofile << NrModuli[i] << "\t\t";
		ofile << Nr27Plets[i] << "\t\t";
		ofile << Nr27BarPlets[i] << "\t\t";
		ofile << NrAdjoints[i] << "\n";
	}
	return true;
}

bool SpectrumSum::Empty()
{
	for( unsigned int i = 0 ; i < 4 ; i++ )
	{
		if(NrModuli[i] != 0)
			return false;
		if(Nr27Plets[i] != 0)
			return false;
		if(Nr27BarPlets[i] != 0)
			return false;
		if(NrAdjoints[i] != 0)
			return false;
	}
	return true;
}
