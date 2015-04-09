#ifndef RAPTOR_CONFIG_H
#define RAPTOR_CONFIG_H


#include <stdint.h>



/*
FEC Payload IDs

The FEC Payload ID MUST be a 4 octet field defined as follows:

 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Source Block Number       |      Encoding Symbol ID       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
Source Block Number (SBN), (16 bits): An integer identifier for
the source block that the encoding symbols within the packet
relate to.

Encoding Symbol ID (ESI), (16 bits): An integer identifier for the
encoding symbols within the packet.
Source Block Number (SBN), (16 bits): An integer identifier for
the source block that the encoding symbols within the packet
relate to.

Encoding Symbol ID (ESI), (16 bits): An integer identifier for the
encoding symbols within the packet.

*/



struct Payload_ID_Format
{
	/* data */
	uint16_t SBN;
	uint16_t ESI;
};


/*
FEC Object Transmission Information (OTI)

   The Common FEC Object Transmission Information elements used by this
   FEC Scheme are:

      - Transfer Length (F)

      - Encoding Symbol Length (T)

   The Transfer Length is a non-negative integer less than 2^^45.  The
   Encoding Symbol Length is a non-negative integer less than 2^^16.

   The encoded Common FEC Object Transmission Information format is
   shown in Figure 2.

       0                   1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                      Transfer Length                          |
      +                               +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                               |           Reserved            |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |    Encoding Symbol Length     |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

          Figure 2: Encoded Common FEC OTI for Raptor FEC Scheme

      NOTE 1: The limit of 2^^45 on the transfer length is a consequence
      of the limitation on the symbol size to 2^^16-1, the limitation on
      the number of symbols in a source block to 2^^13, and the limitation on the number of source blocks to 2^^16.  However, the
      Transfer Length is encoded as a 48-bit field for simplicity.


*/




struct OTI_Format
{
	/* data */

	// Transfer Length
  uint64_t F : 48;
  uint64_t Reserved : 16;
 // uint32_t F[2];
 //Encoding Symbol Length 
	uint16_t T; 
};



/*

Scheme-Specific

  The following parameters are carried in the Scheme-Specific FEC
   Object Transmission Information element for this FEC Scheme:

      - The number of source blocks (Z)

      - The number of sub-blocks (N)

      - A symbol alignment parameter (Al)

   These parameters are all non-negative integers.  The encoded Scheme-
   specific Object Transmission Information is a 4-octet field
   consisting of the parameters Z (2 octets), N (1 octet), and Al (1
   octet) as shown in Figure 3.

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |             Z                 |      N        |       Al      |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

   Figure 3: Encoded Scheme-Specific FEC Object Transmission Information

   The encoded FEC Object Transmission Information is a 14-octet field
   consisting of the concatenation of the encoded Common FEC Object
   Transmission Information and the encoded Scheme-Specific FEC Object
   Transmission Information.

*/

struct Scheme_Specific_Format
{
  /* data */

 //The number of source blocks (Z)
//  uint32_t Z : 16;
  uint16_t Z;
 //The number of sub-blocks (N)
//  uint32_t N : 8;
  uint8_t N;
 //A symbol alignment parameter (Al)  
//  uint32_t Al : 8;
  uint8_t Al;
};



/*
Content Delivery Protocol Requirement(CDP)


The Raptor encoder and decoder for object delivery require the
   following information from the CDP:

      - The transfer length of the object, F, in bytes
          - A symbol alignment parameter, Al

      - The symbol size, T, in bytes, which MUST be a multiple of Al

      - The number of source blocks, Z

      - The number of sub-blocks in each source block, N

   The Raptor encoder for object delivery additionally requires:

      - the object to be encoded, F bytes

   The Raptor encoder supplies the CDP with the following information
   for each packet to be sent:

      - Source Block Number (SBN)

      - Encoding Symbol ID (ESI)

      - Encoding symbol(s)

   The CDP MUST communicate this information to the receiver.


*/

struct CDP_Parameters_Input
{
    //the transfer length of the object  
    uint64_t F;
  //a target on the sub-block size, in bytes
    uint64_t W;
  //the maximum packet payload size, in bytes, which is assumed to be a multiple of Al
    uint32_t P;
  // the symbol alignment parameter, in bytes
    uint8_t Al;
  //the maximum number of source symbols per source block, defines K_max to be 8192
    uint32_t Kmax;
  //a minimum target on the number of symbols per source block
    uint32_t Kmin;
  //a maximum target number of symbols per packet
    uint32_t Gmax;

};


struct CDP_Parameters_Output
{
  //The number of source blocks (Z)
  uint16_t Z;
  //The number of sub-blocks (N)
  uint8_t N;
  //Encoding Symbol Length 
  uint16_t T;
};

class CDP_Transport_Parameters
  {
  public:
  //  CDP_Transport_Parameters(void);
  // ~CDP_Transport_Parameters(void);
 
   struct CDP_Parameters_Output CDP_param;




  // Recommended settings for the input parameters, Al, Kmin, and Gmax are    as follows: Al = 4, Kmin = 1024, Gmax = 10.
  struct CDP_Parameters_Output CDP_Parameters_caculate(struct CDP_Parameters_Input v)
  {
    struct CDP_Parameters_Output r;
    G = std::min(ceil((double)(v.P * v.Kmin) / (double)v.F), (double)(v.P / v.Al));

    G = std::min(G, (double)v.Gmax);

    r.T = floor(v.P / (v.Al * G)) * v.Al;

    Kt = ceil(v.F / r.T);

    r.Z = ceil(Kt / v.Kmax);

    r.N = std::min((uint64_t)ceil(ceil(Kt / r.Z) * (double)(r.T / v.W)), (uint64_t)(r.T / v.Al));


    return r;
  }


  void CDP_Parameters_print(void)
  {
    struct CDP_Parameters_Output r;
    CDP_Parameters_testing();
    r = CDP_param;
    std::cout << "T: " << r.T << " N: " <<(uint32_t) r.N << " Z : " << r.Z << std::endl;
  }


  private:
  struct CDP_Parameters_Input In_v;

  //The value G represents the maximum number of symbols to be    transported in a single packet.  
  double G;
  //The value Kt is the total number of    symbols required to represent the source data of the object. 
  double Kt;

  void CDP_Parameters_testing(void)
  {
    struct CDP_Parameters_Input v;
    v.Al = 4;
    v.Kmin = 1024;
    v.Kmax = 8192;
    v.Gmax = 10;
    v.F = 1024*64*8192;
    v.W = 64;
    v.P = 512;
    CDP_param = CDP_Parameters_caculate(v);
  }



 }; 

#endif