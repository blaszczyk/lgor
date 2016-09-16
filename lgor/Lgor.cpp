#include "Lgor.h"
#include "SimpleMath.h"
#include "A19Models.h"
#include <string>
#include <vector>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>

using namespace std;


void Lgor::SortStrings(vector<lgostring>* strings) // Bubble Sort
{
	bool sorted = false;
	lgostring temp;
	unsigned int i;
	while(!sorted)
	{
		sorted = true;
		for( i = 1 ; i < strings->size() ; i++)
			if(strcmp( strings->at(i-1).at , strings->at(i).at ) > 0)
			{
				strcpy( temp.at , strings->at(i-1).at );
				strcpy( strings->at(i-1).at , strings->at(i).at );
				strcpy( strings->at(i).at , temp.at );
				sorted = false;
			}
	}
}


bool Lgor::scanFolder(char pathname[256], vector<lgostring>* lgoFile, vector<lgostring>* lgoDir)
{
	char filename[256];
	char fileinpathname[256];
	unsigned int flength;
	struct lgostring fname;
	struct dirent *entry;
	DIR *directory;
	struct stat status;
	directory = opendir(pathname);
	if(directory == NULL)
	{
		cout << "Error reading folder " << pathname << endl;
		return false;
	}
	lgoFile->clear();
	lgoDir->clear();
	while( (entry = readdir(directory)) != NULL )
	{
		strcpy(filename,entry->d_name);
		strcpy(fileinpathname,pathname);
		strcat(fileinpathname,"/");
		strcat(fileinpathname,filename);
		lstat(fileinpathname,&status);
		if(S_ISDIR(status.st_mode))
		{
			strcpy(fname.at,filename);
			lgoDir->push_back(fname);
		}
		else
		{
			flength = strlen(filename);
			if( flength > 4 )
				if(filename[flength-4] == '.' && filename[flength-3] == 'l' && filename[flength-2] == 'g' && filename[flength-1] == 'o')	
				{
					strcpy(fname.at,filename);
					lgoFile->push_back(fname);
				}
		}
	}
	closedir(directory);
	SortStrings(lgoDir);
	SortStrings(lgoFile);
	return true;
}

void Lgor::scanModels( char* pathname , vector<lgostring>* lgoFile )
{
	unsigned int i;
	char pathfilename[256];
	for( i = 0 ; i < lgoFile->size() ; i++ )
	{
		Mdl.Clear();
		strcpy(pathfilename,pathname);
		strcat(pathfilename,"/");
		strcat(pathfilename,lgoFile->at(i).at);
		Mdl.Initialize(pathfilename);
		if(Mdl.ExistShortResults())
			cout << "Results for " << lgoFile->at(i).at << " already exist." << endl;
		else
		{
			Mdl.SaveResultsShort();
			if(!Mdl.LoadResults())
				Mdl.ScanAll();
			if(Mdl.SaveResults())
				cout << "Results written to " << Mdl.mdata.resfilename << endl;	
			Mdl.SaveResultsShort();
		}
	}
}


void Lgor::CreateA19Models()
{
	A19Models a19m;
	for( unsigned int i = 1 ; i < 7 ; i++ )
	{
		a19m.CreateA19Models(i);
		cout << "Created Z_3^" << i << " Models!" << endl;
	}
	return;
}

unsigned int Lgor::FindInequivalentShortSpectra( char* pathname , vector<lgostring>* lgoFile )
{
	unsigned int i,j;
	char pathfilename[256];
	char counter[] = {'0','0','0','0',0};
	vector<ModelSum> Sums;
	vector< vector< unsigned int > > FilePointer;
	ModelSum temp; 
	vector< unsigned int > tempVecInt(1);
	bool FoundIt;
	ofstream ofile;
	for( i = 0 ; i < lgoFile->size() ; i++ )
	{
		Mdl.Clear();
		strcpy(pathfilename,pathname);
		strcat(pathfilename,"/");
		strcat(pathfilename,lgoFile->at(i).at);
		pathfilename[strlen(pathfilename)-3] = 'l';
		pathfilename[strlen(pathfilename)-2] = 'r';
		pathfilename[strlen(pathfilename)-1] = 's';
		if(!temp.ReadFromFile(pathfilename))
			cout << "Error opening \"" << pathfilename << "\"!" << endl;
		if(temp.IsCorrect())
		{
			FoundIt = false;
			for( j = 0 ; j < Sums.size() ; j++ )
				if( temp.IsEqualTo( &( Sums[j] ) ) )
				{
					FilePointer[j].push_back(i);
					FoundIt = true;
					break;
				}
			if( ! FoundIt )
			{
				Sums.push_back( temp );
				tempVecInt[0] = i;
				FilePointer.push_back(tempVecInt);
			}
		}
	}
	for( i = 0 ; i < Sums.size() ; i++ )
	{
		strcpy(pathfilename,pathname);
		strcat(pathfilename,"/LgorModel");
		strcat(pathfilename,counter);
		strcat(pathfilename,".lms");
		ofile.open(pathfilename);
		if( ! ofile.is_open() )
			cout << "Error opening \"" << pathfilename << "\"!" << endl;
		else
		{
			ofile << "Spectrum Summary for Inequiv. Model Nr. " << counter << endl;
			Sums[i].PrintToFile(ofile);
			ofile << "Model appeares " << FilePointer[i].size() << " times in files:" << endl;
			for( j = 0 ; j < FilePointer[i].size() ; j++ )
				ofile << lgoFile->at(FilePointer[i][j]).at << endl;
			ofile.close();
		}
		counter[3]++;
		for( j = 3 ; j > 0 ; j-- )
			if( counter[j] > '9' )
			{
				counter[j] = '0';
				counter[j-1]++;
			}
	}
	cout << "Found " << Sums.size() << " inequivalent Spactra!" << endl;
	return Sums.size();
}

void Lgor::run()
{
	string command;
	bool engage = true;
	char pathname[256];
	char pathfilename[256];
	pathname[0]='.';
	pathname[1]=0;
	unsigned int flength,i,temp;
	vector<lgostring> lgoFile(0);
	vector<lgostring> lgoDir(0);
	scanFolder(pathname,&lgoFile,&lgoDir);
	while(engage)
	{
		cout << "lgor:/" << pathname << ">";
		cin >> command;
		switch(command[0])
		{
		case 'h':
			cout << "Enter m to load model from file \n";
			cout << "Enter a to comupe all models in folder \n";
			cout << "Enter i to find inequivalent spectra \n";
			cout << "Enter f to show Folders \n";
			cout << "Enter c to change Folder \n";
			cout << "Enter l to show *.lgo files \n";
			cout << "Enter x to exit \n";
			break;
		case 'x':
			engage = false;
			break;
		case 'c':
			if( command.length() > 0 && command[1] >= '0' && command[1] <= '9' )
			{
				i = 1;
				temp = 0;
				while(command[i] >= '0' && command[i] <= '9' )
					temp = 10 * temp + (unsigned int) command[i++] - '0';
			}
			else
			{
				cout << "Enter Number:";
				cin >> temp;
			}
			if( temp < lgoDir.size())
			{
				if( strcmp( lgoDir[temp].at , "." ) != 0)
				{
					if( strcmp( lgoDir[temp].at , ".." ) == 0)
					{
						flength = strlen ( pathname );
						while( flength != 0 && pathname[flength] != '/' )
							flength --;
						if(flength == 0)
							strcpy(pathname,".");
						else
							pathname[flength] = 0;
					}
					else
					{
						strcat(pathname,"/");
						strcat(pathname,lgoDir[temp].at);
					}
				}
			}
			scanFolder(pathname,&lgoFile,&lgoDir);
			cout << "Showing content of folder \"" << pathname << "\":" << endl;
			cout << "Nr *.lgo files = " << lgoFile.size() << endl; 
			cout << "Subfolders:" << endl;
			for( i = 0 ; i < lgoDir.size(); i++)
				cout << i << ".\t" << lgoDir.at(i).at << endl;
			break;
		case 'f':
			for( i = 0 ; i < lgoDir.size(); i++)
				cout << i << ".\t" << lgoDir.at(i).at << endl;
			break;
		case 'l':
			for( i = 0 ; i < lgoFile.size(); i++)
				cout << i << ".\t" << lgoFile.at(i).at << endl;
			break;
		case 'm':
			if( command.length() > 0 && command[1] >= '0' && command[1] <= '9' )
			{
				i = 1;
				temp = 0;
				while(command[i] >= '0' && command[i] <= '9' )
					temp = 10 * temp + (unsigned int) command[i++] - '0';
			}
			else
			{
				cout << "Enter Number:";
				cin >> temp;
			}
			if( temp < lgoFile.size())
			{
				strcpy(pathfilename,pathname);
				strcat(pathfilename,"/");
				strcat(pathfilename,lgoFile.at(temp).at);
				Mdl.Clear();
				Mdl.Initialize(pathfilename);
				Mdl.run();
			}
			break;
		case 'a':
			scanModels( pathname , &lgoFile );
			break;
		case 'i':
			FindInequivalentShortSpectra( pathname , &lgoFile );
			break;
		case 'z':
			CreateA19Models();
			break;
		}
	}
	return;
}
