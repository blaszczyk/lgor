#include "Model.h"
#include "Parseline.h"
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;



Model::Model() : initialized ( false ), mdata(), Bnch(0,Bunch(&mdata)) {}




bool Model::Initialize(char filename[256])
{
	initialized = mdata.Initialize(filename);
	return initialized;
}

void Model::Clear()
{
	initialized = false;
	Bnch.resize(0,Bunch(&mdata));
	mdata.Clear();
}


bool Model::SaveResultsShort()
{
//	unsigned int i , j , k;
	char filename[256];
	vector<SpectrumSum> Sums(0);
	vector< vector<unsigned int> > BunchPointer;
	vector<unsigned int> NewLine(1);
//	bool newSum;
	strcpy(filename,mdata.resfilename);
	filename[strlen(filename)-1]='s';
	ofstream ofile(filename);
	if(!ofile.is_open())
	{
		cout << "Failed to open " << filename << endl;
		return false;
	}
	ofile << "Results for model " << mdata.Name << endl;
	ofile << "by LGOR, Team Beyond Standard" << endl;
	Sum.Create(&Bnch);
	Sum.PrintToFile(ofile);
	ofile.close();

/*	ofile << "Spectrum Summary:" << endl;
	Sum.PrintToFile(ofile);
	for( i = 0 ; i < Bnch.size(); i++ )
	{
		newSum = true;
		for( j = 0 ; j < Sums.size() ; j++ )
			if(Bnch[i].Sum.IsEqualTo(&Sums[j]))
			{
				newSum=false;
				BunchPointer[j].push_back(i);
				break;
			}
		if(newSum)
		{
			Sums.push_back(Bnch[i].Sum);
			NewLine[0] = i;
			BunchPointer.push_back(NewLine);
		}
	}
	ofile << "Subspectra in Bunches:" << endl;
	for( i = 0 ; i < Sums.size() ; i++ )
	{
		ofile << "New Subspectrum:" << endl;
		Sums[i].PrintToFile(ofile);
		ofile << "In " <<  BunchPointer[i].size() << " Bunches:" << endl;
		for( k = 0 ; k < BunchPointer[i].size() ; k++ )
		{
			for( j = 0 ; j < (unsigned int) mdata.NrDiscretes; j ++)
				ofile << Bnch[BunchPointer[i][k]].Twist[j] << " ";
			ofile << endl;
		}
	}
*/
	return true;
}

bool Model::ExistShortResults()
{
	bool exists;
	char filename[256];
	strcpy(filename,mdata.resfilename);
	filename[strlen(filename)-1]='s';
	ifstream ifile(filename);
	exists = ifile.is_open();
	ifile.close();
	return exists;
}

bool Model::SaveResults()
{
	unsigned int i = 0,j,k,l,m,n,o;	
	unsigned int slimit;
	ofstream ofile(mdata.resfilename);
	if(!ofile.is_open())
	{
		cout << "Failed to open " << mdata.resfilename << endl;
		return false;
	}
	ofile << "Results for model " << mdata.Name << endl;
	ofile << "by LGOR, Team Beyond Standard" << endl;
	ofile << "Spectrum Summary:" << endl;
	Sum.MdlSum.PrintToFile(ofile);
	for( i = 0 ; i < Bnch.size(); i++ )
	{
		ofile << "Bunch ";
		for( j = 0 ; j < (unsigned int) mdata.NrDiscretes; j ++)
			ofile << Bnch[i].Twist[j] << " ";
		ofile << endl;
		slimit = 5;
		for( j = 0 ; j < 2* (unsigned int) mdata.KOrder ; j++ )
		{
			slimit = 9 - slimit;
			ofile << "Sector " << j << endl;
			for ( k = 0 ; k < slimit ; k++)
				for( l = 0 ; l < 4 ; l++ )
				{
					if(Bnch[i].Sctr[j].Cmplx[k].cohomology[l].term.size() != 0)
						ofile << "Cspace " << k << " " << l << endl;
					for(m = 0 ; m <  Bnch[i].Sctr[j].Cmplx[k].cohomology[l].term.size() ; m++ )
					{
						ofile << "State " << endl;
						for( n = 0 ; n < Bnch[i].Sctr[j].Cmplx[k].cohomology[l].term[m].term.size() ; n++)	
						{
							ofile << "Term  " << Bnch[i].Sctr[j].Cmplx[k].cohomology[l].term[m].Koeff[n] << "\t";
							for( o = 0 ; o < (unsigned int) mdata.NrFields ; o++)
							{
								ofile <<  Bnch[i].Sctr[j].Cmplx[k].cohomology[l].term[m].term[n].BPower[o] << " ";
								ofile <<  Bnch[i].Sctr[j].Cmplx[k].cohomology[l].term[m].term[n].BBPower[o] << " ";
								ofile <<  Bnch[i].Sctr[j].Cmplx[k].cohomology[l].term[m].term[n].FPower[o] << " ";
								ofile <<  Bnch[i].Sctr[j].Cmplx[k].cohomology[l].term[m].term[n].BFPower[o] << "  ";
							}
							ofile << endl;
						}
					}
				}
		}
	}
	ofile.close();
	return true;
}

bool Model::LoadResults()
{
	ifstream ifile(mdata.resfilename);
	if(!ifile.is_open())
	{
		cout << "Failed to open " << mdata.resfilename << endl;
		return false;
	}
	Monomial tempMono(&mdata);
	string line,temp;
	parseline pline;
	unsigned int BnchCounter = 0;
	unsigned int SctrCounter = 0;
	unsigned int QRightCounter = 0;
	unsigned int QLeftCounter = 0;
	unsigned int StateCounter = 0;
	unsigned int i;
	vector<int> tempVecInt;
	while(!ifile.eof())
	{
		getline(ifile,line);
		pline.writeLine(line);
		pline.gotoLetter();
		temp = pline.getString(" \t\n");
		if( temp == "Bunch" )
		{
			tempVecInt.clear();
			for( i = 0 ; i < (unsigned int)mdata.NrDiscretes; i++)
			{
				if(pline.gotoInt())
					tempVecInt.push_back(pline.getInt());
				else
					return false;
			}
			Bnch.push_back(Bunch(tempVecInt,&mdata));
			for ( i = 0 ; i < 2 * (unsigned int)mdata.KOrder; i++ )
				Bnch[BnchCounter].Sctr.push_back ( Sector ( i,Bnch[BnchCounter].Twist,&mdata ) );
			BnchCounter++;
		}
		if( temp == "Sector")
		{
			if(!pline.gotoInt())
				return false;
			SctrCounter = pline.getInt();
			Bnch[BnchCounter-1].Sctr[SctrCounter].CreateSFields();
			Bnch[BnchCounter-1].Sctr[SctrCounter].ComputeVacuumQuantumNumbers();
		}
		if( temp == "Cspace" )
		{
			if(!pline.gotoInt())
				return false;
			QLeftCounter = pline.getInt();
			if(!pline.gotoInt())
				return false;
			QRightCounter = pline.getInt();
			StateCounter = -1;
		}
		if( temp == "State" )
		{
			Bnch[BnchCounter-1].Sctr[SctrCounter].Cmplx[QLeftCounter].cohomology[QRightCounter].term.push_back(Polynomial());
			StateCounter ++;
		}
		if( temp == "Term" )
		{
			pline.gotoSignedInt();
			Bnch[BnchCounter-1].Sctr[SctrCounter].Cmplx[QLeftCounter].cohomology[QRightCounter].term[StateCounter].Koeff.push_back( pline.getSignedInt() );
			tempMono.ClearPowers();
			for( i = 0 ; i < (unsigned int)mdata.NrFields ; i++)
			{
				pline.gotoInt();
				tempMono.BPower[i] = pline.getInt();
				pline.gotoInt();
				tempMono.BBPower[i] = pline.getInt();
				pline.gotoInt();
				tempMono.FPower[i] = pline.getInt();
				if(!pline.gotoInt())
					return false;
				tempMono.BFPower[i] = pline.getInt();
			}
			Bnch[BnchCounter-1].Sctr[SctrCounter].Cmplx[QLeftCounter].cohomology[QRightCounter].term[StateCounter].term.push_back(tempMono);
		}
	}
	for( i = 0 ; i < BnchCounter ; i++ )
	{
		Bnch[i].SummarizeSpectrum();
		Bnch[i].CheckE6Consistency();
	}
	cout << "Results Loaded sucessfully. Spectrum Summary:" << endl;
	PrintE6Summary();
//	if(BnchCounter == 0)
//		return false;
	return true;
}

void Model::ScanAll()
{
	vector<int> BnchPointer(mdata.NrDiscretes,0);
	Bnch.clear();
	if(mdata.NrDiscretes == 0)
	{
		Bnch.push_back(Bunch(BnchPointer,&mdata));
		Bnch[0].Initialize();
		return;
	}
	bool engage = true;
	int i;
	while(engage)
	{
		Bnch.push_back(Bunch(BnchPointer,&mdata));
		Bnch[Bnch.size()-1].Initialize();
		BnchPointer[0]++;
		for(i = 1 ; i < mdata.NrDiscretes ; i++)
			if(BnchPointer[i-1] == mdata.Order[i-1])
			{
				BnchPointer[i-1] = 0;
				BnchPointer[i]++;
			}
		if(BnchPointer[mdata.NrDiscretes-1] == mdata.Order[mdata.NrDiscretes-1])
			engage = false;
	}
	Sum.Create( &Bnch );
}

void Model::PrintE6Summary()
{
	Sum.Clear();
	Sum.Create( &Bnch );
	Sum.Print();
}

void Model::PrintBunches(bool skipEmpty)
{
	unsigned int i,j,k,PrintPtr = 0;
	unsigned int temp;
	const unsigned int HPack = 50;
	unsigned int VPack = 4;
	if( mdata.NrDiscretes > 6)
		VPack = 3;
	vector<unsigned int> BnchPtr;
	BnchPtr.reserve(Bnch.size());
	for( i = 0 ; i < Bnch.size(); i++)
		if( ! ( Bnch[i].empty && skipEmpty ) )
			BnchPtr.push_back(i);
	while(PrintPtr < BnchPtr.size())
	{
		for( i = 0 ; i < HPack; i ++)
		{
			for( j = 0 ; j < VPack ; j++)
				if( PrintPtr + HPack * j + i < BnchPtr.size())
				{
					temp = PrintPtr + HPack * j + i;
					if(BnchPtr[temp] < 1000)
						cout << ' ';
					if(BnchPtr[temp] < 100)
						cout << ' ';
					if(BnchPtr[temp] < 10)
						cout << ' ';
					cout << BnchPtr[temp] << ". " << 2 * mdata.KOrder << "-plet : ( " << "x";
					for ( k = 0 ; k < Bnch[BnchPtr[temp]].Twist.size(); k ++ )
						cout << "," << Bnch[BnchPtr[temp]].Twist[k];
					cout << " )\t";
				}
			if( PrintPtr + i < BnchPtr.size())
				cout << endl;
		}
		cout << endl;
		PrintPtr += HPack * VPack;
	}
}

void Model::run()
{
	if ( !initialized )
	{
		cout << "Initialize first" << endl;
		return;
	}
	string command;
	bool engage = true;
	int temp;


	while ( engage )
	{
		cout << "lgor:/" << mdata.Name << ">";
		cin >> command;
		switch ( command[0] )
		{
			case 'x':
				engage = false;
				break;
			case 'h':
				cout << "Press n to create new " << 2 * mdata.KOrder << "-plet of sectors \n";
				cout << "Press b to enter " << 2 * mdata.KOrder << "-plet of sectors \n";
				cout << "Press p to list " << 2 * mdata.KOrder << "-plets of sectors \n";
				cout << "Press f to list non-empty " << 2 * mdata.KOrder << "-plets of sectors \n";
				cout << "Press a to scan all Bunches of Sectors \n";
				cout << "Press s to save results to file \n";
				cout << "Press l to load results from file \n";
				cout << "Press d to print model data \n";
				cout << "Press e to print spectrum summary \n";
				cout << "Press x to close model \n";
				break;
			case 'n':
				Bnch.push_back ( Bunch ( &mdata ) );
				Bnch[Bnch.size()-1].run ( true );
				break;
			case 'b':
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
					case 4:
						temp = ( ( int ) command[1]-'0' ) * 100 + ( ( int ) command[2]-'0' ) * 10 + ( int ) command[3]-'0' ;
						break;
					default:
						temp = ( ( int ) command[1]-'0' ) * 1000 + ( ( int ) command[2]-'0' ) * 100 + ( ( int ) command[3]-'0' ) * 10 + ( int ) command[4]-'0' ;
						break;
				}
				if ( temp > -1 && temp < ( int ) Bnch.size() )
					Bnch[temp].run ( false );
				break;
			case 'e':
				PrintE6Summary();
				break;
			case 'p':
				PrintBunches(false);
				break;
			case 'f':
				PrintBunches(true);
				break;
			case 'd':
				mdata.Print();
				break;
			case 'a':
				ScanAll();
				break;
			case 's':
				if(SaveResults())
					cout << "Results written to file " << mdata.resfilename << endl;
				else
					cout << "Write error" << endl;
				break;
			case 'l':
				if(!LoadResults())
					cout << "Load error" << endl;
				break;
		}
	}
}
