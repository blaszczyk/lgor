#include "Polynomial.h"
#include <iostream>


using namespace std;

void Polynomial::Print()
{
	Print ( false );
}


void Polynomial::Print ( bool withCR )
{
	unsigned int i,NrMonomials = term.size();
	if ( NrMonomials == 0 )
	{
		cout << "0\n";
		return;
	}
	if ( Koeff.size() < term.size() )
		Koeff.resize ( term.size(),1 );
	for ( i = 0; i < NrMonomials-1; i ++ )
	{
		if ( Koeff[i] != 0 )
		{
			if ( Koeff[i] != 1 )
				cout << Koeff[i] << " * ";
			term[i].Print();
			if ( withCR )
				cout << " +\n";
			else
				cout << "  +  ";
		}
	}
	if ( Koeff[NrMonomials-1] != 0 )
	{
		if ( Koeff[NrMonomials-1] != 1 )
			cout << Koeff[i] << " * ";
		term[NrMonomials-1].Print();
	}
	if ( withCR )
		cout << '\n';
}

void Polynomial::Simplify()
{
	unsigned int i,j;
	for ( i = 0 ; i < term.size(); i++ )
	{
		if ( Koeff[i] == 0 )
		{
			term.erase ( term.begin() +i );
			Koeff.erase ( Koeff.begin() +i );
			i--;
		}
		else
			for ( j = 0 ; j < i ; j++ )
				if ( term[i].EqualTo ( &term[j] ) )
				{
					Koeff[j] += Koeff[i];
					term.erase ( term.begin() +i );
					Koeff.erase ( Koeff.begin() +i );
					i--;
				}
	}
}

bool Polynomial::IsZero()
{
	if ( term.size() == 0 )
		return true;
	for ( unsigned int i = 0 ; i < Koeff.size(); i++ )
		if ( Koeff[i] != 0 )
			return false;
	return true;
}

void Polynomial::Subtract ( Polynomial* subtrahend )
{
	unsigned int i;
	for ( i = 0 ; i < subtrahend->term.size() ; i++ )
	{
		term.push_back ( subtrahend->term[i] );
		Koeff.push_back ( -subtrahend->Koeff[i] );
	}
	Simplify();
}

void Polynomial::VectorForm ( vector<Monomial>* basis, vector<int>* result )
{
	int i,j;
	bool identified;
	result->resize ( 0 );
	result->resize ( basis->size(),0 );
	for ( i = 0 ; i < ( int ) term.size() ; i ++ )
	{
		identified = false;
		for ( j = 0 ; j < ( int ) basis->size() ; j ++ )
		{
//      if(!term[i].EqualTo(basis->at(j)))
			if ( basis->at ( j ).EqualTo ( &term[i] ) )
			{
				if ( ( int ) this->Koeff.size() <= i )
					result->at ( j ) += 1;
				else
					result->at ( j ) += Koeff[i];
				identified = true;
			}
		}
		if ( !identified )
		{
			cout << " Basis Vector Missing : ";
			term[i].Print();
			cout << endl;
		}
	}
}



void Polynomial::FromVector ( vector<Monomial>* basis, vector<int>* InputVector )
{
	Clear();
	unsigned int i;
	if ( InputVector->size() > basis->size() )
		return;
	for ( i = 0 ; i < InputVector->size(); i++ )
		if ( InputVector->at ( i ) != 0 )
		{
			term.push_back ( basis->at ( i ) );
			Koeff.push_back ( InputVector->at ( i ) );
		}
}

void Polynomial::FromVector ( vector<Polynomial>* basis, vector<int>* InputVector )
{
	Clear();
	unsigned int i;
	if ( InputVector->size() > basis->size() )
		return;
	for ( i = 0 ; i < InputVector->size(); i++ )
		if ( InputVector->at ( i ) != 0 )
		{
			Add ( &basis->at ( i ) );
		}
	Simplify();
}

void Polynomial::FromMonomial ( Monomial* monom )
{
	term.clear();
	Koeff.clear();
//	if(monom->Zero())
//		return;
	Koeff.push_back ( 1 );
	term.push_back ( *monom );
}

void Polynomial::Add ( Polynomial* Summand )
{
	for ( unsigned int i = 0 ; i < Summand->term.size(); i++ )
	{
		term.push_back ( Summand->term[i] );
		Koeff.push_back ( Summand->Koeff[i] );
	}
}
void Polynomial::Clear()
{
	term.clear();
	Koeff.clear();
}
