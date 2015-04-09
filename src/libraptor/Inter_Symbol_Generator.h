
#ifndef INTER_SYMBOL_GENERATOR_H
#define INTER_SYMBOL_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <math.h>
#include <vector>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>


#include "Array_Data_Types.h"
#include "Triple_Generator.h"
//#include "storage_adaptors.h"



//using namespace boost::numeric::ublas;


/*

   The pre-coding relationships amongst the L intermediate symbols are
   defined by expressing the last L-K intermediate symbols in terms of
   the first K intermediate symbols.

   The last L-K intermediate symbols C[K],...,C[L-1] consist of S LDPC
   symbols and H Half symbols The values of S and H are determined from
   K as described below.  Then L = K+S+H.

------------------------------------------------------------------------   

      X be the smallest positive integer such that X*(X-1) >= 2*K.

      S be the smallest prime integer such that S >= ceil(0.01*K) + X

      H be the smallest integer such that choose(H,ceil(H/2)) >= K + S

      H' = ceil(H/2)

      L = K+S+H

      C[0],...,C[K-1] denote the first K intermediate symbols

      C[K],...,C[K+S-1] denote the S LDPC symbols, initialised to zero

      C[K+S],...,C[L-1] denote the H Half symbols, initialised to zero


*/


struct Inter_Symbol
{
	/* data */
	//X be the smallest positive integer such that X*(X-1) >= 2*K.
	uint32_t X;
	//S be the smallest prime integer such that S >= ceil(0.01*K) + X
	uint32_t S;
	//H be the smallest integer such that choose(H,ceil(H/2)) >= K + S
	uint32_t H;

	uint8_t *src;

	uint8_t *dst;

	uint32_t sym_len;

	// the number of source symbols
	uint32_t K;

};


class Inter_Symbol_Generator
{
public:
	Inter_Symbol_Generator(class Array_Data_Symbol *p);
	Inter_Symbol_Generator(void);

	~Inter_Symbol_Generator(void);


	int LDPC_Symbol_Generator(class Array_Data_Symbol *p);
	int LDPC_Matrix_Generator(class Array_Data_Symbol *p);
	void LDPC_Matrix_print(void);

	//int HALF_Symbol_Generator(class Array_Data_Symbol *p);
	int HALF_Matrix_Generator(class Array_Data_Symbol *p);

	void HALF_Matrix_print(void);






	int I_S_Matrix_Generator(class Array_Data_Symbol *p);
	void I_S_Matrix_print(void);


	int I_H_Matrix_Generator(class Array_Data_Symbol *p);
	void I_H_Matrix_print(void);


	int Zero_SxH_Matrix_Generator(class Array_Data_Symbol *p);
	void Zero_SxH_Matrix_print(void);

	int G_LT_Matrix_Generator(class Array_Data_Symbol *p);
	int G_LT_Matrix_Generator(class Array_Data_Symbol *p, uint32_t M, std::vector<uint32_t> _ESIs);

	void G_LT_Matrix_print(void);


	int Matrix_A_Generator(class Array_Data_Symbol *p);

	int Matrix_A_Generator(class Array_Data_Symbol *p, uint32_t M, std::vector<uint32_t> _ESIs);

	void Matrix_A_print(void);
	void Matrix_A_print(uint32_t M);



	/* data */
	uint8_t **G_LDPC;
	uint8_t **G_HALF;
	uint8_t **I_S;
	uint8_t **I_H;
	uint8_t **Zero_SxH;
	uint8_t **G_LT;

	std::vector<uint32_t> ESIs;

	class Array_Data_Symbol *symbols_p;

    boost::numeric::ublas::matrix<uint8_t> A;


private:
	//class Array_Data_Symbol tmp;
	uint8_t** identity_matrix_generate(uint32_t row, uint32_t col);

	void byte_bits_print(uint8_t v);
	void gray_bits_print(uint32_t v);
	uint32_t non_zero_bits_count(uint32_t v);
	uint32_t gray_bits_generate(uint32_t i);
	bool choose_gray_bit(uint32_t num, uint32_t g);
	void matrix_print(uint8_t **matrix, uint32_t row, uint32_t col);
	uint8_t ** new_matrix(uint32_t row, uint32_t col);
	void matrix_copy_to(uint8_t **src, uint32_t row, uint32_t col, uint8_t **dst, uint32_t r_pos, uint32_t c_pos);

	class Triple_Generator triple;

	uint8_t **_A;





};








#endif
