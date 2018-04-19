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

int CNAME(BLASLONG n, FLOAT *x, BLASLONG inc_x, FLOAT *y, BLASLONG inc_y, FLOAT c, FLOAT s)
{
	BLASLONG i=0;
        BLASLONG ix=0;
        BLASLONG iy=0;


	if ( n <= 0     )  return(0);

        resetvcfg();
#if defined(DOUBLE)
        setvcfg0(VFP64,    // *x
                 VFP64,    // *y
                 VFP64,    // *temp0
                 SFP64);   // c
        setvcfg2(SFP64,    // s
                 SFP32,    //
                 SFP32,    //
                 SFP32);   //
#else
        setvcfg0(VFP32,    // *x
                 VFP32,    // *y
                 VFP32,    // *temp0
                 SFP32);   // c
        setvcfg2(SFP32,    // s
                 SFP32,    //
                 SFP32,    //
                 SFP32);   //
#endif

        int vl = 0;
        asm volatile ("vinsert v3, %0, x0" : : "r" (c));
        asm volatile ("vinsert v4, %0, x0" : : "r" (s));


        while(i < n)
          {
            setvl(vl, n - i);
            asm volatile ("vlds  v0, 0(%0), %1" : : "r" (&x[ix]), "r" (inc_x << STRIDE_W));
            asm volatile ("vlds  v1, 0(%0), %1" : : "r" (&y[iy]), "r" (inc_y << STRIDE_W));
            asm volatile ("vmul  v2, v4, v1");     // temp0 = s*y[]
            asm volatile ("vmadd v2, v3, v0, v2"); // temp0 = c*x[] + x*y[]
            asm volatile ("vmul  v0, v4, v0");     // x     = s*x[]
            asm volatile ("vmsub v1, v3, v1, v0"); // y     = c*y[] - s*x[]
            asm volatile ("vsts  v2, 0(%0), %1" : : "r" (&x[ix]), "r" (inc_x << STRIDE_W));
            asm volatile ("vsts  v1, 0(%0), %1" : : "r" (&y[iy]), "r" (inc_y << STRIDE_W));

            i = i + vl;
            ix = ix + inc_x * vl;
            iy = iy + inc_y * vl;
          }

	return(0);

}
