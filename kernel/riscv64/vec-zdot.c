/***************************************************************************
Copyright (c) 2013, The OpenBLAS Project
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in
the documentation and/or other materials provided with the
distribution.
3. Neither the name of the OpenBLAS project nor the names of
its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE OPENBLAS PROJECT OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

/**************************************************************************************
* 2013/09/14 Saar
*	 BLASTEST float		: FAIL
* 	 BLASTEST double	: FAIL
* 	 CTEST			: OK
* 	 TEST			: OK
*
**************************************************************************************/

#include "common.h"

OPENBLAS_COMPLEX_FLOAT CNAME(BLASLONG n, FLOAT *x, BLASLONG inc_x, FLOAT *y, BLASLONG inc_y)
{
	BLASLONG i=0;
	BLASLONG ix=0,iy=0;
	FLOAT dot[2];
	OPENBLAS_COMPLEX_FLOAT result;
	BLASLONG inc_x2;
	BLASLONG inc_y2;

	dot[0]=0.0;
	dot[1]=0.0;

	CREAL(result) = 0.0 ;
	CIMAG(result) = 0.0 ;

	if ( n < 1 )  return(result);
        resetvcfg();
#if defined(DOUBLE)
        setvcfg0(VFP64,    // *x real
                 VFP64,    // *y real
                 VFP64,    // *acc real
                 SFP64);   // temp
        setvcfg2(VFP64,    // *x imag
                 VFP64,    // *y imag
                 VFP64,    // *acc imag
                 SFP64);
#else
        setvcfg0(VFP32,    // *x
                 VFP32,    // *y
                 VFP32,    // *acc
                 SFP32);   // temp
        setvcfg2(VFP32,    // *x
                 VFP32,    // *y
                 VFP32,    // *acc
                 SFP64);
#endif
        int vl = 0;
        setvl(vl, n);
        log2floor(vl, vl);
        setvl(vl, vl);
	inc_x2 = 2 * inc_x ;
	inc_y2 = 2 * inc_y ;
        asm volatile ("vsne v2, v2, v2");
        asm volatile ("vsne v6, v6, v6");

	while(i < n)
	{
          while (n - i < vl)
            {
                asm volatile ("vslide v0, v2, %0" : : "r" (vl >> 1));
                asm volatile ("vslide v4, v6, %0" : : "r" (vl >> 1));
                setvl(vl, vl >> 1);
                asm volatile ("vadd   v2, v2, v0"); // acc[] = acc[] + acc[+vl]
                asm volatile ("vadd   v6, v6, v4");
            }
            asm volatile ("vlds  v0, 0(%0), %1" : : "r" (&x[ix]), "r" (inc_x2 << STRIDE_W));
            asm volatile ("vlds  v1, 0(%0), %1" : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));
            asm volatile ("vlds  v4, " STRIDE_O "(%0), %1" : : "r" (&x[ix]), "r" (inc_x2 << STRIDE_W));
            asm volatile ("vlds  v5, " STRIDE_O "(%0), %1" : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));
#if !defined(CONJ)
            asm volatile ("vmadd  v2, v0, v1, v2");
            asm volatile ("vnmsub v2, v4, v5, v2");
            asm volatile ("vmadd  v6, v1, v4, v6");
            asm volatile ("vmadd  v6, v0, v5, v6");
#else
            asm volatile ("vmadd  v2, v0, v1, v2");
            asm volatile ("vmadd  v2, v4, v5, v2");
            asm volatile ("vnmsub v6, v1, v4, v6");
            asm volatile ("vmadd  v6, v0, v5, v6");
#endif
            i = i + vl;
            ix = ix + vl * inc_x2;
            iy = iy + vl * inc_y2;
        }

        while (vl > 1)
          {
            asm volatile ("vslide v0, v2, %0" : : "r" (vl >> 1));
            asm volatile ("vslide v4, v6, %0" : : "r" (vl >> 1));
            setvl(vl, vl >> 1);
            asm volatile ("vadd   v2, v2, v0"); // acc[] = acc[] + acc[+vl]
            asm volatile ("vadd   v6, v6, v4");
          }
        FLOAT dot0;
        FLOAT dot1;
        asm volatile ("vextract %0, v2, x0" : "=r" (dot0));
        asm volatile ("vextract %0, v6, x0" : "=r" (dot1));

	CREAL(result) = dot0;
	CIMAG(result) = dot1;
	return(result);

}
