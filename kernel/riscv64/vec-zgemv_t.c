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
 * * 2013/09/14 Saar
 * *	 BLASTEST float		: OK
 * * 	 BLASTEST double	: OK
 * 	 CTEST			: OK
 * 	 TEST			: OK
 * *
 * **************************************************************************************/


#include "common.h"
#include "rvv.h"

int CNAME(BLASLONG m, BLASLONG n, BLASLONG dummy1, FLOAT alpha_r, FLOAT alpha_i, FLOAT *a, BLASLONG lda, FLOAT *x, BLASLONG inc_x, FLOAT *y, BLASLONG inc_y, FLOAT *buffer)
{
	BLASLONG i;
	BLASLONG ix=0,iy=0,ia=0;
	BLASLONG j;
	FLOAT *a_ptr;

        BLASLONG inc_x2 = inc_x << 1;
        BLASLONG inc_y2 = inc_y << 1;
        BLASLONG lda2 = lda << 1;

        resetvcfg();
#if defined(DOUBLE)
        setvcfg0(VFP64,    // x[] real
                 VFP64,    // y[] real
                 VFP64,    // acc[] real
                 SFP64);   //
        setvcfg2(VFP64,    // x[] imag
                 VFP64,    // y[] imag
                 VFP64,    // acc[] imag
                 SFP64);
#else
        setvcfg0(VFP32,    // x[] real
                 VFP32,    // y[] real
                 VFP32,    // acc[] real
                 SFP32);   //
        setvcfg2(VFP32,    // x[] imag
                 VFP32,    // y[] imag
                 VFP32,    // acc[] imag
                 SFP32);
#endif
	iy = 0;
	a_ptr = a;
        int vl = 0;
        setvl(vl, m);
        int ct = 0;
        while (vl > 1)
          {
            ct++;
            vl = vl >> 1;
          }
        int stvl = 1 << ct;
	for (j=0; j<n; j++)
	{
          setvl(vl, stvl);
          asm volatile ("vsne v2, v2, v2");
          asm volatile ("vsne v6, v6, v6");
          ix = 0;
          i = 0;
          ia = 0;
          while (i < m)
            {
              while (m - i < vl)
                {
                  asm volatile ("vslide v0, v2, %0" : : "r" (vl >> 1));
                  asm volatile ("vslide v4, v6, %0" : : "r" (vl >> 1));
                  setvl(vl, vl >> 1);
                  asm volatile ("vadd   v2, v2, v0");
                  asm volatile ("vadd   v6, v6, v4");
                }
              asm volatile ("vlds  v0, 0(%0), %1" : : "r" (&x[ix]), "r" (inc_x2 << STRIDE_W));
              asm volatile ("vlds  v1, 0(%0), %1" : : "r" (&a_ptr[ia]), "r" (2 << STRIDE_W));
              asm volatile ("vlds  v4, " STRIDE_O "(%0), %1" : : "r" (&x[ix]), "r" (inc_x2 << STRIDE_W));
              asm volatile ("vlds  v5, " STRIDE_O "(%0), %1" : : "r" (&a_ptr[ia]), "r" (2 << STRIDE_W));
#if (!defined(CONJ) && !defined(XCONJ)) || (defined(CONJ) && defined(XCONJ))
              asm volatile ("vmadd  v2, v0, v1, v2");
              asm volatile ("vnmsub v2, v4, v5, v2");
              asm volatile ("vmadd  v6, v1, v4, v6");
              asm volatile ("vmadd  v6, v0, v5, v6");
#else
              asm volatile ("vmadd  v2, v0, v1, v2");
              asm volatile ("vmadd  v2, v4, v5, v2");
              asm volatile ("vmadd  v6, v1, v4, v6");
              asm volatile ("vnmsub v6, v0, v5, v6");
#endif

              i = i + vl;
              ix = ix + vl * inc_x2;
              ia = ia + vl * 2;
            }
          while (vl > 1)
            {
              asm volatile ("vslide v0, v2, %0" : : "r" (vl >> 1));
              asm volatile ("vslide v4, v6, %0" : : "r" (vl >> 1));
              setvl(vl, vl >> 1);
              asm volatile ("vadd v2, v2, v0");
              asm volatile ("vadd v6, v6, v4");
            }
          FLOAT f_r;
          FLOAT f_i;
          asm volatile ("vst v2, 0(%0)" : : "r" (&f_r));
          asm volatile ("vst v6, 0(%0)" : : "r" (&f_i));
#if !defined(XCONJ)
          y[iy]   += alpha_r * f_r - alpha_i * f_i;
          y[iy+1] += alpha_r * f_i + alpha_i * f_r;
#else
          y[iy]   += alpha_r * f_r + alpha_i * f_i;
          y[iy+1] -= alpha_r * f_i - alpha_i * f_r;
#endif

          iy += inc_y2;
          a_ptr += lda2;
	}
	return(0);

}


