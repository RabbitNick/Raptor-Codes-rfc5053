
#include "Utility.h"




Utility::Utility(void)
{

}


Utility::~Utility(void)
{

}


bool Utility::checking_prime_integer(uint32_t v)
{
	for (int i = 2; i < v; i++)
	{
		if (v % i == 0)
		{
			return 0;
		}
	}


	return 1;
}



uint32_t Utility::find_smallest_prime_integer(uint32_t v)
{
	bool r = 0;
	uint32_t prime = v;
	r = checking_prime_integer(prime);
	while(r == 0)
	{
		prime++;
		r = checking_prime_integer(prime);
		
	}

	//prime--;

	return prime;
}

ublas::vector<uint8_t>  Utility::matrix_row_XOR(ublas::vector<uint8_t> row_1, ublas::vector<uint8_t> row_2)
{




	if (row_1.size() != row_2.size())
	{
		std::cout << "The columns are not the same in two rows!" << std::endl;
		exit(-1);
	}

	ublas::vector<uint8_t> r(row_1.size());

	for (int i = 0; i < row_1.size(); ++i)
	{
		r(i) = row_1(i) ^ row_2(i);
	}

	return r;
}




// template<class T>
// bool Utility::InvertMatrix (const boost::numeric::ublas::matrix<T>& input, boost::numeric::ublas::matrix<T>& inverse)
// {
// 	using namespace boost::numeric::ublas;
// 	typedef permutation_matrix<std::size_t> pmatrix;
// 	// create a working copy of the input
// 	matrix<T> A(input);
// 	// create a permutation matrix for the LU-factorization
// 	pmatrix pm(A.size1());
// 	// perform LU-factorization
// 	int res = lu_factorize(A,pm);
//    	if( res != 0 ) return false;
// 	// create identity matrix of "inverse"
// 	inverse.assign(boost::numeric::ublas::identity_matrix<T>(A.size1()));
// 	// backsubstitute to get the inverse
// 	lu_substitute(A, pm, inverse);
// 	return true;
// }