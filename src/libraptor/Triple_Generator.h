#ifndef TUPLE_GENERATOR_H
#define TUPLE_GENERATOR_H 


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <math.h>

#include "RandNum_Generator.h"
#include "Degree_Generator.h"


/*
   	*  d is a positive integer denoting an encoding symbol LT degree

    *  a is a positive integer between 1 and W-1 inclusive

    *  b is a non-negative integer between 0 and W-1 inclusive


*/

struct Triple
{
	/* data */
	uint32_t d;
	uint32_t a;
	uint32_t b;
};




class Triple_Generator
{
public:
	Triple_Generator(void);
	~Triple_Generator(void);


/*
      K - The number of source symbols

      X - An encoding symbol ID

      L be determined from K as described in Section 5.4.2.3
 
      L' be the smallest prime that is greater than or equal to L

*/
	struct Triple triple_generator(uint32_t K, uint32_t X, uint32_t LP);
	struct Triple _triple;


private:
	/* data */

	/*
    Q = 65521, the largest prime smaller than 2^^16.
	J(K) be the systematic index associated with K. For each value of K, the systematic index J(K) is designed to have
   the property that the set of source symbol triples (d[0], a[0],
   b[0]), ..., (d[L-1], a[L-1], b[L-1]) are such that the L intermediate
   symbols are uniquely defined, i.e., the matrix A in Section 5.4.2.4.2
   has full rank and is therefore invertible.

   The following is the list of the systematic indices for values of K
   between 4 and 8192 inclusive.

	*/


	struct Triple triple;
	uint16_t Q; //      Q = 65521, the largest prime smaller than 2^^16.
	static uint32_t J[];
	class RandNum_Generator rand;
	class Degree_Generator deg;


};



#endif

