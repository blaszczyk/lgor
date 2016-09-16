#ifndef __LGOR_FIELD__
#define __LGOR_FIELD__

#include <string>
#include <vector>
/*****************************************************************************************************************************************

Here Comes the Fields Class

**************************************************************************************************************************************** */



class Field {

public:
  int Kcharge;
  std::vector<int> Charge;
  std::string Name;
  
  Field(int);
  int GiveCharge(int);
  bool SetCharge(int,int[]);
};


#endif
