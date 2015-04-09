
#include "Partition.h"


Partition::~Partition()
{

}


Partition::Partition(uint32_t I, uint32_t J)
{
	float _I, _J;
	_I = I;
	_J = J;
	v.IL = ceil(_I / _J);
	v.IS = floor(_I / _J);
	v.JL = _I - v.IS * _J;
	v.JS = J - v.JL;

}


struct Partition_Out Partition::get(void)
{
	return v;
}

uint32_t Partition::get(uint32_t index)
{
	switch(index)
	{
		case 0 : return v.IL; break;
		case 1 : return v.IS; break;
		case 2 : return v.JL; break;
		case 3 : return v.JS; break;
	}
	if (index > 3)
	{
		std::cerr << "index is out of range!" << std::endl;
	}
}


void Partition::Partition_testing(void)
{
	struct Partition_Out out = get();
	std::cout << "IL: " << out.IL << " IS: " << out.IS << " JL: " << out.JL << " JS: " << out.JS << std::endl;

	std::cout << "IL: " << get(0) << " IS: " << get(1) << " JL: " << get(2) << " JS: " << get(3) << std::endl;

	
}



Object_Partition_to_SB::Object_Partition_to_SB(uint32_t I, uint16_t J)
{
	//partition.Partition(Kt, Z);
	Kt = I;
	Z = J;
	class Partition par(Kt, Z);
	struct Partition_Out out;
	out = par.get();

	v_SB.KL = out.IL;
	v_SB.KS = out.IS;
	v_SB.ZL = out.JL;
	v_SB.ZS = out.JS;

}

Object_Partition_to_SB::~Object_Partition_to_SB(void)
{

}

struct Partition_SB Object_Partition_to_SB::get(void)
{
	return v_SB;
}



SB_Partition_to_subSB::SB_Partition_to_subSB(uint32_t I, uint16_t J)
{
	//partition.Partition(Kt, Z);
	//Kt = I;
	//Z = J;
	class Partition par(I, J);
	struct Partition_Out out;
	out = par.get();

	v.TL = out.IL;
	v.TS = out.IS;
	v.NL = out.JL;
	v.NS = out.JS;

}

SB_Partition_to_subSB::~SB_Partition_to_subSB(void)
{

}

struct Partition_subSB SB_Partition_to_subSB::get(void)
{
	return v;
}

