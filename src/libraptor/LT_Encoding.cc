
#include "LT_Encoding.h"




LT_Encoding::LT_Encoding(void)
{

}


LT_Encoding::LT_Encoding(class Array_Data_Symbol *p)
{
	init(p);
}




LT_Encoding::~LT_Encoding(void)
{

}


void LT_Encoding::init(class Array_Data_Symbol *p)
{
	decoder = new class R10_Decoder(p->K, p->sym_len);

	D = new class Array_Data_Symbol(p->K, p->sym_len);

	class Array_Data_Symbol IS_v = Generate_Intermediate_Symbols(p);
	
	// std::cout << "Intermediate_Symbols D: " << D->symbol.size() << std::endl;


	// std::cout << "Intermediate_Symbols: " << IS_v.symbol.size() << std::endl;
	// for (int i = 0; i < IS_v.symbol.size(); ++i)
	// {
	// 	printf("%d ", IS_v.symbol[i].s[0]);
	// }
	// std::cout << std::endl;

	C = IS_v;


}


class Array_Data_Symbol LT_Encoding::Generate_Intermediate_Symbols(class Array_Data_Symbol *p)
{

	class Array_Data_Symbol result;

	/*
	D denote the column vector consisting of S+H zero symbols followed by the K source symbols C'[0], C'[1], ..., C'[K-1]
	*/

	int count = 0;
	for (int i = 0; i < p->L; i++)
	{
		if (i < p->L - p->K)
		{
			
		}
		else
		{
			D->symbol[i] = p->symbol[count];
			count++;
		}
	}


	// std::cout << "Original_Symbols: " << D->symbol.size() << std::endl;
	// for (int i = 0; i < D->symbol.size(); ++i)
	// {
	// 	printf("%d ", D->symbol[i].s[0]);
	// }
	// std::cout << std::endl;

	//exit(1);


	class Inter_Symbol_Generator InSym(D);



	InSym.Matrix_A_Generator(D);



	//InSym.Matrix_A_print(D->L);



	class Array_Data_Symbol temp = decoder->InactivationDecoding(InSym.A, *D, D->L);



	result = temp;

	//std::cout << "c order: " << std::endl;
	for (int i = 0; i < D->L; ++i)
	{
	//	printf("%d ", decoder->c[i]);
		result.symbol[decoder->c[i]] = temp.symbol[decoder->d[i]];
	}
	//std::cout << std::endl;

	// std::cout << "d order: " << std::endl;
	// for (int i = 0; i < D->L; ++i)
	// {
	// 	printf("%d ", decoder->d[i]);
	// }

	// std::cout << std::endl;

	return result;

}



std::vector<class Vector_Data_DEF> LT_Encoding::LTEnc_Generate(std::vector<uint32_t> ESIs)
{
	struct Triple T;

	std::vector<class Vector_Data_DEF> r;

	if (C.symbol.size() == 0)
	{
		std::cerr << "C is null !" << std::endl;
		exit(-1);
	}



	for (int i = 0; i < ESIs.size(); ++i)
	{
		T = triple.triple_generator(C.K, ESIs[i], C.LP);
		r.push_back( LTEnc(&C, T) );
	}

	// printf("LT_Encoding symbol size: %d\n", r.size());
	// printf("LT_Encoding symbol:\n");
	// for (int i = 0; i < r.size(); ++i)
	// {
	// 	printf(" %d ", r[i].s[0]);
	// }
	// printf("\n");



	return r;
}


class Vector_Data_DEF  LT_Encoding::LTEnc(class Array_Data_Symbol *p, struct Triple triple)
{
	class Vector_Data_DEF result(p->sym_len);

	struct Triple tr = triple;





	while(tr.b >= p->L)
	{
		tr.b = (tr.b + tr.a) % p->LP;
	}

	result = p->symbol[tr.b];


	for (int j = 1; j <= std::min(tr.d -1, p->L - 1); j++)
	{
		tr.b = (tr.b + tr.a) % p->LP;
		while(tr.b >= p->L)
		{
			tr.b = (tr.b + tr.a) % p->LP;
		}
		result = result ^ p->symbol[tr.b];
	}

	return result;
}


