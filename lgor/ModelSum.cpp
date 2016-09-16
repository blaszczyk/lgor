#include "ModelSum.h"
#include "Parseline.h"
#include <iostream>


using namespace std;



ModelSum::ModelSum() 
{
	Clear();
}

bool ModelSum::IsEqualTo(ModelSum* comparator)
{
	if( !MdlSum.IsEqualTo( &( comparator->MdlSum ) ) )
		return false;
	bool Found;
	vector< bool > BnchFound( comparator->BnchSum.size(), false );
	unsigned int i,j;
	for( i = 0 ; i < BnchSum.size() ; i++ )
	{
		Found = false;
		for( j = 0 ; j < comparator->BnchSum.size(); j++ )
		{
			if( !BnchFound[j] && BnchSum[i].IsEqualTo( &( comparator->BnchSum[j] ) )  )
			{
				if( BnchMulti[i] != comparator->BnchMulti[j] )
					return false;
				Found = true;
				BnchFound[j] = true;
				break;
			}
		}
		if( ! Found )
			return false;
	}
	for( i = 0 ; i < BnchFound.size() ; i++ )
		if( !BnchFound[i] )
			return false;
	return true;
}

bool ModelSum::CPTInvariant()
{
	if( !MdlSum.IsSelfCPT() )
		return false;
	return true;
	bool Found;
	vector< bool > BnchFound( BnchSum.size(), false );
	unsigned int i,j;
	for( i = 0 ; i < BnchSum.size() ; i++ )
	{
		if( BnchFound[i] )
		{
			Found = false;
			for( j = i ; j < BnchSum.size() ; j++ )
			{
				if( !BnchFound[j] && BnchSum[i].IsCPTOf( &BnchSum[j] )  )
				{
					if( BnchMulti[i] != BnchMulti[j] )
						return false;
					Found = true;
					BnchFound[i] = true;
					BnchFound[j] = true;
					break;
				}
			}
		if( ! Found )
			return false;
		}
	}
	return true;
}


void ModelSum::Create(vector< Bunch >* Bnch)
{
	Clear();
	unsigned int i,j;
	bool Found;
	for( i = 0 ; i < Bnch->size() ; i++ )
	{
		MdlSum.Add( &( Bnch->at(i).Sum ) );
		Found = false;
		for( j = 0 ; j < BnchSum.size() ; j++ )
			if( BnchSum[j].IsEqualTo( &( Bnch->at(i).Sum ) ) )
			{
				Found = true;
				BnchMulti[j]++;
				break;
			}
		if( !Found )
		{
			BnchSum.push_back(Bnch->at(i).Sum);
			BnchMulti.push_back(1);
		}
	}
}

void ModelSum::Clear()
{
	MdlSum.Clear();
	BnchSum.clear();
	BnchMulti.clear();
}

bool ModelSum::IsCorrect()
{
	SpectrumSum BnchTotal;
	for( unsigned int i = 0 ; i < BnchSum.size() ; i++ )
		for( unsigned int j = 0 ; j < BnchMulti[i] ; j++ )
			BnchTotal.Add( & (BnchSum[i]) );
	if( BnchTotal.IsEqualTo(&MdlSum) )
		return true;
	return false;
}

void ModelSum::Print()
{
	cout << "Model Spectrum Summary:" << endl;
	MdlSum.Print();
	cout << "Bunch Summaries:" << endl;
	for( unsigned int i = 0 ; i < BnchSum.size() ; i++ )
	{
		cout << "Bunch Multiplicity: " << BnchMulti[i] << "\tSepectrum:" << endl;
		BnchSum[i].Print();
	}
}

bool ModelSum::PrintToFile(ofstream &ofile)
{
	if(!ofile.is_open())
		return false;

	ofile << "Spectrum Summary:" << endl;
	MdlSum.PrintToFile( ofile );
	ofile << "Bunch Summaries:" << endl;
	for( unsigned int i = 0 ; i < BnchSum.size() ; i++ )
	{
		ofile << "Bunch Multiplicity: " << BnchMulti[i] << "\tSepectrum:" << endl;
		BnchSum[i].PrintToFile( ofile );
	}
	return true;
}

bool ModelSum::ReadFromFile( char* filename)
{
	ifstream ifile(filename);
	if( ! ifile.is_open() )
		return false;
	parseline pline;
	string line,temp;
	int BnchCounter = -1;
	int CSpaceCounter;
	Clear();
	while(!ifile.eof())
	{
		getline(ifile,line);
		pline.writeLine(line);
		pline.gotoLetterDigit();
		temp = pline.getString("2 ");
		if( temp[temp.length() - 1] == '/' )
		{
			pline.jumpTo(0);
			CSpaceCounter = (pline.getSignedInt() + 3 ) / 2;
//			CSpaceCounter = ( (int) temp[temp.length() - 2 ] - '1') / 2 + 2;
//			if( temp[0] == '-' )
//				CSpaceCounter = 3 - CSpaceCounter;
			if( CSpaceCounter < 0 || CSpaceCounter > 3)
			{
				cout << "Wrong CSpaceCounter!" << endl;
				cin >> CSpaceCounter;
			}
			pline.Skip(2);
			if(BnchCounter == -1)
			{
				if(!pline.gotoInt())
					cout << ".";
				MdlSum.NrModuli[CSpaceCounter] = pline.getInt();
				if(!pline.gotoInt())
					cout << ".";
				MdlSum.Nr27Plets[CSpaceCounter] = pline.getInt();
				if(!pline.gotoInt())
					cout << ".";
				MdlSum.Nr27BarPlets[CSpaceCounter] = pline.getInt();
				if(!pline.gotoInt())
					cout << ".";
				MdlSum.NrAdjoints[CSpaceCounter] = pline.getInt();
			}
			else
			{
				if(!pline.gotoInt())
					cout << ".";
				BnchSum[BnchCounter].NrModuli[CSpaceCounter] = pline.getInt();
				if(!pline.gotoInt())
					cout << ".";
				BnchSum[BnchCounter].Nr27Plets[CSpaceCounter] = pline.getInt();
				if(!pline.gotoInt())
					cout << ".";
				BnchSum[BnchCounter].Nr27BarPlets[CSpaceCounter] = pline.getInt();
				if(!pline.gotoInt())
					cout << ".";
				BnchSum[BnchCounter].NrAdjoints[CSpaceCounter] = pline.getInt();
			}	
		}
		if( temp == "New" )
		{
			BnchCounter++;
			BnchSum.push_back(SpectrumSum());
			BnchMulti.push_back(0);
		}
		if( temp == "In" )
		{
			if(!pline.gotoInt())
				continue;
			BnchMulti[BnchCounter] = pline.getInt();
		}
		if( temp == "Bunch" )
		{
			BnchCounter++;
			BnchSum.push_back(SpectrumSum());
			BnchMulti.push_back(0);
			if(!pline.gotoInt())
				continue;
			BnchMulti[BnchCounter] = pline.getInt();
		}
	}
	
	ifile.close();
	if(CPTInvariant())
		return true;
	cout << "Spectrum in " << filename << " is not CPT invariant!" << endl; 
	Print();
	cin >> temp;
	Clear();
	return false;
}
