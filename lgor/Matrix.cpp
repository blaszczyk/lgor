#include "Matrix.h"
#include <vector>
#include <iostream>
#include "SimpleMath.h"

using namespace std;

Matrix::Matrix()
{
}

Matrix::Matrix ( const Matrix& matrix )
{
	Entry = matrix.Entry;
}

void Matrix::push_row ( vector<int> line )
{
	Entry.push_back ( line );
}

void Matrix::push_col ( vector<int> line )
{
	unsigned int i;
	for ( i = 0; i < Entry.size(); i++ )
		Entry[i].push_back ( line[i] );
}

void Matrix::SetNrRows ( unsigned int NrRows )
{
	if ( NrRows < 0 )
		return;
	Entry.clear();
	Entry.resize ( NrRows );
}

void Matrix::Clear()
{
	Entry.clear();
}

int Matrix::entry ( int row, int col )
{
	if ( row < 0 || row > ( int ) Entry.size() )
		return 0;
	if ( col < 0 || col > ( int ) Entry.at ( row ).size() )
		return 0;
	return Entry.at ( row ).at ( col );
}

unsigned int Matrix::NrRows()
{
	return Entry.size();
}

unsigned int Matrix::NrCols()
{
	if ( Entry.size() == 0 )
		return 0;
	return Entry[0].size();
}

bool Matrix::ColZero ( int col )
{
	unsigned int i;
	if ( col >= ( int ) NrCols() )
		return true;
	for ( i = 0 ; i < Entry.size(); i ++ )
		if ( Entry[i][col] != 0 )
			return false;
	return true;
}

bool Matrix::RowZero ( int row )
{
	if ( row >= ( int ) NrRows() )
		return true;
	unsigned int i;
	for ( i = 0 ; i < NrCols(); i ++ )
		if ( Entry[row][i] != 0 )
			return false;
	return true;
}

int Matrix::SingleElementCol ( int col )
{
	bool foundit = false;
	int result, i;
	for ( i = 0 ; i < ( int ) Entry.size(); i ++ )
	{
		if ( Entry[i][col] != 0 )
		{
			if ( foundit )
				return -1;
			foundit = true;
			result = i;
		}
	}
	if(!foundit)
		return -1;
	return result;
}

int Matrix::SingleElementRow ( int row )
{
	if ( row >= ( int ) Entry.size() )
	{
		return -1;
	}
	bool foundit = false;
	int result, i;
	for ( i = 0 ; i < ( int ) Entry[row].size(); i ++ )
	{
		if ( Entry[row][i] != 0 )
		{
			if ( foundit )
				return -1;
			foundit = true;
			result = i;
		}
	}
	if(!foundit)
		return -1;
	return result;
}


void Matrix::FuseWithTranspose ( Matrix* LowerPart )
{
	if ( Entry.size() >0 )
	{
		if ( Entry[0].size() ==0 )
			return;
		if ( Entry[0].size() != LowerPart->NrRows() )
			return;
	}
	unsigned int i,j;
	vector<int> pusher;
	pusher.reserve ( LowerPart->NrRows() );
	for ( i = 0; i < LowerPart->NrCols(); i++ )
	{
		pusher.clear();
		for ( j = 0; j < LowerPart->NrRows(); j++ )
			pusher.push_back ( LowerPart->entry ( j,i ) );
		Entry.push_back ( pusher );
	}
}

void Matrix::Print()
{
	unsigned int i,j;
	for ( i = 0 ; i < Entry.size() ; i++ )
	{
		for ( j = 0 ; j < Entry[i].size(); j ++ )
			cout << Entry[i][j] << "  ";
		cout << '\n';
	}
}

void Matrix::PrintNonZero()
{
	unsigned int i,j;
	for ( j = 0 ; j < Entry[j].size(); j ++ )
		cout << j << '\t';
	cout << endl;
	for ( i = 0 ; i < Entry.size() ; i++ )
	{
		for ( j = 0 ; j < Entry[i].size(); j ++ )
			if(Entry[i][j] != 0)
				cout << Entry[i][j] << "\t";
			else
				cout << "\t";
		cout << '\n';
	}
}

void Matrix::DivideByRowGCD()
{
	unsigned int i,j;
	int gcd;
	for ( i = 0 ; i < Entry.size(); i++ )
	{
		gcd = 1 ;
		for ( j = 0 ; j < Entry[i].size(); j++ )
			gcd = GCD ( gcd,Entry[i][j] );
		if ( gcd != 0 && gcd != 1 )
			for ( j = 0 ; j < Entry[i].size(); j++ )
				Entry[i][j] /= gcd;
	}
}

void Matrix::SwitchRows ( unsigned int row1, unsigned int row2 )
{
	if ( row1 >= Entry.size() || row2 >= Entry.size() || row1 == row2 )
		return;
	vector<int> temp = Entry[row1];
	Entry[row1] = Entry[row2];
	Entry[row2] = temp;
}

void Matrix::GaussSubtract ( unsigned int srow, unsigned int scol, unsigned int row )
{
	int gcd = GCD ( Entry[srow][scol] , Entry[row][scol] );
	int sfactor =  Entry[row][scol] / gcd;
	int factor =  Entry[srow][scol] / gcd;
	unsigned int i;
	for ( i = 0 ; i < NrCols(); i++ )
		Entry[row][i] = Entry[row][i] * factor - Entry[srow][i] * sfactor;
}

void Matrix::GaussSubtract ( unsigned int srow,  unsigned int row, int sfactor, int factor )
{
	unsigned int i;
	for ( i = 0 ; i < NrCols(); i++ )
	{
		Entry[row][i] = Entry[row][i] * factor - Entry[srow][i] * sfactor;
	}
}

void Matrix::GiveRow ( unsigned int row, vector<int>* result )
{
	result->clear();
	if ( row >= NrRows() )
		return;
	*result = Entry[row];
}

unsigned int Matrix::FindKernel ( Matrix* Kernel )
{
//	char asd;
	Kernel->Clear();
	if ( NrRows() == 0 )
		return 0;
	unsigned int KernelDim = 0;
	unsigned int i,j,k;
	vector<bool> IsParameter ( NrCols(),false );
	vector<int> ColToRow ( NrCols() );
	for ( i = 0 ; i < NrCols(); i ++ )
	{
		j = i - KernelDim;
		while ( j < NrRows() && Entry[j][i] == 0 )
			j++;
		if ( j == NrRows() )
		{
			IsParameter[i] = true;
			KernelDim++;
		}
		else
		{
			SwitchRows ( j,i-KernelDim );
			j = i - KernelDim;
			ColToRow[i]=j;
			for ( k = 0 ; k < j ; k ++ )
				GaussSubtract ( j,i,k );
			for ( k = j+1 ; k < NrRows() ; k ++ )
				GaussSubtract ( j,i,k );
		}
	}
	if ( KernelDim == 0 )
		return 0;
	DivideByRowGCD();
	int hcf = 1;
	vector<int> pusher ( NrCols() );
	for ( i = 0 ; i < NrCols(); i++ )
	{
		if ( !IsParameter[i] )
		{
			hcf *= Entry[ColToRow[i]][i] / GCD ( hcf , Entry[ColToRow[i]][i] );
		}
		else
		{
			pusher.clear();
			pusher.resize ( NrCols() );
			for ( j = 0 ; j < i ; j ++ )
				if ( !IsParameter[j] )
					pusher[j] = hcf * Entry[ColToRow[j]][i] / Entry[ColToRow[j]][j];
			pusher[i]=-hcf;
			Kernel->push_row ( pusher );
		}
	}
	return KernelDim;
}


void Matrix::IsUnit ( unsigned int Dimension )
{
	this->Clear();
	vector<int> Pusher ( Dimension,0 );
	unsigned int i;
	for ( i = 0 ; i < Dimension ; i++ )
	{
		Pusher[i]=1;
		this->push_row ( Pusher );
		Pusher[i]=0;
	}
}

bool Matrix::BuildLeftDiagonalizer ( Matrix* Diagonalizer )
{
	Diagonalizer->IsUnit ( this->Entry.size() );
	Matrix Copy ( *this );
	if ( NrRows() == 0 )
		return true;
	unsigned int i,j,k;
	int gcd;
	for ( i = 0 ; i < NrCols(); i ++ )
	{
		j = i;
		while ( j < NrRows() && Entry[j][i] == 0 )
			j++;
		if ( j == NrRows() )
		{
			Entry = Copy.Entry;
			return false;
		}
		SwitchRows ( j,i );
		Diagonalizer->SwitchRows ( j,i );
		j = i;
		for ( k = 0 ; k < j ; k ++ )
		{
			gcd = GCD ( Entry[j][i] , Entry[k][i] );
			Diagonalizer->GaussSubtract ( j, k ,Entry[k][i] / gcd, Entry[j][i] / gcd );
			GaussSubtract ( j,i,k );
		}

		for ( k = j+1 ; k < NrRows() ; k ++ )
		{
			gcd = GCD ( Entry[j][i] , Entry[k][i] );
			Diagonalizer->GaussSubtract ( j, k ,Entry[k][i] / gcd, Entry[j][i] / gcd );
			GaussSubtract ( j,i,k );
		}
	}
	if ( NrRows() < NrCols() )
		for ( i = NrRows() ; i < NrCols(); i++ )
			if ( !ColZero ( i ) )
			{
				Entry = Copy.Entry;
				return false;
			}
	if ( NrRows() > NrCols() )
		for ( i = NrCols() ; i < NrRows(); i++ )
			if ( !RowZero ( i ) )
			{
				Entry = Copy.Entry;
				return false;
			}
	Entry = Copy.Entry;
	return true;
}

void Matrix::Transpose()
{
	Matrix Copy ( *this );
	Clear();
	vector<int> Pusher;
	SetNrRows ( Copy.NrCols() );
	for ( unsigned int i = 0 ; i < Copy.NrRows(); i++ )
	{
		Copy.GiveRow ( i,&Pusher );
		push_col ( Pusher );
	}
}

void Matrix::IsTransposeOf ( Matrix* TBT )
{
	Clear();
	vector<int> Pusher;
	SetNrRows ( TBT->NrCols() );
	for ( unsigned int i = 0 ; i < TBT->NrRows(); i++ )
	{
		TBT->GiveRow ( i,&Pusher );
		push_col ( Pusher );
	}
}

bool Matrix::IsProductOf ( Matrix* Left, Matrix* Right )
{
	Clear();
	if ( Left->NrCols() != Right->NrRows() )
		return false;
	SetNrRows ( Left->NrRows() );
	unsigned int i,j,k;
	int Pusher;
	for ( i = 0 ; i < Left->NrRows(); i++ )
		for ( j = 0 ; j < Right->NrCols(); j++ )
		{
			Pusher = 0;
			for ( k = 0 ; k < Left->NrCols(); k++ )
				Pusher += Left->entry ( i,k ) * Right->entry ( k,j );
			Entry[i].push_back ( Pusher );
		}
	return true;
}
