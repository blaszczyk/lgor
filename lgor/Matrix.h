#ifndef __LGOR_MATRIX__
#define __LGOR_MATRIX__


#include <vector>
/* ****************************************************************************************************************************************

MAtrix

**************************************************************************************************************************************** */


class Matrix
{
	private:
		std::vector< std::vector<int> > Entry;

	public:
		void push_row ( std::vector<int> );
		void push_col ( std::vector<int> );
		int entry ( int, int );
		unsigned int NrRows();
		unsigned int NrCols();

		Matrix();
		Matrix ( const Matrix& );

		void Print();
		void PrintNonZero();
		void SetNrRows ( unsigned int );
		void Clear();
		bool ColZero ( int );
		bool RowZero ( int );
		int SingleElementRow ( int );
		int SingleElementCol ( int );
		void IsUnit ( unsigned int );
		void FuseWithTranspose ( Matrix* );
		void DivideByRowGCD();
		void SwitchRows ( unsigned int, unsigned int );
		void GaussSubtract ( unsigned int, unsigned int, unsigned int );
		void GaussSubtract ( unsigned int, unsigned int, int, int );
		void GiveRow ( unsigned int, std::vector<int>* );
		void Transpose();
		void IsTransposeOf ( Matrix* );
		bool IsProductOf ( Matrix*, Matrix* );
		unsigned int FindKernel ( Matrix* );
		bool BuildLeftDiagonalizer ( Matrix* );
};


#endif
