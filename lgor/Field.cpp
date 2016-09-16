#include "Field.h"



Field::Field(int NrDiscretes) : Charge(NrDiscretes) {  };

int Field::GiveCharge(int a) {
return Charge[a];
}

bool Field::SetCharge(int b, int* a) {
  Kcharge=b;
  for(unsigned int i=0;i<Charge.size();i++)
  {
     Charge[i]=a[i];
  }
return true;
}
