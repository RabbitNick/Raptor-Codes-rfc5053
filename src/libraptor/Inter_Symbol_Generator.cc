
#include "Inter_Symbol_Generator.h"





Inter_Symbol_Generator::Inter_Symbol_Generator(void)
{
	//tmp = in;
	_A = NULL;
	G_LDPC = NULL;
	G_HALF = NULL;
	I_S = NULL;
	I_H = NULL;
	Zero_SxH = NULL;
	G_LT = NULL;	
}




Inter_Symbol_Generator::Inter_Symbol_Generator(class Array_Data_Symbol *p)
{
	//tmp = in;
	_A = NULL;
	G_LDPC = NULL;
	G_HALF = NULL;
	I_S = NULL;
	I_H = NULL;
	Zero_SxH = NULL;
	G_LT = NULL;	
	symbols_p = p;
}

Inter_Symbol_Generator::~Inter_Symbol_Generator(void)
{

	//G_LDPC = new uint8_t[8][10];


	// for (int i = 0; i < symbols_p->S; i++)
	// {
	// 	delete[] G_LDPC[i];
	// }
	//delete[] G_LDPC;
}


int Inter_Symbol_Generator::LDPC_Symbol_Generator(class Array_Data_Symbol *p)
{

	uint32_t a = 0;
	uint32_t b = 0;

	for (int i = 0; i < p->K; i++)
	{
		a = 1 + ((uint32_t)floor( (double)(i / p->S)) % (p->S - 1) );
		std::cout << a << std::endl;
		b = i % p->S;
		p->symbol[p->K + b] =  p->symbol[p->K + b] ^ p->symbol[i];
		b = (b + a) % p->S;
		p->symbol[p->K + b] = p->symbol[p->K + b] ^ p->symbol[i];
		b = (b + a) % p->S;
		p->symbol[p->K + b] = p->symbol[p->K + b] ^ p->symbol[i];

	}

	return 0;
}



uint8_t ** Inter_Symbol_Generator::new_matrix(uint32_t row, uint32_t col)
{
	uint8_t **matrix;

	matrix = new uint8_t* [row];

	for (int i = 0; i < row; i++)
	{
		matrix[i] = new uint8_t[col];
	}	

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			matrix[i][j] = 0;
		}
	}

	return matrix;
}

int Inter_Symbol_Generator::LDPC_Matrix_Generator(class Array_Data_Symbol *p)
{

	uint32_t a = 0;
	uint32_t b = 0;

	// G_LDPC = new uint8_t* [p->S]; 

	// for (int i = 0; i < p->S; i++)
	// {
	// 	G_LDPC[i] = new uint8_t[p->K];
	// }


	// for (int i = 0; i < p->S; i++)
	// {
	// 	for (int j = 0; j < p->K; j++)
	// 	{
	// 		G_LDPC[i][j] = 0;
	// 	}
	// }

	G_LDPC = new_matrix(p->S, p->K);


	for (int i = 0; i < p->K; i++)
	{
		a = 1 + ((uint32_t)floor( (double)(i / p->S)) % (p->S - 1));
		b = i % p->S;
		G_LDPC[b][i] = 1;
		b = (b + a) % p->S;
		G_LDPC[b][i] = 1;
		b = (b + a) % p->S;
		G_LDPC[b][i] = 1;		

	}


	return 0;
}


void Inter_Symbol_Generator::LDPC_Matrix_print(void)
{
	std::cout << "LDPC Matrix: " << std::endl;

	matrix_print(G_LDPC, symbols_p->S, symbols_p->K);

}


void Inter_Symbol_Generator::byte_bits_print(uint8_t v)
{
	uint8_t tmp;
	for (int i = 7; i >= 0 ; i--)
	{
		tmp = (v >> i) & 0x01;
		printf("%d", tmp);
	}
}


void Inter_Symbol_Generator::gray_bits_print(uint32_t v)
{
	uint8_t tmp;
	for (int i = 4 - 1; i >= 0; i--)
	{
		tmp = v >> (8 * i);
		byte_bits_print(tmp);
	}
}

uint32_t Inter_Symbol_Generator::non_zero_bits_count(uint32_t v)
{
	uint8_t tmp = 0;
	uint32_t bit_count = 0;;
	for (int i = 0; i < 32; i++)
	{
		tmp = (v >> i) & 0x01;
		if (tmp == 0x01)
		{
			bit_count++;
		}
		
	}
	return bit_count;
}


uint32_t Inter_Symbol_Generator::gray_bits_generate(uint32_t i)
{
	return i ^ (int)(floor ((float) (i / 2)));
}



bool Inter_Symbol_Generator::choose_gray_bit(uint32_t num, uint32_t g)
{
	return (bool)((g >> num) & 0x01);
}



int Inter_Symbol_Generator::HALF_Matrix_Generator(class Array_Data_Symbol *p)
{


	// HALF MATRIX
 

	G_HALF = new_matrix(p->H, (p->K + p->S));


	uint32_t g;
	bool m = 0;
	uint32_t bit_count;
	uint32_t i = 1;

	for (int h = 0; h < p->H; h++)
	{
		for (int j = 0; j < (p->K + p->S); j++)
		{
			while(1)
			{


				g = gray_bits_generate(i);
				bit_count = non_zero_bits_count(g);
			//	std::cout << " 1 bit counts: " << bit_count << std::endl; 
				i++;

				if (bit_count != p->HP)
				{
					continue;
				}

			//	std::cout << "H` : " << p->HP << "  1 bit counts: " << 	bit_count << std::endl; 
			//	gray_bits_print(m);
			//	std::cout << std::endl;

				m = choose_gray_bit(h, g);

			//	std::cout << "m : " << m << std::endl;
				G_HALF[h][j] = m;

				// if (m == true)
				// {
				// 	G_HALF[h][j] = m;
				// }

				break;
			}
		}
		i = 0;
	}

}

void Inter_Symbol_Generator::HALF_Matrix_print(void)
{
	std::cout << "HALF Matrix: " << std::endl;

	matrix_print(G_HALF, symbols_p->H, (symbols_p->K + symbols_p->S));

}



uint8_t** Inter_Symbol_Generator::identity_matrix_generate(uint32_t row, uint32_t col)
{
	uint8_t **matrix;

	if (row != col)
	{
		std::cerr << "row is not equal to col!" << std::endl;
		return NULL;
	}


	matrix = new_matrix(row, col);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (i == j)
			{
				matrix[i][j] = 1;
			}
			
		}
	}

	return matrix;
}

int Inter_Symbol_Generator::I_S_Matrix_Generator(class Array_Data_Symbol *p)
{

	I_S = identity_matrix_generate(p->S, p->S);
	return 0;
}



void Inter_Symbol_Generator::matrix_print(uint8_t **matrix, uint32_t row, uint32_t col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			printf("%d  ", matrix[i][j]);		
		}
		printf("\n");		
	}		
}

void Inter_Symbol_Generator::I_S_Matrix_print(void)
{
	std::cout << "I_S Matrix: " << std::endl;
	matrix_print(I_S, symbols_p->S, symbols_p->S);	
}



int Inter_Symbol_Generator::I_H_Matrix_Generator(class Array_Data_Symbol *p)
{
	I_H = identity_matrix_generate(p->H, p->H);
	return 0;
}

void Inter_Symbol_Generator::I_H_Matrix_print(void)
{
	std::cout << "I_H Matrix: " << std::endl;

	matrix_print(I_H, symbols_p->H, symbols_p->H);
}


int Inter_Symbol_Generator::Zero_SxH_Matrix_Generator(class Array_Data_Symbol *p)
{
	Zero_SxH = new_matrix(p->S, p->H);
	return 0;
}

void Inter_Symbol_Generator::Zero_SxH_Matrix_print(void)
{
	std::cout << "0_SxH Matrix: " << std::endl;
	matrix_print(Zero_SxH, symbols_p->S, symbols_p->H);
}


int Inter_Symbol_Generator::G_LT_Matrix_Generator(class Array_Data_Symbol *p)
{
	G_LT = new_matrix(p->K, p->L);

	for (int i = 0; i < p->K; i++)
	{
		triple._triple = triple.triple_generator(p->K, i, p->LP);
	//	std::cout << "d : " << triple._triple.d << " a: " << triple._triple.a << " b: " << triple._triple.b << std::endl;

		while(triple._triple.b >= p->L)
		{
			triple._triple.b = (triple._triple.b + triple._triple.a) % p->LP;
		}

		G_LT[i][triple._triple.b] = 1;


		for (int j = 1; j <= std::min(triple._triple.d -1, p->L - 1); j++)
		{
			triple._triple.b = (triple._triple.b + triple._triple.a) % p->LP;
			while(triple._triple.b >= p->L)
			{
				triple._triple.b = (triple._triple.b + triple._triple.a) % p->LP;
			}
			//std::cout << "b: " << triple._triple.b << std::endl;
			G_LT[i][triple._triple.b] = 1;
		}
	}


	return 0;
}


int Inter_Symbol_Generator::G_LT_Matrix_Generator(class Array_Data_Symbol *p, uint32_t M, std::vector<uint32_t> _ESIs)
{
	if (G_LT != NULL)
	{
		delete G_LT;
	}
	uint32_t overhead = M - p->L;
	uint32_t N = p->K + overhead;

	ESIs = _ESIs;

//	std::cout << "N: " << N << std::endl;


	G_LT = new_matrix(N, p->L);


	for (int i = 0; i < N; i++)
	{
		triple._triple = triple.triple_generator(p->K, ESIs[i], p->LP);
	//	std::cout << "d : " << triple._triple.d << " a: " << triple._triple.a << " b: " << triple._triple.b << std::endl;
		while(triple._triple.b >= p->L)
		{
			triple._triple.b = (triple._triple.b + triple._triple.a) % p->LP;
		}

		G_LT[i][triple._triple.b] = 1;


		for (int j = 1; j <= std::min(triple._triple.d -1, p->L - 1); j++)
		{
			triple._triple.b = (triple._triple.b + triple._triple.a) % p->LP;
			while(triple._triple.b >= p->L)
			{
				triple._triple.b = (triple._triple.b + triple._triple.a) % p->LP;
			}
			//std::cout << "b: " << triple._triple.b << std::endl;
			G_LT[i][triple._triple.b] = 1;
		}
	}

//	matrix_print(G_LT, N, symbols_p->L);


	return 0;


}



void Inter_Symbol_Generator::G_LT_Matrix_print(void)
{
	std::cout << "G_LT Matrix: " << std::endl;
	matrix_print(G_LT, symbols_p->K, symbols_p->L);
}

int Inter_Symbol_Generator::Matrix_A_Generator(class Array_Data_Symbol *p)
{

	if (_A != NULL)
	{
		delete _A;
	}

	_A = new_matrix(p->L, p->L);


	LDPC_Matrix_Generator(p);
	HALF_Matrix_Generator(p);
	I_S_Matrix_Generator(p);
	I_H_Matrix_Generator(p);
	Zero_SxH_Matrix_Generator(p);
	G_LT_Matrix_Generator(p);


	matrix_copy_to(G_LDPC, p->S, p->K, _A, 0, 0);
	matrix_copy_to(I_S, p->S, p->S, _A, 0, p->K);
	matrix_copy_to(Zero_SxH, p->S, p->H, _A, 0, (p->K + p->S));
	matrix_copy_to(G_HALF, p->H, (p->K + p->S), _A, p->S, 0);
	matrix_copy_to(I_H, p->H, p->H, _A, p->S, (p->K + p->S));
	matrix_copy_to(G_LT, p->K, p->L, _A, (p->S + p->H), 0);

	A.resize(p->L, p->L);
	//matrix_A = boost::numeric::ublas::make_matrix_from_pointer(A);

	for (int i = 0; i < p->L; i++)
	{
		for (int j = 0; j < p->L; j++)
		{
			A(i,j) = _A[i][j];
		}
	}
	return 0;
}



int Inter_Symbol_Generator::Matrix_A_Generator(class Array_Data_Symbol *p, uint32_t M, std::vector<uint32_t> _ESIs)
{

	if (_A != NULL)
	{
		delete _A;
	}

	if (M < p->K)		
	{
		std::cout << "M can not be less than L" << std::endl;
		return -1;
	}

	_A = new_matrix(M, p->L);

	LDPC_Matrix_Generator(p);
	HALF_Matrix_Generator(p);
	I_S_Matrix_Generator(p);
	I_H_Matrix_Generator(p);
	Zero_SxH_Matrix_Generator(p);
	G_LT_Matrix_Generator(p, M, _ESIs);


	matrix_copy_to(G_LDPC, p->S, p->K, _A, 0, 0);
	matrix_copy_to(I_S, p->S, p->S, _A, 0, p->K);
	matrix_copy_to(Zero_SxH, p->S, p->H, _A, 0, (p->K + p->S));
	matrix_copy_to(G_HALF, p->H, (p->K + p->S), _A, p->S, 0);
	matrix_copy_to(I_H, p->H, p->H, _A, p->S, (p->K + p->S));
	matrix_copy_to(G_LT, p->K + (M - p->L), p->L, _A, (p->S + p->H), 0);

	A.resize(M, p->L);
	//matrix_A = boost::numeric::ublas::make_matrix_from_pointer(A);
	A.clear();


	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < p->L; j++)
		{
			A(i,j) = _A[i][j];
		}

	}
	return 0;
}



void Inter_Symbol_Generator::Matrix_A_print(void)
{
	std::cout << "A Matrix: " << std::endl;
	matrix_print(_A, symbols_p->L, symbols_p->L);
}

void Inter_Symbol_Generator::Matrix_A_print(uint32_t M)
{
	std::cout << "A Matrix: " << std::endl;
	matrix_print(_A, M, symbols_p->L);
}


void Inter_Symbol_Generator::matrix_copy_to(uint8_t **src, uint32_t row, uint32_t col, uint8_t **dst, uint32_t r_pos, uint32_t c_pos)
{
	uint32_t i = 0;
	uint32_t j = 0;

	for (i = r_pos; i < r_pos + row; i++)
	{
		for (j = c_pos; j < c_pos + col; j++)
		{
			dst[i][j] = src[i - r_pos][j - c_pos];
			//std::cout << i << " " << j << std::endl;
		}
	}
}