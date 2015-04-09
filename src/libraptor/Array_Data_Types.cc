
#include "Array_Data_Types.h"








// Vector_Data_DEF::Vector_Data_DEF(uint32_t row, uint32_t col)
// {

//  	d_row = row;
//  	d_col = col;

// 	symbol.resize(row);

// 	for (int i = 0; i < symbol.size(); i++)
// 	{
// 		symbol[i].resize(col);
// 	}

// 	std::cout << "row : " << symbol.size() << " col : " << symbol[0].size() << std::endl;

// }


Vector_Data_DEF::Vector_Data_DEF(void)
{
	d_len = 0;
}


Vector_Data_DEF::Vector_Data_DEF(uint32_t len)
{



	s.resize(len);

	d_len = len;



	//std::cout << "length : " << s.size() << std::endl;

}


void Vector_Data_DEF::allocate_size(uint32_t len)
{
	s.resize(len);

	d_len = len;

	for (int i = 0; i < d_len; i++)
	{
		s[i] = 0;
	}



	//std::cout << "length : " << s.size() << std::endl;
}



Vector_Data_DEF::~Vector_Data_DEF(void)
{

}


int Vector_Data_DEF::data_reading(uint8_t *src)
{

	if (src == NULL)
	{
		return -1;
	}

	for (int i = 0; i < d_len; i++)
	{
		s[i] = *(src + i);
	}

	return 0;
}

void Vector_Data_DEF::copy(class Vector_Data_DEF v)
{
	for (int i = 0; i < d_len; i++)
	{
		s[i] = v.s[i];
	}
}


Vector_Data_DEF  Vector_Data_DEF::operator^ (Vector_Data_DEF &v)
{
	Vector_Data_DEF tmp(d_len);



	 if (s.size() != v.s.size())
	{
		std::cerr << "the size of symbol is not same !! exit!" << std::endl;
		exit(0);
	}


	for (int i = 0; i < s.size(); i++)
	{

			tmp.s[i] = s[i]^v.s[i];			
	}	

	return tmp;

}



// Vector_Data_DEF  Vector_Data_DEF::operator= (Vector_Data_DEF &v)
// {

// 	Vector_Data_DEF tmp(d_len);

// 	 if (s.size() != v.s.size() )
// 	{
// 		std::cerr << "the size of symbol is not same !! exit!" << std::endl;
// 		exit(0);
// 	}

// 	for (int i = 0; i < s.size(); i++)
// 	{

// 			tmp.s[i] = v.s[i];			
// 	}

// 	return tmp;	

// }

Array_Data_Symbol::Array_Data_Symbol(void)
{

}




Array_Data_Symbol::Array_Data_Symbol(uint32_t K)
{
	init(K);
}

Array_Data_Symbol::Array_Data_Symbol(uint32_t K, uint32_t symbol_len)
{
	init(K);
	array_allocation(symbol_len);
	//symbol_pointer = new class Vector_Data_DEF(symbol_len);

}



Array_Data_Symbol::~Array_Data_Symbol(void)
{
	//delete symbol_pointer;
}

int Array_Data_Symbol::init(uint32_t K)
{
	int i = 0;
	_K = K;

	// X be the smallest positive integer such that X*(X-1) >= 2*K.
	for(i = 0; i * (i - 1) < 2*_K; i++)
	{
		;
	}

	_X = i;

	//std::cout << "X : " << _X << std::endl;
	//S be the smallest prime integer such that S >= ceil(0.01*K) + X
	for (i = 0; i < ceil(0.01 * ((double)K)) + _X; i++)
	{
		;
	}

	_S = i;

//	std::cout << "S : " << _S << std::endl;

	_S = util.find_smallest_prime_integer(_S);
	//std::cout << "S : " << _S << std::endl;




    //H be the smallest integer such that choose(H,ceil(H/2)) >= K + S


	for (i = 0; nChoosek(i, ceil((double)(i / 2))) < _K + _S; i++)
	{
		;
	}

	_H = i;

	_HP = ceil((double)(i / 2));

//	std::cout << "H: " << _H << " H`: " << _HP << std::endl;

	_L = _K + _S + _H;

//	std::cout << "L : " << _L << std::endl;

	_LP = util.find_smallest_prime_integer(_L);

//	std::cout << "L` : " << _LP << std::endl;

	get_parameters();

	return 0;
}


int Array_Data_Symbol::array_allocation(uint32_t symbol_len)
{
	//std::vector< std::vector<uint8_t> > tmp;
	_sym_len = symbol_len;
	sym_len = _sym_len;

	//symbol.resize(_L);

	symbol.resize(_L);

	//symbol[0].allocate_size(symbol_len);
	for (int i = 0; i < symbol.size(); i++)
	{
		symbol[i].allocate_size(_sym_len);
	}

//	std::cout << "IS Numbers : " << symbol.size() << std::endl;

	//symbol_pointer = new class Vector_Data_DEF(symbol_len);


	return 0;
}


int Array_Data_Symbol::array_allocation(uint32_t size, uint32_t symbol_len)
{
	symbol.resize(size);
	//symbol.clear();
	_sym_len = symbol_len;
	sym_len = _sym_len;	


	for (int i = 0; i < symbol.size(); i++)
	{
		symbol[i].allocate_size(_sym_len);
	}	

}


void Array_Data_Symbol::get_parameters(void)
{
	X = _X;
	S = _S;
	H = _H;
	HP = _HP;
	L = _L;
	LP = _LP;
	K = _K;
}


int Array_Data_Symbol::nChoosek( uint32_t n, uint32_t k) 
{
    if (k > n) 
    {
    	return 0;
    }
    if (k * 2 > n)
    {
    	k = n-k;
    } 
    if (k == 0) 
    {
    	return 1;
    }

    int result = n;

    for( int i = 2; i <= k; i++ ) 
    {
        result *= (n-i+1);
        result /= i;
    }
    return result;
}



int Array_Data_Symbol::symbol_reading(class Vector_Data_DEF *src)
{

	if (src == NULL)
	{
		return -1;
	}

	for (int i = 0; i < _K; i++)
	{
		symbol[i] = *(src + i);
	}

	return 0;
}


// std::vector< std::vector<uint8_t> > Array_Data_Symbol::operator^ (std::vector< std::vector<uint8_t> > v)
// {

// 	std::vector< std::vector<uint8_t> > tmp(symbol.size());

// 	if (symbol.size() != v.size() )
// 	{
// 		std::cerr << "the size of symbol is not same !!" << std::endl;
// 		exit(0);
// 	}


// 	for (int i = 0; i < symbol.size(); i++)
// 	{
// 		for (int j = 0; j < symbol[i].size(); j++)
// 		{
// 			tmp[i].push_back(symbol[i][j]^v[i][j]);
// 		}		
// 	}

// 	return tmp;

// }
