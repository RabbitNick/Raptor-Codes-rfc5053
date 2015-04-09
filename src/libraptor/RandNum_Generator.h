
#ifndef RANDNUM_GENERATOR_H
#define RANDNUM_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <math.h>
//#include <cstdint>
//#include <stdexcept>


using namespace std;

class RandNum_Generator
{
public:
	RandNum_Generator(void);
	~RandNum_Generator(void);

	long generator(int y, int i, int m);
	void randnum_testing(void);

	/* data */
private:
	// x0,x1,x2,x3
	//uint32_t x[4];

	//V0,V1
	// There are 256 entries in each of the four arrays
	// The indexing into each array starts at 0, and the 
	// entries are 32-bit unsigned integers.
	static const uint32_t V[2][256];

};







#endif

