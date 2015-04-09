
#include "R10_Decoder.h"


Row_Edge::Row_Edge(void)
{

}


Row_Edge::~Row_Edge(void)
{

}

R10_Decoder::R10_Decoder(void)
{

	A_i = 0;
	A_u = 0;
	comb_list_generate();


}

uint32_t R10_Decoder::nonZeros_In_Row(ublas::vector<uint8_t> Row, uint32_t fromCol, uint32_t toCol)
{
	uint32_t r = 0;
	for (int i = fromCol; i <= toCol; i++)
	{
		if (Row(i) == 1)
		{
			r++;
		}
	}

	return r;
}

std::vector<uint32_t>  R10_Decoder::find_nonZeros_pos_In_Row(ublas::vector<uint8_t> Row, uint32_t fromCol, uint32_t toCol)
{
	std::vector<uint32_t>  r;
	for (int i = fromCol; i <= toCol ; i++)
	{
		if (Row(i) == 1)
			{
				r.push_back(i);
			}	
	}

	return r;

}



R10_Decoder::~R10_Decoder(void)
{

}

R10_Decoder::R10_Decoder(uint32_t K, uint32_t symbol_len)
{
	sym_p = new Array_Data_Symbol(K, symbol_len);
	A_i = 0;
	A_u = 0;
	comb_list_generate();

}


/*

Let N >= K be the number of received encoding symbols for a source
   block and let M = S+H+N.  The following M by L bit matrix A can be
   derived from the information passed to the decoder for the source
   block to be decoded.  Let C be the column vector of the L
   intermediate symbols, and let D be the column vector of M symbols
   with values known to the receiver, where the first S+H of the M
   symbols are zero-valued symbols that correspond to LDPC and Half
   symbols (these are check symbols for the LDPC and Half symbols, and
   not the LDPC and Half symbols themselves), and the remaining N of the
   M symbols are the received encoding symbols for the source block.
   Then, A is the bit matrix that satisfies A*C = D, where here *
   denotes matrix multiplication over GF[2].  In particular, A[i,j] = 1
   if the intermediate symbol corresponding to index j is exclusive-ORed
   into the LDPC, Half, or encoding symbol corresponding to index i in
   the encoding, or if index i corresponds to a LDPC or Half symbol and
   index j corresponds to the same LDPC or Half symbol.  For all other i
   and j, A[i,j] = 0.

*/

class Array_Data_Symbol R10_Decoder::Get_Inter_Symbols(class Array_Data_Symbol _D, uint32_t N)
{	

	class Array_Data_Symbol result;


	D = _D;
	class Array_Data_Symbol __D = _D;




	uint32_t overhead = N - D.K;

	uint32_t M =  N + __D.S + __D.H;

	// if (M != D.L)
	// {
	// 	std::cout << "M is not equal to D.L: " << D.symbol.size() << std::endl;	
	// 	exit(-1);
	// }


	// for (int i = 0; i < D.symbol.size(); ++i)
	// {
	// std::cout << "size of D symbol: " << D.symbol[i].s.size() << std::endl;
	// }

	// for (int i = 0; i < D.symbol.size(); ++i)
	// {
	// 	printf("symbol size: %d  s size: %d\n", __D.symbol.size(), __D.symbol[i].s.size());
	// }
	// printf("sym_len : %d\n", _D.sym_len);


	D.array_allocation(M, __D.sym_len);





	// std::cout << "size of __D symbol: " << __D.symbol.size() << std::endl;	
	// std::cout << "size of D symbol: " << D.symbol.size() << std::endl;	

	// std::cout << "M : " << M << std::endl;	


	


	int count = 0;
	class Array_Data_Symbol tmp;
	tmp.array_allocation(M, __D.sym_len);
	for (int i = M - N; i < M; ++i)
	{
		tmp.symbol[i] = __D.symbol[count];
		count++;
	}



	//int count = 0;
	for (int i = 0; i < M; ++i)
	{
		D.symbol[i] = tmp.symbol[i];
		//count++;
	}

	// printf("Inter symbol: \n");
	// for (int i = 0; i < __D.symbol.size(); ++i)
	// {
	// 	printf("%d ", __D.symbol[i].s[0]);
	// }
	// printf("\n");

	// printf("Inter symbol D:  \n");
	// for (int i = 0; i < D.symbol.size(); ++i)
	// {
	// 	printf("%d ", D.symbol[i].s[0]);
	// }
	// printf("\n");

//	exit(1);



	if (M < sym_p->K)
	{
		std::cout << "M can not be less than L" << std::endl;
		exit(-1);		
	}	

	InSym = new Inter_Symbol_Generator(&__D);
	InSym->Matrix_A_Generator(&__D, M, __D.ESIs);


	//InSym->Matrix_A_print(M);



	A = InSym->A;



	// change the order.
	class Array_Data_Symbol temp = InactivationDecoding(A, D, M);

	result = temp;


//	std::cout << "c order: " << std::endl;
	for (int i = 0; i < result.L; ++i)
	{
//		printf("%d ", c[i]);
		result.symbol[c[i]] = temp.symbol[d[i]];
	}
//	std::cout << std::endl;

	// std::cout << "d order: " << std::endl;
	// for (int i = 0; i < M; ++i)
	// {
	// 	printf("%d ", d[i]);
	// }
	// std::cout << std::endl;

	for (int i = 0; i < M - result.L; ++i)
	{
		result.symbol.erase(result.symbol.begin() + result.L + i);
	}




	return result;

}



class Array_Data_Symbol R10_Decoder::InactivationDecoding(matrix<uint8_t> &A, class Array_Data_Symbol &D, uint32_t &M)
{
	//class Array_Data_Symbol _D = D;
	//uint32_t _M = M;
	class Array_Data_Symbol result;


	overhead = M - sym_p->L;
	N = sym_p->K + overhead;
	//std::cout << "M : " << M << "  overhead: " << overhead << "  N: " << N << std::endl;



	result = Indecoding_phase_1(A, D, M);



	return result;
}


/*
The first phase of the Gaussian elimination, the matrix A, is conceptually partitioned into submatrices. The submatrix sizes are 
parameterized by non-negative integers i and u, which are initialized to 0.

*/

class Array_Data_Symbol R10_Decoder::Indecoding_phase_1(matrix<uint8_t> &A, class Array_Data_Symbol &D, uint32_t &M)
{

	uint32_t r = 0;


	class Array_Data_Symbol result;

	//printf("D.L : %d\n", D.L);

	c.resize(D.L);
	d.resize(M);

	for (int i = 0; i < D.L; i++)
	{
		c[i] = i;
		d[i] = i;
	}

	for (int i = D.L; i < M; i++)
	{
		d[i] = i;
	}

	A_i = 0;
	A_u = 0;




	//std::cout << "nonZeros: " << std::endl;
	for (int i = 0; i < M; i++)
	{
		class Row_Edge tmp;
		tmp.nonZeros = nonZeros_In_Row(row(A, i), 0, D.L - 1);
		tmp.originalDegree = tmp.nonZeros;
		tmp.position = i;		
		rows[i] = tmp;

		//printf("%d ", rows[i].nonZeros);
	}
//	std::cout << std::endl;



    // counts how many rows have been chosen already
    //uint32_t chosenRowsCounter = 0;

/*
	Let r be the minimum integer such that at least one row of A has exactly r ones in V.

      *  If r != 2, then choose a row with exactly r ones in V with
         minimum original degree among all such rows.

      *  If r = 2, then choose any row with exactly 2 ones in V that is
         part of a maximum size component in the graph defined by V.
*/

    while((A_i + A_u) !=  D.L)
    {

    	class Row_Edge chosenRow;
		bool allZeros = true;
		uint32_t minDegree = 1048576 + 1;

    	r = D.L + 1;




    	for (int i = A_i; i < rows.size() ; i++)
    	{
    		if (rows[i].nonZeros != 0)
    		{
    			allZeros = false;
    		}


    		if (rows[i].nonZeros == 2)
    		{
    			chosenRow = rows[i];
    			r = chosenRow.nonZeros;
    			minDegree = chosenRow.originalDegree;

    		//	printf("chosenRow pos: %d nonZeros: %d\n", chosenRow.position, chosenRow.nonZeros);

    			break;
    		}


    		if ((rows[i].nonZeros < r) && (rows[i].nonZeros > 0) )
    		{
    			chosenRow = rows[i];
    			r = chosenRow.nonZeros;
    			minDegree = chosenRow.originalDegree;
    			    	//		printf("chosenRow pos: %d nonZeros: %d\n", chosenRow.position, chosenRow.nonZeros);

    		}
    		else if ((rows[i].nonZeros == r) && rows[i].originalDegree < minDegree)
    		{
    			chosenRow = rows[i];
    			minDegree = chosenRow.originalDegree;
    			    //			printf("chosenRow pos: %d nonZeros: %d\n", chosenRow.position, chosenRow.nonZeros);

    		}
    	}

    	// there need to recode. I just do it for the testing version!!
    	if (allZeros == true)
    	{
    		std::cout << "Decoding Failure - PI Decoding  Phase 1: All entries in V are zero." << std::endl;
    		exit(-1);
    	}

//    	printf("chosenRow pos: %d nonZeros: %d\n", chosenRow.position, chosenRow.nonZeros);

/*
	After the row is chosen in this step the first row of A that intersects V is exchanged with the chosen row so that the chosen row is the first row that intersects V.  

*/

		if (chosenRow.position != A_i)
			{

				//swap rows of A
				row(A, A_i).swap(row(A, chosenRow.position));

				//swap symbol in d according to A operation
				std::swap(d[A_i], d[chosenRow.position]);

				// update rows map
				// printf(" size of rows : %d pos: %d deg: %d\n", rows.size(), rows[A_i].position, rows[A_i].originalDegree);
				std::swap(rows[A_i], rows[chosenRow.position]);

				rows[chosenRow.position].position = chosenRow.position;

				rows[A_i].position = A_i;

				// printf(" size of rows : %d  pos: %d deg: %d\n", rows.size(), rows[chosenRow.position].position, rows[chosenRow.position].originalDegree);
	
			}	

	//	matrix_print(A);	



	/*
 	The columns of A among those that intersect V are reordered so that one of the r ones in the chosen row appears in the first column of V and so that the remaining r-1 ones appear in the last columns of V.
	*/
 		//caculate the  non-zeros bits position in A_i row of A
 		std::vector<uint32_t> nonZeros_pos = find_nonZeros_pos_In_Row(ublas::row(A, A_i), A_i, D.L - 1 - A_u);

 		// std::cout << "nonZeros position: " << std::endl;
 		// for (int i = 0; i < nonZeros_pos.size(); i++)
 		// {
 		// 	printf("%d ", nonZeros_pos[i]);
 		// }

 	//	std::cout << std::endl << std::endl;

 		uint32_t _1st_nonZeros_pos = nonZeros_pos[0];

 		if (A_i != _1st_nonZeros_pos)
 		{
 			ublas::column(A, A_i).swap(ublas::column(A, _1st_nonZeros_pos));

 			std::swap(c[A_i], c[_1st_nonZeros_pos]);
	 	
	 	}

	 // 	std::cout << "find_nonZeros_pos_In_Row A swap: " << std::endl;
		
		// for (int i = 0; i < c.size(); ++i)
		// {
		// 	printf("%d ", c[i]);
		// }

		// std::cout << std::endl;

		// matrix_print(A);	


		// swap the remaining non-zeros columns so that they are r-1 ones appear in the last columns of V
		uint32_t last_col = D.L - A_u - 1;
		for (int i = (nonZeros_pos.size() - 1); i > 0 ; i--)
		{
			// we need to know if the current column is the last column of V.
			uint32_t current_col = nonZeros_pos[i];
			if (current_col != last_col)
			{
				//if no, swap the current columns
				ublas::column(A, current_col).swap(ublas::column(A, last_col));

				std::swap(c[current_col], c[last_col]);

			}
			// move the last column of V
			last_col--;
		}
	 // 	std::cout << "remaining non-zeros of V swap: " << std::endl;
		
		// for (int i = 0; i < c.size(); ++i)
		// {
		// 	printf("%d ", c[i]);
		// }

		// std::cout << std::endl;

		// matrix_print(A);	





	/*
	Then, the chosen row is exclusive-ORed into all the other rows of A below the chosen row that have a one in the first column of V. 
	*/

		for (int _row = A_i + 1; _row < M; _row++)
		{
			//check if the A_ith column if '0'
			if (A(_row, A_i) == 0)
			{
				continue;
			}

			// using chosen rows XOR with remaining in A
			ublas::row(A, _row) = util.matrix_row_XOR(ublas::row(A, A_i), ublas::row(A, _row));
			// decoding process XOR D[d[row]] and D[d[i]]
	//		printf("symbol size: %d  row: %d  A_i: %d \n",D.symbol.size(), _row, A_i);


				D.symbol[d[_row]] = D.symbol[d[_row]] ^ D.symbol[d[A_i]];

			// for (int i = 0; i < D.sym_len; ++i)
			// {
			// 	D.symbol[_row].s[i] = D.symbol[_row].s[i] ^ D.symbol[A_i].s[i];
			// }

		}

	 //  	std::cout << "A XOR :  A_u: " <<  A_u << " A_i: "<<  A_i << std::endl;

		// matrix_print(A);	

	/*
	Finally, i is incremented by 1 and u is incremented by r-1, which completes the step.
	*/

    	A_i++;
 	   	A_u += r - 1;

	//  	std::cout << "A_u: " << A_u << " A_i: "<<  A_i << std::endl; 	   	

 	   	// update non-zeros
 	   	// std::cout << "position: " << std::endl;
 	   	// for (int i = 0; i < rows.size(); ++i)
 	   	// {
 	   	// 	printf("%d ", rows[i].position);
 	   	// }
 	   	// std::cout << std::endl;


	//	std::cout << "updated nonZeros: " << std::endl;
		for (int i = A_i; i < rows.size(); i++)
		{
			//uint32_t nzpos = rows[i].position;
			rows[i].nonZeros = nonZeros_In_Row(row(A, i), A_i, D.L - A_u - 1);
			//printf("%d:%d ",rows[i].position, rows[i].nonZeros);
		}
		//std::cout << std::endl;


    }



	result = Indecoding_phase_2(A, D, M);

    return result;

}


class Array_Data_Symbol R10_Decoder::Indecoding_phase_2(matrix<uint8_t> &A, class Array_Data_Symbol &D, uint32_t &M)
{
	/*
	The submatrix U is further partitioned into the first i rows,   U_upper, and the remaining M - i rows, U_lower.  Gaussian elimination is performed in the second phase on U_lower to either determine that its rank is less than u (decoding failure) or to convert it into a matrix where the first u rows is the identity matrix (success of the second phase).  Call this u by u identity matrix I_u.  The M - L rows of A that intersect U_lower - I_u are discarded.  After this phase, A has L rows and L columns.
	*/

	class Array_Data_Symbol result;

	gaussian_elimination(A, D, A_i, M - 1, D.L - A_u, D.L - 1, d);


	// std::cout << "phase 2 Gaussian Elimination: " << std::endl;

	// matrix_print(A);	

	// std::cout << std::endl;

    // check U_lower's rank, if it's less than 'u' we've got a decoding failure

	int rank = caculate_rank_in_matrix(A, A_i, M - 1, A_i, D.L - 1);

//	std::cout << "rank of A after Indecoding_phase_2: " << rank << std::endl;
	// need to rewite here!!!
	if (rank < A_u)
	{
		std::cout << "Decoding Failure - PI Decoding @ Phase 2: U_lower's rank is less than u." << std::endl;
		exit(-1);
	}

	result = Indecoding_phase_3(A, D, M);

	return result;
}


class Array_Data_Symbol R10_Decoder::Indecoding_phase_3(matrix<uint8_t> &A, class Array_Data_Symbol &D,  uint32_t &M)
{

	/*
	To zero out U_upper efficiently, the following precomputation matrix U' is computed based on I_u in the third phase and then U' is used in the fourth phase to zero out U_upper.  The u rows of Iu are partitioned into ceil(u/8) groups of 8 rows each.  Then, for each group of 8 rows, all non-zero combinations of the 8 rows are computed, resulting in 2^^8 - 1 = 255 rows (this can be done with 2^^8-8-1 = 247 exclusive-ors of rows per group, since the combinations of Hamming weight one that appear in I_u do not need to be recomputed).  Thus,the resulting precomputation matrix U' has ceil(u/8)*255 rows and u columns.  Note that U' is not formally a part of matrix A, but will be used in the fourth phase to zero out U_upper.

	*/
	class Array_Data_Symbol result;

	uint32_t A_up = std::ceil(A_u / 8.0);

	//std::cout << "A_up : " << A_up * 255 << std::endl;

	ublas::matrix<uint8_t> U_p(A_u, A_u);

	U_p =  ublas::project(A, range(A_i, D.L), range(A_i, D.L));

	// for (int i = 0; i < U_p.size1(); i++)
	// {
	// 	printf("%2d: ", i);
	// 	for (int j = 0; j < U_p.size2(); j++)
	// 	{
	// 		printf("%d ",  U_p(i, j));  
	// 	}
	// 	printf("\n");
	// }

	// printf("\n");


	U_p =  ublas::project(A, range(0, A_i), range(A_i, D.L));

	// for (int i = 0; i < U_p.size1(); i++)
	// {
	// 	printf("%2d: ", i);
	// 	for (int j = 0; j < U_p.size2(); j++)
	// 	{
	// 		printf("%d ",  U_p(i, j));  
	// 	}
	// 	printf("\n");
	// }

	precomput_param = precomputation_matrix_generate(A_up * 255, D.L - A_i, A, D);

	result = Indecoding_phase_4(A, D, M);


	return result;
}


class Array_Data_Symbol R10_Decoder::Indecoding_phase_4(matrix<uint8_t> &A, class Array_Data_Symbol &D,  uint32_t &M)
{

	/*
	For each of the first i rows of A, for each group of 8 columns in the
   U_upper submatrix of this row, if the set of 8 column entries in
   U_upper are not all zero, then the row of the precomputation matrix
   U' that matches the pattern in the 8 columns is exclusive-ORed into
   the row, thus zeroing out those 8 columns in the row at the cost of
   exclusive-ORing one row of U' into the row.
	
	*/

	class Array_Data_Symbol result;

	uint32_t groups_num = std::ceil(A_u / 8.0);

	uint32_t nonZeros = 0;

	ublas::matrix<uint8_t> U_upper =  ublas::project(A, range(0, A_i), range(A_i, D.L));

	std::vector<uint32_t> nonZeros_pos;
	nonZeros_pos.clear();

	ublas::vector<uint8_t> group_col(8);



	// for (int i = 0; i < U_upper.size1(); i++)
	// {
	// 	printf("%2d: ", i);
	// 	for (int j = 0; j < U_upper.size2(); j++)
	// 	{
	// 		printf("%d ",  U_upper(i, j));  
	// 	}
	// 	printf("\n");
	// }

	//groups_num = 1;
	uint32_t _row = 0;
	for (int i = 0; i < groups_num - 1; i++)
	{


		_row = i * 255;
		for (int j = 0; j < U_upper.size1(); j++)
		{

			group_col = ublas::project(ublas::row(U_upper, j), range(8 * i, 8 * i + 8));

			// for (int k = 0; k < group_col.size(); k++)
			// {
			// 	printf(" %d ", group_col(k));
			// }
			// std::cout << std::endl;
	

			nonZeros = nonZeros_In_Row(group_col, 0, group_col.size() - 1);
		//	std::cout << "U_upper nonZeros: " << nonZeros << std::endl;

			if (nonZeros == 0)
			{
				continue;
			}


		//	std::cout << "find_nonZeros_pos_In_Row: " << std::endl;

			nonZeros_pos = find_nonZeros_pos_In_Row(group_col, 0, group_col.size() - 1);

			// for (int k = 0; k < nonZeros_pos.size(); k++)
			// {
			// 	std::cout << "  " << nonZeros_pos[k];
			// }
			// std::cout << std::endl;


		//	std::cout << "match_pos: " << std::endl;
			std::vector<uint32_t> match_pos;
			//	printf(" %d \n", _row);
			uint32_t orig_row = _row;
			while(_row < 255 * (i + 1))
			{
				group_col = ublas::project(ublas::row(precomput_param.U_p, _row), range(8 * i, 8 * i + 8));

//				match_pos = find_nonZeros_pos_In_Row (ublas::row(precomput_param.U_p, _row), 8 * i, 8 * i + 8 - 1);
				match_pos = find_nonZeros_pos_In_Row (group_col, 0, 8 - 1);
				


				// for (int k = 0; k < match_pos.size(); k++)
				// {
				// 	printf(" %d ", match_pos[k]);
				// }
				//std::cout << std::endl;

				if (match_pos == nonZeros_pos)
				{
			//		std::cout << "match_pos == nonZeros_pos" << std::endl;
					// for (int k = 0; k < nonZeros_pos.size(); k++)
					// {
					// 	std::cout << "  " << nonZeros_pos[k];
					// }
					// std::cout << std::endl;

					// for (int k = 0; k < match_pos.size(); k++)
					// {
					// 	printf(" %d ", match_pos[k]);
					// }
					// std::cout << std::endl;


					ublas::vector<uint8_t> for_xor = ublas::row(A, j);
					 ublas::subrange(for_xor, A_i, D.L) = util.matrix_row_XOR(ublas::subrange(for_xor, A_i, D.L), ublas::row(precomput_param.U_p, _row));
					 ublas::row(A, j) = for_xor;
					//exit(0);
					 D.symbol[d[j]] = D.symbol[d[j]] ^ precomput_param.D_p.symbol[_row];

				}
				// the _row can increase after matching.
				_row++;

			}
			_row = orig_row;


		//	std::cout << std::endl;
		}
		
	}

				


	// last group handle
	//std::cout << "last group handle: " << std::endl;


	_row = (groups_num - 1) * 255;
	for (int j = 0; j < U_upper.size1(); j++)
	{

		if (A_u % 8 == 0)
		{
			group_col = ublas::project(ublas::row(U_upper, j), range(8 * (groups_num - 1), 8 * (groups_num - 1) + 8));
		}
		else
		{
			group_col = ublas::project(ublas::row(U_upper, j), range(8 * (groups_num - 1), 8 * (groups_num - 1) + A_u % 8));
		}
		

		// for (int k = 0; k < group_col.size(); k++)
		// {
		// 	printf(" %d ", group_col(k));
		// }
		// std::cout << std::endl;
	



		nonZeros = nonZeros_In_Row(group_col, 0, group_col.size() - 1);
	//	std::cout << "U_upper nonZeros: " << nonZeros << std::endl;

		if (nonZeros == 0)
		{
			continue;
		}

			

	//	std::cout << "find_nonZeros_pos_In_Row: " << std::endl;

		nonZeros_pos = find_nonZeros_pos_In_Row(group_col, 0, group_col.size() - 1);

		// for (int k = 0; k < nonZeros_pos.size(); k++)
		// {
		// 	std::cout << " " << nonZeros_pos[k];
		// }
		// std::cout << std::endl;



	//	std::cout << "match_pos: " << std::endl;
		std::vector<uint32_t> match_pos;
		//	printf(" %d \n", _row);
		uint32_t orig_row = _row;


		uint32_t last_group_condition = 255 * (groups_num - 1) + (1 << A_u % 8) - 1;

		if (A_u % 8 == 0)
		{
			last_group_condition = 255 * (groups_num - 1) + 255;
		}

		uint32_t chosen_col = 0;

	//	std::cout << "last_group_condition: " << last_group_condition << std::endl;

		//while(_row < 255 * (groups_num - 1) + (1 << A_u % 8) - 1 )
		while(_row < last_group_condition)
		{





			if (A_u % 8 == 0)
			{
				group_col = ublas::project(ublas::row(precomput_param.U_p, _row), range(8 * (groups_num - 1), 8 *(groups_num - 1) + 8));
			}
			else
			{
				group_col = ublas::project(ublas::row(precomput_param.U_p, _row), range(8 * (groups_num - 1), 8 *(groups_num - 1) + A_u % 8));

			}


			//match_pos = find_nonZeros_pos_In_Row (ublas::row(precomput_param.U_p, _row), 8 * (groups_num - 1), 8 * (groups_num - 1) + A_u % 8 - 1);

			if (A_u % 8 == 0)
			{
				match_pos = find_nonZeros_pos_In_Row (group_col, 0, 8 - 1);

			}
			else
			{
				match_pos = find_nonZeros_pos_In_Row (group_col, 0, A_u % 8 - 1);

			}


			// for (int k = 0; k < group_col.size(); k++)
			// {
			// 	printf(" %d ", group_col(k));
			// }
			// std::cout << std::endl;
	

				// for (int k = 0; k < precomput_param.U_p.size2(); k++)
				// {
				// 	printf(" %d ", precomput_param.U_p(_row,k));
					
				// }

				//std::cout << A_u % 8 - 1 << std::endl;


			// for (int k = 0; k < match_pos.size(); k++)
			// {
			// 	printf(" %d ", match_pos[k]);
			// }
			// std::cout << std::endl;




	//		std::cout << "_row: "<< _row << std::endl;

			if (match_pos == nonZeros_pos)
			{

				// std::cout << "match_pos == nonZeros_pos" << std::endl;


				// for (int k = 0; k < nonZeros_pos.size(); k++)
				// {
				// 	std::cout << "  " << nonZeros_pos[k];
				// }
				// std::cout << std::endl;

				// for (int k = 0; k < match_pos.size(); k++)
				// {
				// 	printf(" %d ", match_pos[k]);
				// }
				// std::cout << std::endl;


				ublas::vector<uint8_t> for_xor = ublas::row(A, j);
				 ublas::subrange(for_xor, A_i, D.L) = util.matrix_row_XOR(ublas::subrange(for_xor, A_i, D.L), ublas::row(precomput_param.U_p, _row));
					 ublas::row(A, j) = for_xor;
				
				D.symbol[d[j]] = D.symbol[d[j]] ^ precomput_param.D_p.symbol[_row];

				

			}
			_row++;


		}
		_row = orig_row;


//		std::cout << std::endl;
	}

	// for (int i = 0; i < precomput_param.U_p.size1(); i++)
	// {
	// 	printf("%2d: ", i);
	// 	for (int j = 0; j < precomput_param.U_p.size2(); j++)
	// 	{
	// 		printf("%d ",  precomput_param.U_p(i, j));  
	// 	}
	// 	printf("\n");
	// }	


	// for (int i = 0; i < A.size1(); i++)
	// {
	// 	printf("%2d: ", i);
	// 	for (int j = 0; j < A.size2(); j++)
	// 	{
	// 		printf("%d ",  A(i, j));  
	// 	}
	// 	printf("\n");
	// }	


	result = D;


	return result;
}




struct precompute_matrix R10_Decoder::precomputation_matrix_generate(uint32_t row, uint32_t col, matrix<uint8_t> A, class Array_Data_Symbol D)
{
	ublas::matrix<uint8_t> U = ublas::project(A, range(A_i, D.L), range(A_i, D.L));

	uint32_t groups_num = std::ceil(A_u / 8.0);


	// if ((A_u % 8) != 0 )
	// {

	// 	U.resize(A_u + 8 - (A_u % 8), col);
	// 	for (int i = A_u; i < A_u + 8 - (A_u % 8); i++)
	// 	{
	// 		for (int j = 0; j < col; j++)
	// 		{
	// 			U(i,j) = 0;
	// 		}
	// 	}
	// }


	class Array_Data_Symbol D_p;
	class Array_Data_Symbol D_u;

	D_p.array_allocation(row, D.sym_len);


	D_u.array_allocation(A_u, D.sym_len);


	//std::cout << "D_p.symbol "  << " : " << D_p.symbol.size() << std::endl;




	for (int i = 0; i < A_u; i++)
	{
		D_u.symbol[i] = D.symbol[d[A_i + i]];
		//printf("%d\n", D_p.symbol[i].s[0]);
	}




	struct precompute_matrix m;

	ublas::matrix<uint8_t> U_p(row, col);

	U_p.clear();





	ublas::matrix<uint8_t> group_m(8, col);

	class Array_Data_Symbol group_D;
	group_D.array_allocation(8, D.sym_len);

  //  std::cout << "precompute_matrix: " << std::endl;



    std::vector< std::set<int> > list;
    // init the _row = 0
    uint32_t _row = 0;


    // finish the above groups_num firstly
	for (int k = 0; k < groups_num - 1; k++)
	{

		group_m = ublas::project(U, range(8 * k, 8 * (k + 1)), range(0, col));
		

		group_D.symbol.assign(D_u.symbol.begin() + (8 * k), D_u.symbol.begin() + (8 * (k + 1)) );


	//	std::cout << "groups_num : " << k << std::endl;

	    for (int i = 0; i < comb_list.size(); i++)
	    {
	    	list = comb_list[i];
		    for (int j = 0; j < list.size(); j++)
		    {
		    	for (std::set<int>::iterator it = list[j].begin(); it != list[j].end(); it++)
		    	{
		    		//std::cout << " " << *it;
		    		ublas::row(U_p, _row) = util.matrix_row_XOR(ublas::row (group_m, *it), ublas::row(U_p, _row));



		    		D_p.symbol[_row] = D_p.symbol[_row] ^ group_D.symbol[*it];
		    		// std::cout << "group_D.symbol " << *it << " : " << group_D.symbol.size() << std::endl;
		    		// std::cout << "D_p.symbol " << _row << " : " << D_p.symbol.size() << std::endl;
		    	}
		    	//std::cout << std::endl;
		    	_row++;
		    }

	    }
		    	
	}

	// the last group generate
    std::vector< std::vector< std::set<int> > > last_list;

	for (int i = 1; i <= (A_u % 8); i++)
	{
		last_list.push_back(comb((A_u % 8), i));
	}


	// check if A_u % 8 == 0, then combination is 8.

	if (A_u % 8 == 0)
	{
		last_list = comb_list;

	}



	group_m = ublas::project(U, range(8 * (groups_num - 1), A_u), range(0, col));
	

	group_D.symbol.assign(D_u.symbol.begin() + (8 * (groups_num - 1)), D_u.symbol.begin() + A_u);

		

	for (int i = 0; i < last_list.size(); i++)
	{
	    list = last_list[i];
		for (int j = 0; j < list.size(); j++)
		{
	    	for (std::set<int>::iterator it = list[j].begin(); it != list[j].end(); it++)
		    {
	//	   		std::cout << " " << *it;
		   		ublas::row(U_p, _row) = util.matrix_row_XOR(ublas::row (group_m, *it), ublas::row(U_p, _row));

		   		D_p.symbol[_row] = D_p.symbol[_row] ^ group_D.symbol[*it];

		   	}
	//	   	std::cout << std::endl;
	    	_row++;
	    }

    }






	// for (int i = 0; i < U_p.size1(); i++)
	// {
	// 	printf("%2d: ", i);
	// 	for (int j = 0; j < U_p.size2(); j++)
	// 	{
	// 		printf("%d ",  U_p(i, j));  
	// 	}
	// 	printf("\n");
	// }		


	// exit(-1);

	m.U_p = U_p;
	m.D_p = D_p;


	return m;
}


std::vector< std::set<int> > R10_Decoder::comb(int N, int K)
{
    std::string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's

    std::vector< std::set<int> > list;
    std::set<int> tmp;

    // print integers and permute bitmask
    do {
        for (int i = 0; i < N; ++i) // [0..N-1] integers
        {
            if (bitmask[i]) 
            {
            	//std::cout << " " << i;
            	tmp.insert(i);
            }
        }
      //  std::cout << std::endl;
        list.push_back(tmp);
        tmp.clear();
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

    return list;

}
 

void R10_Decoder::comb_list_generate(void)
{

	for (int i = 1; i <= 8; i++)
	{
		comb_list.push_back(comb(8,i));
	}

//	int c = 0;
 //   std::cout << "comb vector: " << std::endl;

    std::vector< std::set<int> > list;
    for (int j = 0; j < comb_list.size(); j++)
    {
    	list = comb_list[j];
	    for (int i = 0; i < list.size(); i++)
	    {
	    	for (std::set<int>::iterator it = list[i].begin(); it != list[i].end(); it++)
	    	{
	    		//std::cout << " " << *it;
	    	}
	    	//std::cout << std::endl;
	    }

    }
  //  std::cout << "total: " << c << std::endl;
}



int R10_Decoder::caculate_rank_in_matrix(ublas::matrix<uint8_t> A, uint32_t from_row, uint32_t to_row, uint32_t from_col, uint32_t to_col)
{
	int rank = 0;
	for (int i = from_row; i <= to_row; i++)
	{
//		printf("%d\n", i);
		if(nonZeros_In_Row(ublas::row(A, i), from_col, to_col) == 1)
		{
			rank++;
		}
	}

	return rank;	
}



int R10_Decoder::gaussian_elimination(ublas::matrix<uint8_t> &A, class Array_Data_Symbol &D, uint32_t from_row, uint32_t to_row, uint32_t from_col, uint32_t to_col, std::vector<uint32_t> &d)
{
	uint32_t _1st_nonZeros_pos = from_col;
	//int r = 0;
	int i = 0;

	// printf("row: %d to: %d\n",from_row, to_row);
	// printf("col: %d to: %d\n",from_col, to_col);


	for (int r = from_row; r <= to_row; r++)
	{
//		printf("r: %d\n", r);
		if (_1st_nonZeros_pos > to_col)
		{
			return 0;
		}

		i = r;
		while(A(i, _1st_nonZeros_pos) == 0)
		{
			if (i == to_row)
			{
				i = r;
				_1st_nonZeros_pos++;
				if (_1st_nonZeros_pos > to_col)
				{
					return 0;
				}
			}
			i++;

		}

		//if (r == 21)
		
//			std::cout << "_1st_nonZeros_pos: " << _1st_nonZeros_pos<<std::endl;
		

		if (i != r)
		{
//			std::cout << "i != r" << std::endl;
//			printf("i: %d r: %d\n", i,r);
//			matrix_print(A);	
//			std::cout << std::endl;
			ublas::row(A, i).swap(ublas::row(A, r));
			std::swap(d[i], d[r]);

		}


		for (int _row = from_row; _row <= to_row; _row++)
		{

//			std::cout << "_row != r" << std::endl;

//			printf("_row: %d r: %d\n", _row,r);

			if (_row != r)
			{

				if (A(_row, _1st_nonZeros_pos) == 0)
				{
					continue;
				}
				ublas::row(A, _row) = util.matrix_row_XOR(ublas::row(A, r),ublas::row(A, _row));
				D.symbol[d[_row]] = D.symbol[d[r]] ^ D.symbol[d[_row]];
			}
	//		matrix_print(A);
	//		std::cout << std::endl;

		}

		_1st_nonZeros_pos++;

	}




	return 0;
}


void R10_Decoder::matrix_print(ublas::matrix<uint8_t> m)
{
	for (int i = 0; i < m.size1(); i++)
	{
		printf("%2d:   ", i); 
		for (int j = 0; j < m.size2(); j++)
		{
			printf("%d  ", m(i,j));		
		}
		printf("\n");		
	}	
}




class Array_Data_Symbol R10_Decoder::Inter_Symbols_Decoding(class Array_Data_Symbol intermediate_symbols)
{

	class Vector_Data_DEF result(intermediate_symbols.sym_len);

	struct Triple T;
    class Triple_Generator triple;

    class Array_Data_Symbol source_symbol(intermediate_symbols.K, intermediate_symbols.sym_len);



    for (int i = 0; i < intermediate_symbols.K; ++i)
    {
		T = triple.triple_generator(intermediate_symbols.K, i, intermediate_symbols.LP);

		while(T.b >= intermediate_symbols.L)
		{
			T.b = (T.b + T.a) % intermediate_symbols.LP;
		}
		
		result = intermediate_symbols.symbol[T.b];

		for (int j = 1; j <= std::min(T.d -1, intermediate_symbols.L - 1); j++)
		{
			T.b = (T.b + T.a) % intermediate_symbols.LP;
			while(T.b >= intermediate_symbols.L)
			{
				T.b = (T.b + T.a) % intermediate_symbols.LP;
			}
			result = result ^ intermediate_symbols.symbol[T.b];
		}

		source_symbol.symbol[i] = result;
	}


	// printf("source symbol: \n");
	// for (int i = 0; i < source_symbol.K; ++i)
	// {
	// 	printf("%d ", source_symbol.symbol[i].s[0]);
	// }
	// printf("\n");

	return source_symbol;	
}



class Array_Data_Symbol R10_Decoder::Get_Source_Symbol(class Array_Data_Symbol _D, uint32_t N)
{

	class Array_Data_Symbol D = _D;
	class Array_Data_Symbol source_symbol;



	class Array_Data_Symbol intermediate_symbols = Get_Inter_Symbols(D, N);

	source_symbol = Inter_Symbols_Decoding(intermediate_symbols);


	source_symbol.symbol.resize(source_symbol.K);

	return source_symbol;


}








