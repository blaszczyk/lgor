#include <iostream>
#include "Bunch.h"
#include <pthread.h>

using namespace std;

Bunch::Bunch ( ModelData* mData ) : mdata ( mData ) , Spectrum ( 4 ), empty(true)
{
}


Bunch::Bunch ( vector<int> twist, ModelData* mData ) : mdata ( mData ) , Twist ( twist ), Spectrum ( 4 ), empty(true)
{

}


void* initSector(void* t)
{
	Sector* s=static_cast<Sector*>(t);
	s->Initialize();
	return NULL;
}

void Bunch::CreateSectors()
{
	Sctr.clear();
	int i;
/*	for ( i = 0 ; i < 2 * mdata->KOrder; i++ )
	{
		Sctr.push_back ( Sector ( i,Twist,mdata ) );
		Sctr[i].Initialize();
	}// Take this when not using pthread*/
	for ( i = 0 ; i < 2 * mdata->KOrder; i++ )
		Sctr.push_back ( Sector ( i,Twist,mdata ) );
	vector<pthread_t> thread(2 * mdata->KOrder);
	for ( i = 0 ; i < 2 * mdata->KOrder; i++ )
		pthread_create( &(thread[i]), NULL, &initSector, &(Sctr[i]));
	for ( i = 0 ; i < 2 * mdata->KOrder; i++ )
		pthread_join( thread[i], NULL);
}

void Bunch::SummarizeSpectrum()
{
	vector<int> Pusher ( 9 );
	unsigned int i,j,k;
	for ( j = 0 ; j < 4 ; j++ )
	{
		Spectrum[j].Clear();
		Spectrum[j].SetNrRows ( 9 );
		for ( i = 0 ; i < Sctr.size(); i++ )
		{
			if ( Sctr[i].Ramond )
			{
				Pusher.clear();
				Pusher.resize ( 9,0 );
				for ( k = 0 ; k < 4 ; k++ )
					Pusher[2*k + 1] = Sctr[i].Cmplx[k].cohomology[j].term.size();
				Spectrum[j].push_col ( Pusher );
			}
			else
			{
				Pusher.clear();
				Pusher.resize ( 9,0 );
				for ( k = 0 ; k < 5 ; k++ )
					Pusher[2*k] = Sctr[i].Cmplx[k].cohomology[j].term.size();
				Spectrum[j].push_col ( Pusher );
			}
		}
	}
}


void Bunch::CheckE6Consistency()
{
	unsigned int i;
	int j;
	Sum.Clear();
	for ( i = 0 ; i < 4 ; i ++ )
	{
		for ( j = 0 ; j < 2*mdata->KOrder ; j++ )
		{
			if ( Spectrum[i].entry ( 0,j ) == Spectrum[i].entry ( 3,PlusMod ( j,1,2*mdata->KOrder ) ) && Spectrum[i].entry ( 0,j ) == Spectrum[i].entry ( 6,PlusMod ( j,2,2*mdata->KOrder ) ) )
				Sum.Nr27Plets[i] +=  Spectrum[i].entry ( 0,j );
			else
				cout << "27-plet Inconsistency in the Spectrum" << endl;
			if ( Spectrum[i].entry ( 1,j ) <= Spectrum[i].entry ( 4,PlusMod ( j,1,2*mdata->KOrder ) ) && Spectrum[i].entry ( 1,j ) == Spectrum[i].entry ( 7,PlusMod ( j,2,2*mdata->KOrder ) ) )
				Sum.NrAdjoints[i] +=  Spectrum[i].entry ( 1,j );
			else
				cout << "78-plet Inconsistency in the Spectrum" << endl;
			if ( Spectrum[i].entry ( 2,j ) == Spectrum[i].entry ( 5,PlusMod ( j,1,2*mdata->KOrder ) ) && Spectrum[i].entry ( 2,j ) == Spectrum[i].entry ( 8,PlusMod ( j,2,2*mdata->KOrder ) ) )
				Sum.Nr27BarPlets[i] +=  Spectrum[i].entry ( 2,j );
			else
				cout << "27Bar-plet Inconsistency in the Spectrum" << endl;
			Sum.NrModuli[i] += Spectrum[i].entry ( 4,j );
		}
		Sum.NrModuli[i] -= Sum.NrAdjoints[i];
	}
	if(Sum.Empty())
		empty = true;
	else
		empty=false;
	return;
}

void Bunch::PrintSpectrum()
{
	int i;
	for ( i = 0 ; i < 4 ; i ++ )
	{
		cout << "Spectrum with q_+ = " << 2 * i - 3 << " / 2 \n";
		Spectrum[i].PrintNonZero();
	}
}


void Bunch::ReadTwists()
{
	cout << "Enter the " << mdata->NrDiscretes << " discrete twists: \n";
	Twist.resize ( mdata->NrDiscretes );
	for ( int i = 0; i<mdata->NrDiscretes; i++ )
		cin >> Twist[i];
}

void Bunch::Initialize()
{
	CreateSectors();
	SummarizeSpectrum();
	CheckE6Consistency();
}

void Bunch::run ( bool initialize )
{
	if ( initialize )
	{
		ReadTwists();
		Initialize();
	}
	string command;
	int temp;
	bool engage = true;
	while ( engage )
	{
		cout << "lgor:/" << mdata->Name << "/" << "x";
		for ( unsigned int i = 0 ; i < Twist.size(); i++ )
			cout << ',' << Twist[i];
		cout <<  ">";
		cin >> command;
		switch ( command[0] )
		{
			case 'x':
				engage = false;
				break;
			case 'h':
				cout << "Enter s to enter Sector \n";
				cout << "Enter e to print Summary of Spectrum \n";
				cout << "Enter p to print Spectrum \n";
				cout << "Enter x to go back \n";
				break;
			case 's':
				switch ( command.length() )
				{
					case 1:
						cout << "Which one?";
						cin >> temp;
						break;
					case 2:
						temp = ( int ) command[1]-'0';
						break;
					case 3:
						temp = ( ( int ) command[1]-'0' ) * 10 + ( int ) command[2]-'0' ;
						break;
					default:
						temp = ( ( int ) command[1]-'0' ) * 100 + ( ( int ) command[2]-'0' ) * 10 + ( int ) command[3]-'0' ;
						break;
				}
				if ( temp > -1 && temp < ( int ) Sctr.size() )
					Sctr[temp].run ( false );
				break;
			case 'p':
				PrintSpectrum();
				break;
			case 'e':
				Sum.Print();
				break;
		}
	}
}
