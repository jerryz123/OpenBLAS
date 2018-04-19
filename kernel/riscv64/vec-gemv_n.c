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


int CNAME(BLASLONG m, BLASLONG n, BLASLONG dummy1, FLOAT alpha, FLOAT *a, BLASLONG lda, FLOAT *x, BLASLONG inc_x, FLOAT *y, BLASLONG inc_y, FLOAT *buffer)
{
	BLASLONG i=0;
	BLASLONG ix=0,iy=0;
	BLASLONG j=0;
	FLOAT temp;
        FLOAT* a_ptr;
        int vl;
        if (alpha == 0.0) return(0);
        resetvcfg();
#if defined(DOUBLE)
        setvcfg0(VFP64, // y[]
                 SFP64, // x
                 VFP64, // a[]
                 SFP64);// alpha
#else
        setvcfg0(VFP32, // y[]
                 SFP32, // x
                 VFP32, // a[]
                 SFP32);// alpha
#endif
        asm volatile ("vinsert v3, %0, x0" : : "r" (alpha));
        while (i < m)
          {
            a_ptr = a + i;
            ix = 0;
            setvl(vl, m - i);
            asm volatile ("vlds  v0, 0(%0), %1" : : "r" (&y[iy]), "r" (inc_y << STRIDE_W));
            for (j=0; j < n; j++)
              {
                asm volatile ("vinsert v1, %0, x0" : : "r" (x[ix]));
                asm volatile ("vld     v2, 0(%0)"  : : "r" (a_ptr));
                asm volatile ("vmul    v1, v1, v3");
                asm volatile ("vmadd   v0, v1, v2, v0");
                a_ptr += lda;
                ix += inc_x;
              }
            asm volatile ("vsts  v0, 0(%0), %1" : : "r" (&y[iy]), "r" (inc_y << STRIDE_W));
            i += vl;
            iy += inc_y * vl;
          }
	return(0);
}


