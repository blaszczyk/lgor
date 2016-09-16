#include "Complex.h"
#include <iostream>

using namespace std;

Complex::Complex ( ModelData* mData ) : part ( 6 ), cohomology ( 4 ), shrunk ( 6 ), QBarMatrix ( 6 ), QBarRemainder ( 6 )
{
	mdata = mData;
};

int Complex::QBarAction ( Polynomial* QBar, Monomial* Input, Polynomial* Output )
{
	int i,factor;
	Monomial Tester ( mdata );
	Output->term.clear();
	Output->Koeff.clear();
	for ( i = 0 ; i < ( int ) QBar->term.size(); i++ )
	{
		factor = Tester.IsProductOf ( Input,&QBar->term[i] );
		if ( Tester.RegularAndNonZero() )
		{
			Output->term.push_back ( Tester );
			Output->Koeff.push_back ( factor * QBar->Koeff[i] );
		}
	}
	return Output->term.size();
}


int Complex::GGBarAction ( Polynomial* QBar, Polynomial* Input, Polynomial* Output )
{
	int factor;
	unsigned int i,j;
	Monomial Tester ( mdata );
	Output->term.clear();
	Output->Koeff.clear();
	if ( Input->term.size() == 0 )
	{
		Tester.ClearPowers();
		Input->FromMonomial ( &Tester );
	}
	for ( i = 0 ; i < QBar->term.size(); i++ )
	{
		for ( j = 0 ; j < Input->term.size(); j++ )
		{
			factor = Tester.IsProductOf ( &Input->term[j],&QBar->term[i] );
			if ( Tester.RegularAndNonZero() )
			{
				Output->term.push_back ( Tester );
				Output->Koeff.push_back ( factor * QBar->Koeff[i] * Input->Koeff[j] );
			}
		}
	}
	Output->Simplify();
	return Output->term.size();
}


void Complex::CreateQBarMatrices ( Polynomial QBar )
{
	int i,j;
	vector<int> RawLine;
	Polynomial Tester;
	for ( i = 0; i < 5; i ++ )
	{
		QBarMatrix[i].SetNrRows ( part[i+1].term.size() );
		RawLine.reserve ( part[i+1].term.size() );
		for ( j = 0 ; j < ( int ) part[i].term.size() ; j ++ )
		{
			if ( QBarAction ( &QBar,&part[i].term[j],&Tester ) > 0 )
			{
				Tester.VectorForm ( &part[i+1].term,&RawLine );
				QBarMatrix[i].push_col ( RawLine );
			}
			else
			{
				RawLine.resize ( 0 );
				RawLine.resize ( part[i+1].term.size() );
				QBarMatrix[i].push_col ( RawLine );
			}
		}
	}
}


void Complex::ShrinkProblem()
{
	vector< vector<bool> > toshrunk ( 6 );
	vector<int> pusher;
	Polynomial MonoToPoly;
	int i,j,k;
	int temp;
	for ( i = 0 ; i < 6 ; i++ )
		toshrunk[i].resize ( part[i].term.size(),true );
	for ( i = 1 ; i < 5; i ++ ) //Check if Monomials are Cohomologicaly Trivial
	{
		for ( j = 0 ; j < ( int ) toshrunk[i].size(); j ++ )
		{
			if ( QBarMatrix[i-1].RowZero ( j ) && QBarMatrix[i].ColZero ( j ) )
			{
				toshrunk[i][j] = false;
				MonoToPoly.FromMonomial ( &part[i].term[j] );
				cohomology[i-1].term.push_back ( MonoToPoly );
			}
		}
	}
	for ( i = 0 ; i < 4 ; i ++ )  // Check if Monomial are mapped 1 on 1
	{
		for ( j = 0; j < ( int ) part[i].term.size(); j ++ )
		{
			temp = QBarMatrix[i].SingleElementCol ( j );
			if ( temp > -1 )
			{
				if ( QBarMatrix[i].SingleElementRow ( temp ) == j )
				{
					toshrunk[i][j] = false;
					toshrunk[i+1][temp] = false;
				}
			}
		}
	}  // Collect remaining Monomials
	for ( i = 0 ; i < 6 ; i++ )
		for ( j = 0 ; j < ( int ) toshrunk[i].size(); j ++ )
			if ( toshrunk[i][j] )
				shrunk[i].term.push_back ( part[i].term[j] );
	for ( i = 0 ; i < 5 ; i++ )  // Collect remaining QBarMatrices
	{
		pusher.reserve ( shrunk[i].term.size() );
		for ( j = 0 ; j < ( int ) part[i+1].term.size(); j++ )
			if ( toshrunk[i+1][j] )
			{
				pusher.resize ( 0 );
				for ( k = 0 ; k < ( int ) part[i].term.size(); k++ )
					if ( toshrunk[i][k] )
						pusher.push_back ( QBarMatrix[i].entry ( j,k ) );
				QBarRemainder[i].push_row ( pusher );
			}
	}
}

void Complex::ReplaceByShrunk()
{
	part = shrunk;
	QBarMatrix = QBarRemainder;
}

void Complex::FuseRemainderQBars()
{
	int i;
	for ( i = 5; i > 0 ; i-- )
		QBarRemainder[i].FuseWithTranspose ( &QBarRemainder[i-1] );
}

void Complex::KillMonomialPreimages()
{
	vector< vector<bool> > toshrunk ( 6 );
	vector<int> pusher;
	int i,j,k;
	for ( i = 0 ; i < 6 ; i++ )
	{
		toshrunk[i].resize ( part[i].term.size(),true );
		shrunk[i].term.clear();
		QBarRemainder[i].Clear();
	}
	for ( i = 0; i < 6; i++ )
		for ( j = 0; j < ( int ) QBarMatrix[i].NrRows(); j++ )
			if ( QBarMatrix[i].SingleElementRow ( j ) > -1 )
				toshrunk[i][QBarMatrix[i].SingleElementRow ( j ) ] = false;
	for ( i = 0 ; i < 6 ; i++ )
		for ( j = 0 ; j < ( int ) toshrunk[i].size(); j ++ )
			if ( toshrunk[i][j] )
				shrunk[i].term.push_back ( part[i].term[j] );
	for ( i = 0 ; i < 6 ; i++ )  // Collect remaining QBarMatrices
	{
		pusher.reserve ( shrunk[i].term.size() );
		for ( j = 0 ; j < ( int ) QBarMatrix[i].NrRows(); j++ )
//      if(toshrunk[i+1][j])
		{
			pusher.resize ( 0 );
			for ( k = 0 ; k < ( int ) part[i].term.size(); k++ )
				if ( toshrunk[i][k] )
					pusher.push_back ( QBarMatrix[i].entry ( j,k ) );
			QBarRemainder[i].push_row ( pusher );
		}
	}
}

/* *****************************************
with pthread
***************************************** */

// Causes too much trouble!

/*
 #include <pthread.h>

struct twoMatrixPtr
{
Matrix* m1;
Matrix* m2;
};

void* findKernel( void* p)
{
	struct twoMatrixPtr* ptr = static_cast<twoMatrixPtr*>(p);
	Matrix* QBar = ptr->m1;
	Matrix* Kernel = ptr->m2;
	QBar->DivideByRowGCD();
	QBar->FindKernel(Kernel);
	return NULL;
}

void Complex::FindFusedQBarKernels() 
{
	Matrix Kernel[6];
	int KernelDim;
	int i,j;
	vector<int> KernelRow;
	Polynomial pusher;
	pthread_t thread[6];
	struct twoMatrixPtr* ptr;
	for ( i = 0 ; i < 6 ; i++ )
	{
		ptr->m1 = &(QBarMatrix[i]);
		ptr->m2 = &(Kernel[i]);
		pthread_create(&(thread[i]),NULL,&findKernel,ptr);
	}
	for ( i = 0 ; i < 6 ; i++ )
	{
		pthread_join(thread[i],NULL);
		KernelDim = Kernel[i].NrRows();
		if ( KernelDim != 0 && ( i == 0 || i ==5 ) )
			cout << "\n\n\n WTF!!! Found physical states of QBar = +-5/2!!! \n\n"  << std::endl;
		else
			for ( j = 0 ; j < KernelDim; j++ )
			{
				Kernel[i].GiveRow ( j,&KernelRow );
				pusher.FromVector ( &part[i].term,&KernelRow );
				cohomology[i-1].term.push_back ( pusher );
			}
	}
} //*/


/* *****************************************
End: with pthread
***************************************** */



void Complex::FindFusedQBarKernels() // w/o pthread 
{
	Matrix Kernel;
	int KernelDim;
	int i,j;
	vector<int> KernelRow;
	Polynomial pusher;
	for ( i = 0 ; i < 6 ; i++ )
	{
		QBarMatrix[i].DivideByRowGCD();
		KernelDim=QBarMatrix[i].FindKernel ( &Kernel );
		if ( KernelDim != 0 && ( i == 0 || i ==5 ) )
			cout << "\n\n\n WTF!!! Found physical states of QBar = +-5/2!!! \n\n"  << std::endl;
		else
			for ( j = 0 ; j < KernelDim; j++ )
			{
				Kernel.GiveRow ( j,&KernelRow );
				pusher.FromVector ( &part[i].term,&KernelRow );
				cohomology[i-1].term.push_back ( pusher );
			}
	}
} //*/
