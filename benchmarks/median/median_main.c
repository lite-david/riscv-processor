// See LICENSE for license details.

//**************************************************************************
// Median filter bencmark
//--------------------------------------------------------------------------
//
// This benchmark performs a 1D three element median filter. The
// input data (and reference data) should be generated using the
// median_gendata.pl perl script and dumped to a file named
// dataset1.h.

#include "median.h"
#define STR1(x) #x
#define STR(x) STR1(x)
#define EXTRACT(a, size, offset) (((~(~0 << size) << offset) & a) >> offset)

#define CUSTOMX_OPCODE(x) CUSTOM_##x
#define CUSTOM_0 0b0001011
#define CUSTOM_1 0b0101011
#define CUSTOM_2 0b1011011
#define CUSTOM_3 0b1111011

#define CUSTOMX(X, rd, rs1, rs2, funct) \
  CUSTOMX_OPCODE(X)                   | \
  (rd                   << (7))       | \
  (0x7                  << (7+5))     | \
  (rs1                  << (7+5+3))   | \
  (rs2                  << (7+5+3+5)) | \
  (EXTRACT(funct, 7, 0) << (7+5+3+5+5))

#define CUSTOMX_R_R_R(X, rd, rs1, rs2, funct)           \
  asm volatile ("mv a4, %[_rs1]\n\t"                             \
       "mv a5, %[_rs2]\n\t"                             \
       ".word "STR(CUSTOMX(X, 15, 14, 15, funct))"\n\t" \
       "mv %[_rd], a5"                                  \
       : [_rd] "=r" (rd)                                \
       : [_rs1] "r" (rs1), [_rs2] "r" (rs2)             \
       : "a4", "a5");

#include "dataset1.h"

// Main
void main( )
{
  int results_data[DATA_SIZE];

  // Do the filter
  int i=10;
  int j;
  int k;
  CUSTOMX_R_R_R(0,k,i,i,0)
  median( DATA_SIZE, input_data, results_data );
}
