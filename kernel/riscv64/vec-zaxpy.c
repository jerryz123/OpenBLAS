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
*	 BLASTEST float		: OK
* 	 BLASTEST double	: OK
* 	 CTEST			: OK
* 	 TEST			: OK
*
**************************************************************************************/


#include "common.h"

int CNAME(BLASLONG n, BLASLONG dummy0, BLASLONG dummy1, FLOAT da_r, FLOAT da_i, FLOAT *x, BLASLONG inc_x, FLOAT *y, BLASLONG inc_y, FLOAT *dummy, BLASLONG dummy2)
{
	BLASLONG i=0;
	BLASLONG ix,iy;
        BLASLONG inc_x2 = inc_x << 1;
        BLASLONG inc_y2 = inc_y << 1;
        int vl;

	if ( n < 0     )  return(0);
	if ( da_r == 0.0 && da_i == 0.0 ) return(0);

        resetvcfg();
#if defined(DOUBLE)
        setvcfg0(VFP64, // y[] real
                 VFP64, // x[] real
                 SFP64, // da  real
                 VFP64);// y[] imag
        setvcfg2(VFP64, // x[] imag
                 SFP64, // da  imag
                 SFP64, // 
                 SFP64);
#else
        setvcfg0(VFP32, // y[] real
                 VFP32, // x[] real
                 SFP32, // da  real
                 VFP32);// y[] imag
        setvcfg2(VFP32, // x[] imag
                 SFP32, // da  imag
                 SFP32, // 
                 SFP32);
#endif
	ix = 0;
	iy = 0;

        asm volatile ("vinsert v2, %0, x0" : : "r" (da_r));
        asm volatile ("vinsert v5, %0, x0" : : "r" (da_i));
        while (i < n)
          {
            setvl(vl, n - i);
            asm volatile ("vlds  v0, 0(%0), %1" : : "r" (&x[ix]), "r" (inc_x2 << STRIDE_W));
            asm volatile ("vlds  v1, 0(%0), %1" : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));
            asm volatile ("vlds  v4, " STRIDE_O "(%0), %1" : : "r" (&x[ix]), "r" (inc_x2 << STRIDE_W));
            asm volatile ("vlds  v3, " STRIDE_O "(%0), %1" : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));

#if !defined(CONJ)
            asm volatile ("vmadd  v1, v2, v0, v1");
            asm volatile ("vnmsub v1, v5, v4, v1");
            asm volatile ("vmadd  v3, v2, v4, v3");
            asm volatile ("vmadd  v3, v5, v0, v3");
#else
            asm volatile ("vmadd  v1, v2, v0, v1");
            asm volatile ("vmadd  v1, v5, v4, v1");
            asm volatile ("vnmsub v3, v2, v4, v3");
            asm volatile ("vmadd  v3, v5, v0, v3");
#endif
            asm volatile ("vsts  v1, 0(%0), %1" : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));
            asm volatile ("vsts  v3, " STRIDE_O "(%0), %1" : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));
            i = i + vl;
            ix = ix + vl * inc_x2;
            iy = iy + vl * inc_y2;
          }
	return(0);

}
