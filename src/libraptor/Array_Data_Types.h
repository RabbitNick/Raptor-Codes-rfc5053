
#ifndef ARRAY_DATA_TYPES_H
#define ARRAY_DATA_TYPES_H


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <stdint.h>
#include <math.h>


#include "Array_Data_Types.h"
#include "Utility.h"


/*

      X be the smallest positive integer such that X*(X-1) >= 2*K.

      S be the smallest prime integer such that S >= ceil(0.01*K) + X

      H be the smallest integer such that choose(H,ceil(H/2)) >= K + S

      H' = ceil(H/2)

      L = K+S+H

      C[0],...,C[K-1] denote the first K intermediate symbols

      C[K],...,C[K+S-1] denote the S LDPC symbols, initialised to zero

      C[K+S],...,C[L-1] denote the H Half symbols, initialised to zero


*/



class Vector_Data_DEF
{
public:
	//Vector_Data_DEF(uint32_t row, uint32_t col);
	Vector_Data_DEF(void);
	Vector_Data_DEF(uint32_t len);
	~Vector_Data_DEF(void);

	//std::vector< std::vector<uint8_t> > symbol;
	void allocate_size(uint32_t len);

	std::vector<uint8_t> s;

	void copy(class Vector_Data_DEF v);

	Vector_Data_DEF  operator^ (Vector_Data_DEF &v);

	int data_reading(uint8_t *src);

	/* data */
	uint32_t d_len;
//	uint32_t d_row;
//	uint32_t d_col;

};



//template <class T>
class Array_Data_Symbol
{
public:
	Array_Data_Symbol(void);
	Array_Data_Symbol(uint32_t K);
	Array_Data_Symbol(uint32_t K, uint32_t symbol_len);

	~Array_Data_Symbol(void);

	int array_allocation(uint32_t size, uint32_t symbol_len);



	uint32_t sym_len;
	//std::vector<uint8_t> array;

	//X be the smallest positive integer such that X*(X-1) >= 2*K.
	uint32_t X;
	//S be the smallest prime integer such that S >= ceil(0.01*K) + X
	uint32_t S;
	//H be the smallest integer such that choose(H,ceil(H/2)) >= K + S
	uint32_t H;
	//H' = ceil(H/2)
	uint32_t HP;
	// L = K+S+H
	uint32_t L;

	// L` is the smallest prime integer greater than or equal to L
	uint32_t LP;

	// the number of source symbols
	uint32_t K;

	//An encoding symbol ID
	//uint32_t ESI;
	std::vector<uint32_t> ESIs;

	std::vector< class Vector_Data_DEF > symbol;

	int symbol_reading(class Vector_Data_DEF *src);


	//class Vector_Data_DEF *symbol_pointer;



	// std::vector< std::vector<uint8_t> > operator^ (std::vector< std::vector<uint8_t> > v);

private:
	int init(uint32_t K);
	int nChoosek( uint32_t n, uint32_t k );
	void get_parameters(void);
	int array_allocation(uint32_t symbol_len);



	class Utility util;





	/* data */
	uint32_t _K;
	uint32_t _X;
	uint32_t _S;
	uint32_t _H;
	uint32_t _HP;
	uint32_t _L;
	uint32_t _LP;
	uint32_t _sym_len;
};






#endif