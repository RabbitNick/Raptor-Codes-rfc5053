#ifndef PARTITION_H
#define PARTITION_H


//#include "rfc5053_config.h"
#include <stdint.h>
#include <math.h>
#include <iostream>


#include "rfc5053_config.h"


/*
The construction of source blocks and sub-blocks is determined based
   on five input parameters, F, Al, T, Z, and N, and a function
   Partition[].  The five input parameters are defined as follows:

   - F  the transfer length of the object, in bytes

   - Al a symbol alignment parameter, in bytes

   - T  the symbol size, in bytes, which MUST be a multiple of Al

   - Z  the number of source blocks

   - N  the number of sub-blocks in each source block

*/

struct Partition_Out
{
	/* data */
	uint32_t IL;
	uint32_t IS;
	uint32_t JL;
	uint32_t JS;
};


class Partition
{
public:
	Partition(uint32_t I, uint32_t J);
	~Partition();

	/* data */
	//the transfer length of the object  
    uint64_t F;
  // the symbol alignment parameter, in bytes
    uint8_t Al;
  	//the symbol size, in bytes, which MUST be a multiple of Al 
  	uint16_t T;
  	//the number of source blocks
  	uint16_t Z;
  	//the number of sub-blocks in each source block
  	uint8_t N;

  	struct Partition_Out get(void);

  	uint32_t get(uint32_t index);

  	void Partition_testing(void);

  //	struct Partition_SB object_partition_to_SB(uint32_t Kt, uint16_t Z);

  //	struct Partition_subSB SB_partition_to_subSB();

private:
	struct Partition_Out v;

};



struct Partition_SB
{
	/* data */
	uint32_t KL;
	uint32_t KS;
	uint32_t ZL;
	uint32_t ZS;
};




class Object_Partition_to_SB
{
public:

	Object_Partition_to_SB(uint32_t I, uint16_t J);
	
	~Object_Partition_to_SB();

	struct Partition_SB get(void);


private:
	uint32_t Kt;
	uint32_t Z;
	struct Partition_SB v_SB;

};



struct Partition_subSB
{
	/* data */
	uint32_t TL;
	uint32_t TS;
	uint32_t NL;
	uint32_t NS;
};


class SB_Partition_to_subSB
{
public:

	SB_Partition_to_subSB(uint32_t I, uint16_t J);
	
	~SB_Partition_to_subSB();

	struct Partition_subSB get(void);


private:
	//uint32_t Kt;
	//uint32_t Z;
	struct Partition_subSB v;

};

#endif