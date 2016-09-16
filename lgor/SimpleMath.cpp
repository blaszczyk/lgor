#include "SimpleMath.h"


int GCD(int a, int b)
{
if( a == 0 && b == 0 )
	return 1;
if(a<0)
a=-a;
if(b<0)
b=-b;
while(true)
{
if(a==0)
  return b;
b=b%a;
if(b==0)
  return a;
a=a%b;
}
}

int choose(int n, int k)
{
if(n<k)
  return 0;
if(n<0)
  n=-n;
if(k<0)
  k=-k;
int result = 1,i=1;
while(i <= k)
{
  result *= n;
  result /= i;
  i++;
  n--;
}
return result;
}

int TrueMod(int a, int b)
{
    a %= b;
    if(a<0)
           return a+b;
    return a;
}

int choosesort(int n, int k)
{
if(k<0)
  return 0;
//if(n<=k)
//  return 1;
//if(n<0)
//  n=-n;
int result = 1;
for ( k = n-k; k < n ; n--)
  result *= n;
return result;
}

unsigned int PlusMod(unsigned int a, unsigned int b, unsigned int m)
{
	if(m == 0)
		return 0;
	return (a + b) % m;
}

int Power(int Basis,unsigned int Exponent)
{
	int result = 1;
	while(Exponent-- > 0)
		result *= Basis;
	return result;
}
