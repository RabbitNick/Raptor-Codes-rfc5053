#ifndef UTILITY_H
#define UTILITY_H 


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <math.h>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>





using namespace boost::numeric::ublas;
namespace ublas = boost::numeric::ublas;


 //namespace ublas = boost::numeric::ublas;

class Utility
{
public:
	Utility(void);
	~Utility(void);

	uint32_t find_smallest_prime_integer(uint32_t v);

	//uint32_t nonZeros_In_Row(matrix_row<uint8_t> A);

	ublas::vector<uint8_t> matrix_row_XOR(ublas::vector<uint8_t> row_1, ublas::vector<uint8_t> row_2);


	/* data */

private:
	bool checking_prime_integer(uint32_t v);


};





#endif