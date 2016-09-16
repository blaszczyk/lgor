#include "ModelData.h"
#include "SimpleMath.h"
#include <iostream>
#include <fstream>
#include "InitData.h"
#include "PrintDef.h"
#include <string.h>
#include "Parseline.h"

using namespace std;

ModelData::ModelData()
{               
}



bool ModelData::Initialize(char filename[256])
{
	strcpy(this->filename,filename);
	if(!InitFromFile())
		InitFromHeader();
	InitHCF();
	return CheckConsistency();
}

void ModelData::Clear()
{
	NrFields = 0;
	NrDiscretes = 0;
	NrSuperpotTerms = 0;

	Name = "";	
	filename[0]=0;	
	resfilename[0]=0;

	KOrder=0;
	Order.clear();
	
	Fld.clear();

	SuperPotential.term.clear();
}

bool ModelData::InitFromFile()
{
	string line,temp;
	int i;
//	GetFileName(filename);
	ifstream datafile (filename);
	if(!datafile.is_open())
	{
		cout << "Failed to open " << filename << endl;
		return false;
	}
	i = 0;
	while(filename[i] != 0)
		i++;
	strcpy(resfilename,filename);
	resfilename[i-3] = 'l';
	resfilename[i-2] = 'r';
	resfilename[i-1] = 't';
	parseline pline;
	Field tmp(0);
	SuperMonomial tempMono(0);
	NrFields = 0;
	NrDiscretes = 0;
	NrSuperpotTerms = 0;
	Fld.clear();
	SuperPotential.term.clear();
	bool gotName = false;
	bool gotKOrder = false;
	bool gotFld;
	while(!datafile.eof())
	{
		getline(datafile,line);
		pline.writeLine(line);
		pline.gotoLetter();
		temp = pline.getString(" \t\n");
		if( temp == "Name" )
			if(pline.gotoChar("\""))
			{
				pline.Skip(1);
				this->Name = pline.getString("\"");
				gotName = true;
			}
		if( temp == "Korder" || temp == "KOrder"  )
			if(pline.gotoInt())
			{
				this->KOrder = (int) pline.getInt();
				gotKOrder = true;
			}
		if( temp == "Order"  && NrFields == 0)
			while(pline.gotoInt())
			{
				this->Order.push_back((int) pline.getInt());
				NrDiscretes++;
			}
		if( temp == "Field"  && NrSuperpotTerms == 0)
		{
			gotFld = true;
			if(!pline.gotoChar("\""))
				gotFld = false;
			pline.Skip(1);
			tmp.Name = pline.getString("\"");
			if(!pline.gotoInt())
				gotFld = false;
			tmp.Kcharge = (int) pline.getInt();
			tmp.Charge.clear();
			for( i = 0 ; i < NrDiscretes ; i++)
			{

				if(!pline.gotoSignedInt())
					gotFld = false;
				tmp.Charge.push_back( pline.getSignedInt());
			}
			if(gotFld)
			{
				this->Fld.push_back(tmp);
				NrFields ++;
			}
			else
				cout << "Invalid Field" << endl;
		}
		if( temp == "SPotTerm" )
		{
			gotFld = true;
			tempMono.Power.clear();
			if(!pline.gotoSignedInt())
				gotFld = false;
			tempMono.Koeff = pline.getSignedInt();
			for( i = 0 ; i < NrFields ; i++ )
			{
				if(!pline.gotoInt())
					gotFld = false;
				tempMono.Power.push_back( (int) pline.getInt());
			}
			if(gotFld)
			{
				this->SuperPotential.term.push_back(tempMono);
				NrSuperpotTerms++;
			}
			else
				cout << "Invalid Superpotential Term" << endl;
		}

	}
	datafile.close();
	if(!gotName)
		this->Name = "NoName";
	if(!gotKOrder)
	{
		cout << "Error: No K-Order" << endl;
		return false;
	}
	if(NrFields == 0)
	{
		cout << "Error: No Fields" << endl;
		return false;
	}
	if(NrSuperpotTerms == 0)
	{
		cout << "Error: No Superpotential" << endl;
		return false;
	}
	cout << "Model Input from " << filename << " successful" << endl; 
	cout << Name << " contains " << NrFields << " Fields , " << NrDiscretes << " discrete Symmetries and " << NrSuperpotTerms << " Superpotential Terms" << endl;
	return true;
}

void ModelData::InitFromHeader()
{
	int i,j;
	NrFields = InitNrFields;
	NrDiscretes = InitNrDiscretes;
	NrSuperpotTerms = InitNrSuperpotTerms;
	Name = InitName;
	KOrder = InitKOrder;
	for(i = 0 ; i < NrDiscretes; i ++)
		Order.push_back(InitOrder[i]);
	Fld.resize(NrFields,Field(NrDiscretes));
	for(i=0;i<NrFields;i++)
	{
		Fld[i].SetCharge(InitKCharge[i],InitChargeMatrix[i]);
		Fld[i].Name=InitNames[i];
	}
	SuperPotential.term.resize(NrSuperpotTerms,SuperMonomial(NrFields));
	for(i=0; i<NrSuperpotTerms; i++)
	{
		for(j=0; j<NrFields; j++)
			SuperPotential.term[i].Power[j] = InitSuperpotPower[i][j];
		SuperPotential.term[i].Koeff = InitSuperpotKoeff[i];
	}
}

void ModelData::InitHCF()
{
	OrderHCF = 2*KOrder;
	for(int i = 0; i<NrDiscretes; i++)
		OrderHCF = OrderHCF * Order[i] / GCD(OrderHCF,Order[i]);
	KOrderF=OrderHCF/KOrder;
	for(int i = 0; i<NrDiscretes; i++)
		OrderF.push_back(OrderHCF/Order[i]);
}

bool ModelData::CheckConsistency()
{
	int i,j,k,sum;
for(i=0; i<NrDiscretes;i++)
{
  sum=0;
  for(j=0; j<NrFields; j++)
    sum += Fld[j].Charge[i];
  if(sum!=0)
  {
    cout << "Charges of discrete symmetry No " << i << " do not sum to Zero" << endl;
    for(j=0; j<NrFields; j++)
      cout <<  Fld[j].Charge[i] << ' ';
    return false;
  }
}
sum=0;
for(i=0; i<NrFields; i++)
  sum += KOrder - 2 * Fld[i].Kcharge;
if(sum != 3 * KOrder)
{
  cout << "Central Charges will not sum to proper value" << endl;
  return false;
}
for(i=0; i<NrSuperpotTerms; i++)
{
  sum = 0;
  for(j=0;j<NrFields; j++)
  {
    if(SuperPotential.term[i].Power[j] < 0)
    {
      cout << "Superpotential not holomorphic!" << endl;
      return false;
    } 
    sum += SuperPotential.term[i].Power[j] * Fld[j].Kcharge;
  }
  if(sum != KOrder)
  {
    cout << " Term No " << i << " in Superpotential does not have proper R-Charge" << endl;
  for(j=0;j<NrFields; j++)
	cout << SuperPotential.term[i].Power[j] << ' ' <<  Fld[j].Kcharge << '\t';
	cout << endl;
    return false;
  }
  for(k=0; k<NrDiscretes; k++)
  {
    sum = 0;
    for(j=0;j<NrFields; j++)
      sum += SuperPotential.term[i].Power[j] * Fld[j].Charge[k];
    if( TrueMod( sum , Order[k] ) != 0 )
    {
      cout << " Term No " << i << " in Superpotential does not have proper Charge under discrete Symmetry no" << k << endl;
      return false;
    }
  }
}

return true;
}

void ModelData::Print()
{
	int i,j;
	cout << "Model : " << Name << '\n';
	cout << "K-Order : " << KOrder << '\n';
	cout << "Discrete Symmetry Orders : ";
	for( i = 0 ; i < NrDiscretes; i ++)
		cout << Order[i] << '\t';
	cout << '\n';
	cout << " Charge Table: \n";
	cout << "\t k \t";
	for( i = 0 ; i < NrDiscretes; i ++)
		cout << "k" << i << '\t';
	cout << '\n';
	for( i = 0; i < NrFields ; i++)
	{
		cout << SuperFieldName << Fld[i].Name << "\t" << Fld[i].Kcharge << "\t";
		for ( j = 0 ; j < NrDiscretes ; j++)
			cout << Fld[i].Charge[j] << "\t";
		cout << "\n";
	}
}
