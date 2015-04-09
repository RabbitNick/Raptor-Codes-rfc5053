#ifndef LT_ENCODING_H
#define LT_ENCODING_H 


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <math.h>

#include "RandNum_Generator.h"
#include "Degree_Generator.h"
#include "Triple_Generator.h"
#include "Array_Data_Types.h"

#include "R10_Decoder.h"

class LT_Encoding
{
public:
	LT_Encoding(void);
	LT_Encoding(class Array_Data_Symbol *p);


	~LT_Encoding(void);

	class Vector_Data_DEF LTEnc(class Array_Data_Symbol *p, struct Triple triple);

	void init(class Array_Data_Symbol *p);

	class Array_Data_Symbol Generate_Intermediate_Symbols(class Array_Data_Symbol *p);


	std::vector<class Vector_Data_DEF> LTEnc_Generate(std::vector<uint32_t> ESIs);

	/* data */


private:
	class R10_Decoder *decoder;
    class Array_Data_Symbol *D;
    class Triple_Generator triple;
	
	//class Array_Data_Symbol IS_v
    class Array_Data_Symbol C;



};



#endif