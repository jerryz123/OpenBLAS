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
 * * 2013/11/23 Saar
 * *	 BLASTEST float		: OK
 * * 	 BLASTEST double	: OK
 * 	 CTEST			: OK
 * 	 TEST			: OK
 * *
 * **************************************************************************************/


#include "common.h"

int CNAME(BLASLONG m, BLASLONG n, BLASLONG dummy1, FLOAT alpha_r, FLOAT alpha_i, FLOAT *a, BLASLONG lda, FLOAT *x, BLASLONG inc_x, FLOAT *y, BLASLONG inc_y, FLOAT *buffer)
{
	BLASLONG i=0;
	BLASLONG ix=0,iy=0;
	BLASLONG j=0;
	FLOAT *a_ptr=a;
        FLOAT x_r;
        FLOAT x_i;
        int vl;
        BLASLONG i2=0;
	BLASLONG inc_x2=inc_x<<1;
        BLASLONG inc_y2=inc_y<<1;
	BLASLONG lda2=lda<<1;

        resetvcfg();
#if defined(DOUBLE)
        setvcfg0(VFP64, // y[] real
                 VFP64, // y[] imag
                 SFP64, // temp real
                 SFP64);// temp imag
        setvcfg2(VFP64, // a[] real
                 SFP64, // a[]
                 SFP64, //
                 SFP64);// temp

#else
        setvcfg0(VFP32, // y[] real
                 VFP32, // y[] imag
                 SFP32, // temp real
                 SFP32);// temp imag
        setvcfg2(VFP32, // a[]
                 SFP32, //
                 SFP32, //
                 SFP32);// temp
#endif

        while (i < m)
          {
            a_ptr = a + i2;
            ix = 0;
            setvl(vl, m - i);
            asm volatile ("vlds v0, 0(%0), %1"            : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));
            asm volatile ("vlds v1, " STRIDE_O "(%0), %1" : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));
            for (j=0; j<n; j++)
              {
                x_r = x[ix];
                x_i = x[ix+1];
#if !defined(XCONJ)
                asm volatile ("vinsert v2, %0, x0" : : "r" (alpha_r*x_r - alpha_i*x_i));
                asm volatile ("vinsert v3, %0, x0" : : "r" (alpha_r*x_i + alpha_i*x_r));
#else
                asm volatile ("vinsert v2, %0, x0" : : "r" (alpha_r*x_r + alpha_i*x_i));
                asm volatile ("vinsert v3, %0, x0" : : "r" (alpha_r*x_i - alpha_i*x_r));
#endif

                asm volatile ("vlds    v4, 0(%0), %1" : : "r" (a_ptr), "r" (2 << STRIDE_W));
#if !defined(CONJ)
#if !defined(XCONJ)
                asm volatile ("vmadd   v0, v2, v4, v0");
                asm volatile ("vmadd   v1, v3, v4, v1");
#else
                asm volatile ("vmadd   v0, v2, v4, v0");
                asm volatile ("vnmsub  v1, v3, v4, v1");
#endif
#else
#if !defined(XCONJ)
                asm volatile ("vmadd   v0, v2, v4, v0");
                asm volatile ("vnmsub  v1, v3, v4, v1");
#else
                asm volatile ("vmadd   v0, v2, v4, v0");
                asm volatile ("vmadd   v1, v3, v4, v1");
#endif
#endif
                asm volatile ("vlds    v4, " STRIDE_O "(%0), %1" : : "r" (a_ptr), "r" (2 << STRIDE_W));
#if !defined(CONJ)
#if !defined(XCONJ)
                asm volatile ("vnmsub  v0, v3, v4, v0");
                asm volatile ("vmadd   v1, v2, v4, v1");
#else
                asm volatile ("vmadd   v0, v3, v4, v0");
                asm volatile ("vmadd   v1, v2, v4, v1");
#endif
#else
#if !defined(XCONJ)
                asm volatile ("vmadd   v0, v3, v4, v0");
                asm volatile ("vnmsub  v1, v2, v4, v1");
#else
                asm volatile ("vnmsub  v0, v3, v4, v0");
                asm volatile ("vnmsub  v1, v2, v4, v1");
#endif
#endif
                a_ptr += lda2;
                ix += inc_x2;
              }
            asm volatile ("vsts v0, 0(%0), %1"            : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));
            asm volatile ("vsts v1, " STRIDE_O "(%0), %1" : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));

            i += vl;
            i2 += 2*vl;
            iy += inc_y2 * vl;
          }

	return(0);
}
