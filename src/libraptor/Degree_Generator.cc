
#include "Degree_Generator.h"



Degree_Generator::Degree_Generator(void)
{


}


Degree_Generator::~Degree_Generator(void)
{

}


uint32_t Degree_Generator::f_table[] = {0, 10241, 491582, 712794, 831695, 948446, 1032189, 1048576};



uint32_t Degree_Generator::d_table[] = {1, 2, 3, 4, 10, 11, 40};


int Degree_Generator::deg_generator(int v)
{
	int index_d = 0;
	int deg = 0;
	// v is a non-negative integer that is less than 2^^20 = 1048576
	// please refer to rfc6330 5.3.5.2 degree generator
	if ((v >= 1048576) || (v < 0) )
	{
		return -1;
	}

	// choose the degree: lookup the f_tabole
	for(index_d = 1; index_d < 8; index_d++)
	{
		if((f_table[index_d - 1] <= v) && (v < f_table[index_d]))
		{
		     break;
		}
	}

	if (index_d > 8)
	{
		std::cerr << "Inconsistent table state" << std::endl;
	}

	deg = d_table[index_d - 1];

	return deg;

}
