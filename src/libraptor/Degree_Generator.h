#ifndef DEGREE_GENERATOR_H
#define DEGREE_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <math.h>




class Degree_Generator
{
public:
	Degree_Generator(void);
	~Degree_Generator(void);

	int deg_generator(int v);

private:
	/* data */
	static uint32_t f_table[];
	static uint32_t d_table[];

};




#endif


