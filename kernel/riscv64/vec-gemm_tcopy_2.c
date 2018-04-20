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

  a_offset = a;
  b_offset = b;
  b_offset2 = b + m * (n & ~1);

  i = (m >> 1);

  resetvcfg();
#if defined(DOUBLE)
  setvcfg0(VFP64, //a_offset10
           VFP64, //a_offset11
           VFP64, //a_offset20
           VFP64);//a_offset21
#else
  setvcfg0(VFP32, //a_offset1
           VFP32, //a_offset2
           VFP32,
           VFP32);
#endif
  int vl;
  if (i > 0) {
    do {
      a_offset1 = a_offset;
      a_offset2 = a_offset + lda;
      a_offset += 2 * lda;

      b_offset1 = b_offset;
      b_offset += 4;

      j = (n >> 1);
      while (j > 0)
        {
          setvl(vl, j);
          asm volatile ("vlds v0, 0(%0), %1"            : : "r" (a_offset1), "r" (2 << STRIDE_W));
          asm volatile ("vlds v1, " STRIDE_O "(%0), %1" : : "r" (a_offset1), "r" (2 << STRIDE_W));
          asm volatile ("vlds v2, 0(%0), %1"            : : "r" (a_offset2), "r" (2 << STRIDE_W));
          asm volatile ("vlds v3, " STRIDE_O "(%0), %1" : : "r" (a_offset2), "r" (2 << STRIDE_W));

          asm volatile ("vsts v0, 0(%0), %1"            : : "r" (b_offset1), "r" (m*2 << STRIDE_W));
          asm volatile ("vsts v1, " STRIDE_O "(%0), %1" : : "r" (b_offset1), "r" (m*2 << STRIDE_W));
          asm volatile ("vsts v2, " STRIDE_O2"(%0), %1" : : "r" (b_offset1), "r" (m*2 << STRIDE_W));
          asm volatile ("vsts v3, " STRIDE_O3"(%0), %1" : : "r" (b_offset1), "r" (m*2 << STRIDE_W));
          a_offset1 += 2 * vl;
          a_offset2 += 2 * vl;
          b_offset1 += m * 2 * vl;
          j -= vl;
        }

      if (n & 1){
	  *(b_offset2 + 0) = *(a_offset1 + 0);
	  *(b_offset2 + 1) = *(a_offset2 + 0);
	  b_offset2 += 2;
      }
      i --;
    } while (i > 0);
  }

  if (m & 1) {
    j = (n >> 1);
    while (j > 0)
      {
        setvl(vl, j);
        asm volatile ("vlds v0, 0(%0), %1"            : : "r" (a_offset), "r" (2 << STRIDE_W));
        asm volatile ("vlds v1, " STRIDE_O "(%0), %1" : : "r" (a_offset), "r" (2 << STRIDE_W));
        asm volatile ("vsts v0, 0(%0), %1"            : : "r" (b_offset), "r" (m*2 << STRIDE_W));
        asm volatile ("vsts v1, " STRIDE_O "(%0), %1" : : "r" (b_offset), "r" (m*2 << STRIDE_W));
        a_offset += 2 * vl;
        b_offset += m * 2 * vl;
        j -= vl;
      }


    if (n & 1){
      *(b_offset2 + 0) = *(a_offset + 0);
      }
  }

  return 0;
}
