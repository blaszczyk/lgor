#include "A19Models.h"
#include "SimpleMath.h"
#include <string.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

using namespace std;

A19Models::A19Models() 
{
	GenerateFullS9( &S9 );
}

/*
bool A19Models::S9ElementEqual(struct S9Permutation* P1, struct S9Permutation* P2)
{
	unsigned int i;
	for( i = 0 ; i < 9 ; i++ )
		if( P1->To[i] != P2->To[i] )
			return false;
	return true;
}

struct S9SubGrp A19Models::MultiplyS9SubGrps( struct S9SubGrp * G1, struct S9SubGrp * G2)
{
	struct S9SubGrp result;
	struct S9Permutation temp;
	unsigned int i,j,k;
	bool NewElement;
	for( i = 0 ; i < G1->Element.size() ; i++ )
		for( j = 0 ; j < G2->Element.size() ; j++ )
		{
			NewElement = true;
			for( k = 0 ; k < 9 ; k++ )
				temp.To[k] = G1->Element[i].To[ G2->Element[j].To[k] ];
			for( k = 0 ; k < result.Element.size() ; k++ )
				if( S9ElementsEqual( &temp, &(result.Element[k]) ) )
				{
					NewElement = false;
					break;
				}
			if( NewElement )
				result.Element.push_back(temp);
		}
	return result;
}


struct S9SubGrp A19Models::CommonS9SubGrp( struct S9SubGrp *  G1, struct S9SubGrp *  G2)
{
	struct S9SubGrp result;
	unsigned int i,j;
	for( i = 0 ; i < G1->Element.size() ; i++ )
		for( j = 0 ; j < G2->Element.size() ; j++ )
			if( S9ElementEqual( &(G1->Element[i]) , &(G2->Element[j]) ) )
			{
				result.Element.push_back(G1->Element[i]);
				break;
			}
	return result;
}
//*/

void A19Models::GenerateSn( unsigned int n , S9Permutation * FirstBit , S9Permutation * SecondBit , struct S9SubGrp * S9)
{
	S9Permutation Pusher;
	unsigned int i;
	for( i = 0 ; i < 9-n ; i++ )
		Pusher.To[i] = FirstBit->To[i];
	if( n == 2 )
	{
		Pusher.To[7] = SecondBit->To[0];
		Pusher.To[8] = SecondBit->To[1];
		S9->Element.push_back( Pusher );
		Pusher.To[7] = SecondBit->To[1];
		Pusher.To[8] = SecondBit->To[0];
		S9->Element.push_back( Pusher );
		return;
	}
	S9Permutation NewSecondBit;
	for( i = 1 ; i < n ; i++ )
		NewSecondBit.To[i-1] = SecondBit->To[i];
	for( i = 0 ; i < n ; i++ )
	{
		Pusher.To[9-n] = SecondBit->To[i];
		GenerateSn( n-1 , &Pusher , &NewSecondBit , S9 );
		NewSecondBit.To[i] = SecondBit->To[i];
	}	
}

void A19Models::GenerateFullS9( struct S9SubGrp * S9 )
{
	unsigned int i;
	S9->Element.clear();
	S9Permutation FirstBit;
	S9Permutation SecondBit;
	for( i = 0 ; i < 9 ; i++ )
		SecondBit.To[i]=i;
	GenerateSn( 9 , &FirstBit , &SecondBit , S9 );
}


bool A19Models::Z3GrpElementEqual( struct Z3GrpElement* E1, struct Z3GrpElement* E2)
{
	for( unsigned int i = 0 ; i < 9 ; i++ )
		if( E1->Pow[i] != E2->Pow[i] )
			return false;
	return true;
}

void A19Models::GenerateZ3SubGroup( struct Z3SubGrp *  Group)
{	
	bool engage = true;
	struct Z3GrpElement temp;
	bool found;
	unsigned int i,j;
	for( i = 0 ; i < 9 ; i++ )
		temp.Pow[i]=1;
	Group->Element.push_back(temp);
	unsigned int NrGenerators = Group->Element.size();
	vector< unsigned int > Counter(NrGenerators,0);
	while(engage)
	{
		for( i = 0 ; i < 9 ; i++ )
		{
			temp.Pow[i]=0;
			for( j = 0 ; j < NrGenerators ; j++ )
				temp.Pow[i] += Counter[j] * Group->Element[j].Pow[i];
			temp.Pow[i] = TrueMod( temp.Pow[i] , 3 );
		}
		found = false;
		for( i = 0 ; i < Group->Element.size() ; i++ )
			if( Z3GrpElementEqual( &temp, &( Group->Element[i] ) ) )
				found = true;
		if( !found )
			Group->Element.push_back(temp);
		Counter[0]++;
		for( i = 1 ; i < NrGenerators ; i++ )
			if( Counter[i-1] == 3 )
			{
				Counter[i-1] = 0;
				Counter[i]++;
			}
		if( Counter[NrGenerators-1] == 3 )
			engage = false;
	}
}

void A19Models::SortGroupElements( struct Z3SubGrp * G, vector< struct Z3SubGrp > * SubClasses)
{
	unsigned int Identi,i,j;
	SubClasses->clear();
	SubClasses->resize(6);
	for( i = 0 ; i < G->Element.size() ; i++)
	{
		Identi = 0;
		for( j = 0 ; j < 9 ; j++ )
		{
			if( G->Element[i].Pow[j] == 0 )
				Identi ++;
			if( G->Element[i].Pow[j] == 1 )
				Identi += 10;
		}
		switch(Identi)
		{
		case 0:
			SubClasses->at(0).Element.push_back( G->Element[i] );
			break;
		case 11:
			SubClasses->at(1).Element.push_back( G->Element[i] );
			break;
		case 22:
			SubClasses->at(2).Element.push_back( G->Element[i] );
			break;
		case 33:
			SubClasses->at(3).Element.push_back( G->Element[i] );
			break;
		case 30:
			SubClasses->at(4).Element.push_back( G->Element[i] );
			break;
		case 41:
			SubClasses->at(5).Element.push_back( G->Element[i] );
			break;
		case 44: case 52: case 63: case 60: case 71: case 90: case 03: case 14: case 25: case 36: case 6: case 17: case 9:
			break;
		default:
			cout << "Got SUSY breaking Discrete Group element: " << Identi << endl;
			break;
		}
	}
}

bool A19Models::Z3GroupsEqual( struct Z3SubGrp * G1, struct Z3SubGrp * G2)
{
	if( G1->Element.size() != G2->Element.size() )
		return false;
	unsigned int i,j;
	vector< struct Z3SubGrp > SubClasses1(6);
	vector< struct Z3SubGrp > SubClasses2(6);
	SortGroupElements( G1, &SubClasses1);
	SortGroupElements( G2, &SubClasses2);
	for( i = 0 ; i < 6 ; i++ )
		if( SubClasses1[i].Element.size() != SubClasses2[i].Element.size() )
			return false;
	for( i = 0 ; i < S9.Element.size() ; i++)
	{
		j=1;
		while( j < 6 &&  Z3SubGrpConjugate( &( SubClasses1[j] ) , &( SubClasses2[j] ) , &( S9.Element[i] ) ) )
			j++;
		if( j == 6)
			return true;
	}
	return false;
}

bool A19Models::Z3SubGrpConjugate( struct Z3SubGrp * G1, struct Z3SubGrp * G2, struct S9Permutation * sigma ) 
{
	if( G1->Element.size() != G2->Element.size() )
		return false;
	unsigned int i , j;
	Z3GrpElement temp;
	bool found;
	for( i = 0 ; i < G1->Element.size() ; i++ )
	{
		found = false;
		for( j = 0 ; j < 9 ; j++ )
			temp.Pow[j] = G1->Element[i].Pow[ sigma->To[j] ];
		for(j = 0 ; j < G2->Element.size() ; j++ )
			if( Z3GrpElementEqual( &temp , &( G2->Element[j] ) ) )
				found = true;
		if( !found )
			return false;
	}
	return true;
}


void A19Models::CreateA19Models(unsigned int NrDiscretes)
{
	char pathname1[] = "./A19";
	char pathname2[] = "xDiscreteSymmetries";
	char filename[] = "A19Z";
	char pathfilename[256];
	char counter[8];
	vector< vector<int> > ChargeMatrix(8);
	struct Z3SubGrp tempModel;
	struct Z3GrpElement tempElement;
	vector< struct Z3SubGrp > Models(0);
	vector< unsigned int > ChargePointer;
	int temp,temp1;
	unsigned int ChargePointerLimit;
	unsigned int i,j;
	bool engage,smaller;
	ofstream ofile;
	if( chdir( pathname1 ) )
	{
		mkdir( pathname1 , S_IRWXU );
		chdir( pathname1 );
	}
	pathname2[0] = (char) NrDiscretes + '0';
	if( chdir( pathname2 ) )
	{
		mkdir( pathname2 , S_IRWXU );
	}
	chdir( ".." );
	strcpy(counter,"0000000");
	for( i = 0 ; i < 8 ; i++ )
	{
		ChargeMatrix[i].clear();
		ChargeMatrix[i].resize(NrDiscretes,0);
	}
	for( i = 0 ; i < NrDiscretes ; i++ )
		ChargeMatrix[i][i] = 1;
	ChargePointer.resize(7 - NrDiscretes,0);
	ChargePointerLimit = Power(3,NrDiscretes)-1;
	engage = true;
	while(engage)
	{
		for( i = NrDiscretes ; i < 7 ; i ++)
		{
			temp = ChargePointer[i - NrDiscretes];
			for( j = 0 ; j < NrDiscretes ; j++ )
			{
				temp1 = TrueMod(temp,3);
				temp = (temp - temp1) / 3;
				ChargeMatrix[i][j] = temp1;
			}
		}
		smaller = true;
		for( i = 1 ; i < NrDiscretes ; i++ )
			if( ChargeMatrix[NrDiscretes][i] < ChargeMatrix[NrDiscretes][i-1] )
				smaller = false;
		if(ChargeMatrix[6][i] < TrueMod( ChargeMatrix[7][i] , 3 ))
			smaller = false;
		for( i = 0 ; i < NrDiscretes ; i++ )
		{
			ChargeMatrix[7][i]=0;
			for( j = 0 ; j < 7 ; j++ )
				ChargeMatrix[7][i] -= ChargeMatrix[j][i];
		}
		i = NrDiscretes -1;
		while( i > 0 &&  ChargeMatrix[6][i] == TrueMod( ChargeMatrix[7][i] , 3 ) )
		{
			i--;
		}
		if( ChargeMatrix[6][i] < TrueMod( ChargeMatrix[7][i] , 3 ) )
			smaller = false;
		if( smaller )
		{
			tempElement.Pow[8] = 0;
			tempModel.Element.clear();
			for( i = 0 ; i < NrDiscretes; i++ )
			{
				for( j = 0 ; j < 7 ; j++ )
					tempElement.Pow[j] = ChargeMatrix[j][i];
				tempElement.Pow[7] = (unsigned int) TrueMod( ChargeMatrix[7][i] , 3 );
				tempModel.Element.push_back( tempElement );
			}
			GenerateZ3SubGroup( &tempModel );
			for( i = 0 ; i < Models.size() ; i++ )
				if( smaller )
					if( Z3GroupsEqual( &tempModel , &(Models[i]) ) )
						smaller = false;
		}
		if( smaller )
		{
			Models.push_back(tempModel);
			strcpy(pathfilename,pathname1);
			strcat(pathfilename,"/");
			strcat(pathfilename,pathname2);
			strcat(pathfilename,"/");
			strcat(pathfilename,filename);
			for( i = 0 ; i < NrDiscretes ; i++ )
				strcat(pathfilename,"3");
			strcat(pathfilename,"-");
			strcat(pathfilename,counter);
			strcat(pathfilename,".lgo");
			ofile.open(pathfilename);
			if(!ofile.is_open())
			{
				cout << "Error opening " << pathfilename << endl;
				cin >> temp;
			}
			ofile << "Model File automatically generated by LGOR, Team Beyond Standard" << endl;
			ofile << "Name = \"A1^9/Z";
			for( i = 0 ; i < NrDiscretes ; i++ )
				ofile << "3";
			ofile << "-" << counter << "\"" << endl;
			ofile << "KOrder = 3"<< endl << "Order =";
			for( i = 0 ; i < NrDiscretes ; i++ )
				ofile << " 3";
			ofile << endl;
			for( i = 0 ; i < 8 ; i ++ )
			{
				ofile << "Field \"" << i+1 << "\" 1";
				for( j = 0 ; j < NrDiscretes; j++ )
					ofile << " " << ChargeMatrix[i][j];
				ofile << endl;
			}
			ofile << "Field \"9\" 1";
			for( i = 0 ; i < NrDiscretes; i++)
				ofile << " 0" ;
			ofile << endl;
			for( i = 0 ; i < 9 ; i ++ )
			{
				ofile << "SPotTerm 1";
				for( j = 0 ; j < 9 ; j++ )
				{
					if( i == j)
						ofile << " 3";
					else
						ofile << " 0";
				}
				ofile << endl;
			}
			ofile.close();
			cout << "Created Model " << pathfilename << endl;
			counter[6]++;
			for( i = 6 ; i > 0 ; i-- )
				if( counter[i] > '9')
				{
					counter[i] = '0';
					counter[i-1]++;
				}
		}
		i=6-NrDiscretes;
		while(i > 0 && ChargePointer[i] == ChargePointer[i-1])
			i--;
		ChargePointer[i]++;
		for( j = i+1 ; j < 7-NrDiscretes ; j ++ )
			ChargePointer[j] =0;
		if(ChargePointer[6-NrDiscretes] ==  ChargePointerLimit)
			engage = false;
	}
	return;
}


