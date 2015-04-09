#ifndef R10_DECODER_H
#define R10_DECODER_H 


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <math.h>
#include <vector>
#include <map>
#include <set>
#include <algorithm>


#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>


#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>


#include <boost/fusion/sequence/comparison/equal_to.hpp>
#include <boost/fusion/include/equal_to.hpp>


// #include <boost/graph/adjacency_list.hpp>
// #include <boost/graph/topological_sort.hpp>
// #include <boost/graph/graph_traits.hpp>
// #include <boost/graph/undirected_graph.hpp>
// #include <boost/graph/adjacency_matrix.hpp>
// #include <boost/graph/graph_utility.hpp>


#include "Array_Data_Types.h"
#include "Triple_Generator.h"
#include "Inter_Symbol_Generator.h"
#include "Triple_Generator.h"

#include "LT_Encoding.h"



using namespace boost::numeric::ublas;

namespace ublas = boost::numeric::ublas;

class Row_Edge
{
public:
    Row_Edge(void);
    ~Row_Edge(void);

    /* data */

    uint32_t position;
    uint32_t nonZeros;
    uint32_t originalDegree;

};

struct precompute_matrix
{
    ublas::matrix<uint8_t> U_p;
    class Array_Data_Symbol D_p;
    
};


class R10_Decoder
{
public:
	R10_Decoder(void);
	R10_Decoder(uint32_t K, uint32_t symbol_len);
	~R10_Decoder(void);

	class Array_Data_Symbol *sym_p;


    class Inter_Symbol_Generator *InSym;
	//N >= K be the number of received encoding symbols for a source block
    uint32_t N;

 	//M = S+H+N   
    uint32_t M;

    uint32_t overhead;

    //Let c[0] = 0, c[1] = 1,...,c[L-1] = L-1 and d[0] =    0, d[1] = 1,...,d[M-1] = M-1 initially.
    std::vector<uint32_t> c;
    std::vector<uint32_t> d;

    // The submatrix sizes are    parameterized by non-negative integers i and u
    uint32_t A_i;
    uint32_t A_u;


    std::vector<uint32_t> ESIs;



    class Array_Data_Symbol Get_Inter_Symbols(class Array_Data_Symbol _D, uint32_t N);

    class Array_Data_Symbol InactivationDecoding(matrix<uint8_t> &A, class Array_Data_Symbol &D, uint32_t &M);

    class Array_Data_Symbol Inter_Symbols_Decoding(class Array_Data_Symbol intermediate_symbols);



    class Array_Data_Symbol Get_Source_Symbol(class Array_Data_Symbol _D, uint32_t N);





private:    

    class Array_Data_Symbol D;
    boost::numeric::ublas::matrix<uint8_t> A;

    std::map<uint32_t, class Row_Edge> rows;


    class Utility util;


    std::vector< std::vector< std::set<int> > > comb_list;

    struct precompute_matrix precomput_param;




    std::vector< std::set<int> >  comb(int N, int K);
    void comb_list_generate(void);



    int gaussian_elimination(ublas::matrix<uint8_t> &A, class Array_Data_Symbol &D, uint32_t from_row, uint32_t to_row, uint32_t from_col, uint32_t to_col, std::vector<uint32_t> &d);


    uint32_t nonZeros_In_Row(ublas::vector<uint8_t> Row, uint32_t fromCol, uint32_t toCol);

    std::vector<uint32_t> find_nonZeros_pos_In_Row(ublas::vector<uint8_t> Row, uint32_t fromCol, uint32_t toCol);

    int caculate_rank_in_matrix(ublas::matrix<uint8_t> A, uint32_t from_row, uint32_t to_row, uint32_t from_col, uint32_t to_col);

    struct precompute_matrix precomputation_matrix_generate(uint32_t row, uint32_t col, matrix<uint8_t> A, class Array_Data_Symbol D);



	class Array_Data_Symbol
 Indecoding_phase_1(matrix<uint8_t> &A, class Array_Data_Symbol &D,  uint32_t &M);
	class Array_Data_Symbol Indecoding_phase_2(matrix<uint8_t> &A, class Array_Data_Symbol &D,  uint32_t &M);
	class Array_Data_Symbol Indecoding_phase_3(matrix<uint8_t> &A, class Array_Data_Symbol &D,  uint32_t &M);
	class Array_Data_Symbol Indecoding_phase_4(matrix<uint8_t> &A, class Array_Data_Symbol &D,  uint32_t &M);

    void matrix_print(ublas::matrix<uint8_t> m);

};





#endif



