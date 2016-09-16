#ifndef __LGOR_INIT_DATA__
#define __LGOR_INIT_DATA__

#include <string>


/*/* ****************************************************************************************************************************************

Initialization Data

Note: We keep things simple by allowing only INTEGER coefficients. I'm sure you can approximate any general superpotential well enough

Here: The Minimal T6 / Z3 Orbifold LGO Phase

**************************************************************************************************************************************** */


/*

std::string InitName = "T6Z3_Minimal";

#define InitNrFields 9
#define InitNrDiscretes 3
#define InitNrSuperpotTerms 9


int InitKOrder = 3;
int InitOrder[InitNrDiscretes] = {3,3,3};

int InitKCharge[InitNrFields] = {1,1,1,1,1,1,1,1,1};
int InitChargeMatrix[InitNrFields][InitNrDiscretes] = {{-2,0,-2},{1,0,0},{1,0,0}, {0,-2,1},{0,1,0},{0,1,0}, {0,0,1},{0,0,0},{0,0,0}};
std::string InitNames[InitNrFields] = {"11","12","13","21","22","23","31","32","33"}; 

int InitSuperpotPower[InitNrSuperpotTerms][InitNrFields] = {{3,0,0,0,0,0,0,0,0},{0,3,0,0,0,0,0,0,0},{0,0,3,0,0,0,0,0,0},{0,0,0,3,0,0,0,0,0},{0,0,0,0,3,0,0,0,0},{0,0,0,0,0,3,0,0,0},{0,0,0,0,0,0,3,0,0},{0,0,0,0,0,0,0,3,0},{0,0,0,0,0,0,0,0,3}};
int InitSuperpotKoeff[InitNrSuperpotTerms] = {1,1,1,1,1,1,1,1,1};


  //*/

// Another Test: The blanc SixTorus ( P^2_{1,1,1}[3] ) ^ 3 at the Fermat Point



#define InitNrFields 9
#define InitNrDiscretes 2
#define InitNrSuperpotTerms 9

std::string InitName = "P^2[3]^3_Fermat";

int InitKOrder = 3;
int InitOrder[InitNrDiscretes] = {3,3};

int InitKCharge[InitNrFields] = {1,1,1,1,1,1,1,1,1};
int InitChargeMatrix[InitNrFields][InitNrDiscretes] = {{-2,0},{1,0},{1,0}, {0,-2},{0,1},{0,1}, {0,0},{0,0},{0,0}};
std::string InitNames[InitNrFields] = {"11","12","13","21","22","23","31","32","33"}; 

int InitSuperpotPower[InitNrSuperpotTerms][InitNrFields] = {{3,0,0,0,0,0,0,0,0},{0,3,0,0,0,0,0,0,0},{0,0,3,0,0,0,0,0,0},{0,0,0,3,0,0,0,0,0},{0,0,0,0,3,0,0,0,0},{0,0,0,0,0,3,0,0,0},{0,0,0,0,0,0,3,0,0},{0,0,0,0,0,0,0,3,0},{0,0,0,0,0,0,0,0,3}};
int InitSuperpotKoeff[InitNrSuperpotTerms] = {1,1,1,1,1,1,1,1,1};


  //*/


// Another Test: The SixTorus, discretes symmetries removed at the Fermat Point

/*

#define InitNrFields 9
#define InitNrDiscretes 0
#define InitNrSuperpotTerms 9

std::string InitName = "A1^9";

int InitKOrder = 3;
int InitOrder[InitNrDiscretes] ;

int InitKCharge[InitNrFields] = {1,1,1,1,1,1,1,1,1};
int InitChargeMatrix[InitNrFields][InitNrDiscretes] ;
std::string InitNames[InitNrFields] = {"1","2","3","4","5","6","7","8","9"}; 

int InitSuperpotPower[InitNrSuperpotTerms][InitNrFields] = {{3,0,0,0,0,0,0,0,0},{0,3,0,0,0,0,0,0,0},{0,0,3,0,0,0,0,0,0},{0,0,0,3,0,0,0,0,0},{0,0,0,0,3,0,0,0,0},{0,0,0,0,0,3,0,0,0},{0,0,0,0,0,0,3,0,0},{0,0,0,0,0,0,0,3,0},{0,0,0,0,0,0,0,0,3}};
int InitSuperpotKoeff[InitNrSuperpotTerms] = {1,1,1,1,1,1,1,1,1};


  //*/


// Another Test: The SixTorus, 1 discretes symmetry

/*

#define InitNrFields 9
#define InitNrDiscretes 1
#define InitNrSuperpotTerms 9

std::string InitName = "A1^9Z3";

int InitKOrder = 3;
int InitOrder[InitNrDiscretes] = {3};

int InitKCharge[InitNrFields] = {1,1,1,1,1,1,1,1,1};
int InitChargeMatrix[InitNrFields][InitNrDiscretes] = {{1},{-1},{0},{0},{0},{0},{0},{0},{0}};
std::string InitNames[InitNrFields] = {"a","b","0","1","2","3","4","5","6"}; 

int InitSuperpotPower[InitNrSuperpotTerms][InitNrFields] = {{3,0,0,0,0,0,0,0,0},{0,3,0,0,0,0,0,0,0},{0,0,3,0,0,0,0,0,0},{0,0,0,3,0,0,0,0,0},{0,0,0,0,3,0,0,0,0},{0,0,0,0,0,3,0,0,0},{0,0,0,0,0,0,3,0,0},{0,0,0,0,0,0,0,3,0},{0,0,0,0,0,0,0,0,3}};
int InitSuperpotKoeff[InitNrSuperpotTerms] = {1,1,1,1,1,1,1,1,1};


  //*/



/*              For Test Purposes: The Quintic     */

/*
std::string InitName = "P^4[5]";

 
#define InitNrFields 5
#define InitNrDiscretes 0
#define InitNrSuperpotTerms 5


int InitKOrder = 5;
int InitOrder[InitNrDiscretes] ;

int InitKCharge[InitNrFields] = {1,1,1,1,1};
int InitChargeMatrix[InitNrFields][InitNrDiscretes] ;
std::string InitNames[InitNrFields] = {"1","2","3","4","5"}; 

// Fermat

#define InitNrSuperpotTerms 5
int InitSuperpotPower[InitNrSuperpotTerms][InitNrFields] = {{5,0,0,0,0},{0,5,0,0,0},{0,0,5,0,0},{0,0,0,5,0},{0,0,0,0,5}};
int InitSuperpotKoeff[InitNrSuperpotTerms] = {1,1,1,1,1}; //*/

// Non-Fermat
/*
#define InitNrSuperpotTerms 6
int InitSuperpotPower[InitNrSuperpotTerms][InitNrFields] = {{5,0,0,0,0},{0,5,0,0,0},{0,0,5,0,0},{0,0,0,5,0},{0,0,0,0,5},{1,4,0,0,0}};
int InitSuperpotKoeff[InitNrSuperpotTerms] = {1,1,1,1,1,1};
//*/


//*/

/*              For Test Purposes: The Mirror Quintic     */
/*


std::string InitName = "P^4[5]/Z5^3";

 
#define InitNrFields 5
#define InitNrDiscretes 3


int InitKOrder = 5;
int InitOrder[InitNrDiscretes] = {5,5,5};

int InitKCharge[InitNrFields] = {1,1,1,1,1};
int InitChargeMatrix[InitNrFields][InitNrDiscretes] = {{1,0,0},{-1,1,0},{0,-1,1},{0,0,-1},{0,0,0}};
std::string InitNames[InitNrFields] = {"1","2","3","4","5"}; 

// Fermat

#define InitNrSuperpotTerms 5
int InitSuperpotPower[InitNrSuperpotTerms][InitNrFields] = {{5,0,0,0,0},{0,5,0,0,0},{0,0,5,0,0},{0,0,0,5,0},{0,0,0,0,5}};
int InitSuperpotKoeff[InitNrSuperpotTerms] = {1,1,1,1,1}; //*/

// Non-Fermat
/*
#define InitNrSuperpotTerms 6
int InitSuperpotPower[InitNrSuperpotTerms][InitNrFields] = {{5,0,0,0,0},{0,5,0,0,0},{0,0,5,0,0},{0,0,0,5,0},{0,0,0,0,5},{1,1,1,1,1}};
int InitSuperpotKoeff[InitNrSuperpotTerms] = {1,1,1,1,1,1};
//*/


//*/


/*              For Test Purposes: The Quintic  w/ 1 discrete symmetry   */
/*


std::string InitName = "P^4[5]/Z5";

 
#define InitNrFields 5
#define InitNrDiscretes 1


int InitKOrder = 5;
int InitOrder[InitNrDiscretes] = {5};

int InitKCharge[InitNrFields] = {1,1,1,1,1};
int InitChargeMatrix[InitNrFields][InitNrDiscretes] = {{1},{-1},{0},{0},{0}};
std::string InitNames[InitNrFields] = {"1","2","3","4","5"}; 

// Fermat

#define InitNrSuperpotTerms 5
int InitSuperpotPower[InitNrSuperpotTerms][InitNrFields] = {{5,0,0,0,0},{0,5,0,0,0},{0,0,5,0,0},{0,0,0,5,0},{0,0,0,0,5}};
int InitSuperpotKoeff[InitNrSuperpotTerms] = {1,1,1,1,1}; //*/

// Non-Fermat
/*
#define InitNrSuperpotTerms 6
int InitSuperpotPower[InitNrSuperpotTerms][InitNrFields] = {{5,0,0,0,0},{0,5,0,0,0},{0,0,5,0,0},{0,0,0,5,0},{0,0,0,0,5},{1,1,1,1,1}};
int InitSuperpotKoeff[InitNrSuperpotTerms] = {1,1,1,1,1,1};
//*/


//*/


  //*/




#endif 
