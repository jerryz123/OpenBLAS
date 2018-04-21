/*********************************************************************/
/* Copyright 2009, 2010 The University of Texas at Austin.           */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/*   1. Redistributions of source code must retain the above         */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer.                                                  */
/*                                                                   */
/*   2. Redistributions in binary form must reproduce the above      */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer in the documentation and/or other materials       */
/*      provided with the distribution.                              */
/*                                                                   */
/*    THIS  SOFTWARE IS PROVIDED  BY THE  UNIVERSITY OF  TEXAS AT    */
/*    AUSTIN  ``AS IS''  AND ANY  EXPRESS OR  IMPLIED WARRANTIES,    */
/*    INCLUDING, BUT  NOT LIMITED  TO, THE IMPLIED  WARRANTIES OF    */
/*    MERCHANTABILITY  AND FITNESS FOR  A PARTICULAR  PURPOSE ARE    */
/*    DISCLAIMED.  IN  NO EVENT SHALL THE UNIVERSITY  OF TEXAS AT    */
/*    AUSTIN OR CONTRIBUTORS BE  LIABLE FOR ANY DIRECT, INDIRECT,    */
/*    INCIDENTAL,  SPECIAL, EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES    */
/*    (INCLUDING, BUT  NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE    */
/*    GOODS  OR  SERVICES; LOSS  OF  USE,  DATA,  OR PROFITS;  OR    */
/*    BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON ANY THEORY OF    */
/*    LIABILITY, WHETHER  IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*    (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY OUT    */
/*    OF  THE  USE OF  THIS  SOFTWARE,  EVEN  IF ADVISED  OF  THE    */
/*    POSSIBILITY OF SUCH DAMAGE.                                    */
/*                                                                   */
/* The views and conclusions contained in the software and           */
/* documentation are those of the authors and should not be          */
/* interpreted as representing official policies, either expressed   */
/* or implied, of The University of Texas at Austin.                 */
/*********************************************************************/

#include <stdio.h>
#include "common.h"

int CNAME(BLASLONG m, BLASLONG n, FLOAT *a, BLASLONG lda, FLOAT *b){

  BLASLONG i, j;
  FLOAT *a_offset, *a_offset1, *a_offset2;
  FLOAT *b_offset, *b_offset1, *b_offset2;
  FLOAT  ctemp1,  ctemp2,  ctemp3,  ctemp4;
  FLOAT  ctemp5,  ctemp6,  ctemp7,  ctemp8;
  FLOAT  ctemp9, ctemp10, ctemp11, ctemp12;
  FLOAT ctemp13, ctemp14, ctemp15, ctemp16;
  resetvcfg();
#if defined(DOUBLE)
  setvcfg0(VFP64, //a_offset10
           VFP64, //a_offset11
           VFP64, //a_offset20
           VFP64);//a_offset21
  setvcfg2(VFP64, //a_offset10
           VFP64, //a_offset11
           VFP64, //a_offset20
           VFP64);//a_offset21
#else
  setvcfg0(VFP32, //a_offset1
           VFP32, //a_offset2
           VFP32,
           VFP32);
  setvcfg2(VFP32, //a_offset1
           VFP32, //a_offset2
           VFP32,
           VFP32);
#endif
  int vl;
  a_offset = a;
  b_offset = b;

  b_offset2 = b + m * (n & ~1) * 2;

  lda *= 2;

  j = (m >> 1);
  if (j > 0){
    do{
      a_offset1 = a_offset;
      a_offset2 = a_offset1 + lda;
      a_offset += 2 * lda;

      b_offset1 = b_offset;
      b_offset += 8;

      i = (n >> 2);
      while (i > 0)
        {
          setvl(vl, i);
          asm volatile ("vlds v0, "       "0" "(%0), %1" : : "r" (a_offset1), "r" (8 << STRIDE_W));
          asm volatile ("vlds v1, " STRIDE_O  "(%0), %1" : : "r" (a_offset1), "r" (8 << STRIDE_W));
          asm volatile ("vlds v2, " STRIDE_O2 "(%0), %1" : : "r" (a_offset1), "r" (8 << STRIDE_W));
          asm volatile ("vlds v3, " STRIDE_O3 "(%0), %1" : : "r" (a_offset1), "r" (8 << STRIDE_W));

          asm volatile ("vlds v4, "       "0" "(%0), %1" : : "r" (a_offset1+4), "r" (8 << STRIDE_W));
          asm volatile ("vlds v5, " STRIDE_O  "(%0), %1" : : "r" (a_offset1+4), "r" (8 << STRIDE_W));
          asm volatile ("vlds v6, " STRIDE_O2 "(%0), %1" : : "r" (a_offset1+4), "r" (8 << STRIDE_W));
          asm volatile ("vlds v7, " STRIDE_O3 "(%0), %1" : : "r" (a_offset1+4), "r" (8 << STRIDE_W));

          asm volatile ("vsts v0, "       "0" "(%0), %1" : : "r" (b_offset1), "r" (2*m*4 << STRIDE_W));
          asm volatile ("vsts v1, " STRIDE_O  "(%0), %1" : : "r" (b_offset1), "r" (2*m*4 << STRIDE_W));
          asm volatile ("vsts v2, " STRIDE_O2 "(%0), %1" : : "r" (b_offset1), "r" (2*m*4 << STRIDE_W));
          asm volatile ("vsts v3, " STRIDE_O3 "(%0), %1" : : "r" (b_offset1), "r" (2*m*4 << STRIDE_W));

          asm volatile ("vsts v4, "       "0" "(%0), %1" : : "r" (b_offset1+4*m), "r" (2*m*4 << STRIDE_W));
          asm volatile ("vsts v5, " STRIDE_O  "(%0), %1" : : "r" (b_offset1+4*m), "r" (2*m*4 << STRIDE_W));
          asm volatile ("vsts v6, " STRIDE_O2 "(%0), %1" : : "r" (b_offset1+4*m), "r" (2*m*4 << STRIDE_W));
          asm volatile ("vsts v7, " STRIDE_O3 "(%0), %1" : : "r" (b_offset1+4*m), "r" (2*m*4 << STRIDE_W));

          asm volatile ("vlds v0, "       "0" "(%0), %1" : : "r" (a_offset2), "r" (8 << STRIDE_W));
          asm volatile ("vlds v1, " STRIDE_O  "(%0), %1" : : "r" (a_offset2), "r" (8 << STRIDE_W));
          asm volatile ("vlds v2, " STRIDE_O2 "(%0), %1" : : "r" (a_offset2), "r" (8 << STRIDE_W));
          asm volatile ("vlds v3, " STRIDE_O3 "(%0), %1" : : "r" (a_offset2), "r" (8 << STRIDE_W));

          asm volatile ("vlds v4, "       "0" "(%0), %1" : : "r" (a_offset2+4), "r" (8 << STRIDE_W));
          asm volatile ("vlds v5, " STRIDE_O  "(%0), %1" : : "r" (a_offset2+4), "r" (8 << STRIDE_W));
          asm volatile ("vlds v6, " STRIDE_O2 "(%0), %1" : : "r" (a_offset2+4), "r" (8 << STRIDE_W));
          asm volatile ("vlds v7, " STRIDE_O3 "(%0), %1" : : "r" (a_offset2+4), "r" (8 << STRIDE_W));

          asm volatile ("vsts v0, "       "0" "(%0), %1" : : "r" (b_offset1+4), "r" (2*m*4 << STRIDE_W));
          asm volatile ("vsts v1, " STRIDE_O  "(%0), %1" : : "r" (b_offset1+4), "r" (2*m*4 << STRIDE_W));
          asm volatile ("vsts v2, " STRIDE_O2 "(%0), %1" : : "r" (b_offset1+4), "r" (2*m*4 << STRIDE_W));
          asm volatile ("vsts v3, " STRIDE_O3 "(%0), %1" : : "r" (b_offset1+4), "r" (2*m*4 << STRIDE_W));

          asm volatile ("vsts v4, "       "0" "(%0), %1" : : "r" (b_offset1+4+4*m), "r" (2*m*4 << STRIDE_W));
          asm volatile ("vsts v5, " STRIDE_O  "(%0), %1" : : "r" (b_offset1+4+4*m), "r" (2*m*4 << STRIDE_W));
          asm volatile ("vsts v6, " STRIDE_O2 "(%0), %1" : : "r" (b_offset1+4+4*m), "r" (2*m*4 << STRIDE_W));
          asm volatile ("vsts v7, " STRIDE_O3 "(%0), %1" : : "r" (b_offset1+4+4*m), "r" (2*m*4 << STRIDE_W));
          b_offset1 += 2 * m * 4 * vl;
          a_offset1 += 8 * vl;
          a_offset2 += 8 * vl;
          i -= vl;
        }

      if (n & 2){
	ctemp1  = *(a_offset1 +  0);
	ctemp2  = *(a_offset1 +  1);
	ctemp3  = *(a_offset1 +  2);
	ctemp4  = *(a_offset1 +  3);

	ctemp9  = *(a_offset2 +  0);
	ctemp10 = *(a_offset2 +  1);
	ctemp11 = *(a_offset2 +  2);
	ctemp12 = *(a_offset2 +  3);

	*(b_offset1 + 0) = ctemp1;
	*(b_offset1 + 1) = ctemp2;
	*(b_offset1 + 2) = ctemp3;
	*(b_offset1 + 3) = ctemp4;

	*(b_offset1 + 4) = ctemp9;
	*(b_offset1 + 5) = ctemp10;
	*(b_offset1 + 6) = ctemp11;
	*(b_offset1 + 7) = ctemp12;

	b_offset1 += m * 4;
	a_offset1 += 4;
	a_offset2 += 4;
      }

      if (n & 1){
	ctemp1  = *(a_offset1 +  0);
	ctemp2  = *(a_offset1 +  1);
	ctemp9  = *(a_offset2 +  0);
	ctemp10 = *(a_offset2 +  1);

	*(b_offset2 + 0) = ctemp1;
	*(b_offset2 + 1) = ctemp2;
	*(b_offset2 + 2) = ctemp9;
	*(b_offset2 + 3) = ctemp10;
	b_offset2 += 4;
      }
      j--;
    } while(j > 0);
  }

  if (m & 1){
    i = (n >> 2);
    while (i > 0)
      {
        setvl(vl, i);
        asm volatile ("vlds v0, "       "0" "(%0), %1" : : "r" (a_offset), "r" (8 << STRIDE_W));
        asm volatile ("vlds v1, " STRIDE_O  "(%0), %1" : : "r" (a_offset), "r" (8 << STRIDE_W));
        asm volatile ("vlds v2, " STRIDE_O2 "(%0), %1" : : "r" (a_offset), "r" (8 << STRIDE_W));
        asm volatile ("vlds v3, " STRIDE_O3 "(%0), %1" : : "r" (a_offset), "r" (8 << STRIDE_W));

        asm volatile ("vlds v4, "       "0" "(%0), %1" : : "r" (a_offset+4), "r" (8 << STRIDE_W));
        asm volatile ("vlds v5, " STRIDE_O  "(%0), %1" : : "r" (a_offset+4), "r" (8 << STRIDE_W));
        asm volatile ("vlds v6, " STRIDE_O2 "(%0), %1" : : "r" (a_offset+4), "r" (8 << STRIDE_W));
        asm volatile ("vlds v7, " STRIDE_O3 "(%0), %1" : : "r" (a_offset+4), "r" (8 << STRIDE_W));

        asm volatile ("vsts v0, "       "0" "(%0), %1" : : "r" (b_offset), "r" (2*m*4 << STRIDE_W));
        asm volatile ("vsts v1, " STRIDE_O  "(%0), %1" : : "r" (b_offset), "r" (2*m*4 << STRIDE_W));
        asm volatile ("vsts v2, " STRIDE_O2 "(%0), %1" : : "r" (b_offset), "r" (2*m*4 << STRIDE_W));
        asm volatile ("vsts v3, " STRIDE_O3 "(%0), %1" : : "r" (b_offset), "r" (2*m*4 << STRIDE_W));

        asm volatile ("vsts v4, "       "0" "(%0), %1" : : "r" (b_offset+4*m), "r" (2*m*4 << STRIDE_W));
        asm volatile ("vsts v5, " STRIDE_O  "(%0), %1" : : "r" (b_offset+4*m), "r" (2*m*4 << STRIDE_W));
        asm volatile ("vsts v6, " STRIDE_O2 "(%0), %1" : : "r" (b_offset+4*m), "r" (2*m*4 << STRIDE_W));
        asm volatile ("vsts v7, " STRIDE_O3 "(%0), %1" : : "r" (b_offset+4*m), "r" (2*m*4 << STRIDE_W));

        b_offset += 2*m*4 * vl;
        a_offset += 8 * vl;
        i -= vl;
      }
    /* if (i > 0){ */
    /*   do{ */
    /*     ctemp1  = *(a_offset +  0); */
    /*     ctemp2  = *(a_offset +  1); */
    /*     ctemp3  = *(a_offset +  2); */
    /*     ctemp4  = *(a_offset +  3); */

    /*     ctemp5  = *(a_offset +  4); */
    /*     ctemp6  = *(a_offset +  5); */
    /*     ctemp7  = *(a_offset +  6); */
    /*     ctemp8  = *(a_offset +  7); */

    /*     *(b_offset + 0) = ctemp1; */
    /*     *(b_offset + 1) = ctemp2; */
    /*     *(b_offset + 2) = ctemp3; */
    /*     *(b_offset + 3) = ctemp4; */

    /*     b_offset += m * 4; */

    /*     *(b_offset + 0) = ctemp5; */
    /*     *(b_offset + 1) = ctemp6; */
    /*     *(b_offset + 2) = ctemp7; */
    /*     *(b_offset + 3) = ctemp8; */

    /*     b_offset += m * 4; */
    /*     a_offset += 8; */
    /*     i --; */
    /*   } while(i > 0); */
    /* } */

    if (n & 2){
      ctemp1  = *(a_offset +  0);
      ctemp2  = *(a_offset +  1);
      ctemp3  = *(a_offset +  2);
      ctemp4  = *(a_offset +  3);

      *(b_offset + 0) = ctemp1;
      *(b_offset + 1) = ctemp2;
      *(b_offset + 2) = ctemp3;
      *(b_offset + 3) = ctemp4;

      b_offset += m * 4;
      a_offset += 4;
    }

    if (n & 1){
      ctemp1  = *(a_offset +  0);
      ctemp2  = *(a_offset +  1);
      *(b_offset2 + 0) = ctemp1;
      *(b_offset2 + 1) = ctemp2;
    }
  }

  return 0;
}
