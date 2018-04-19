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
* 2013/08/20 Saar
*	 BLASTEST float		OK
* 	 BLASTEST double	OK
*
**************************************************************************************/

#include "common.h"


int CNAME(BLASLONG n, BLASLONG dummy0, BLASLONG dummy1, FLOAT dummy3, FLOAT dummy4, FLOAT *x, BLASLONG inc_x, FLOAT *y, BLASLONG inc_y, FLOAT *dummy, BLASLONG dummy2)
{
	BLASLONG i=0;
	BLASLONG ix=0,iy=0;
        BLASLONG inc_x2 = inc_x << 1;
        BLASLONG inc_y2 = inc_y << 1;
	FLOAT temp;

	if ( n < 0     )  return(0);

        resetvcfg();
#if defined(DOUBLE)
        setvcfg0(VFP64, // y[]
                 VFP64, // x[]
                 SFP32,
                 SFP32);
#else
        setvcfg0(VFP32, // y[]
                 VFP32, // x[]
                 SFP32,
                 SFP32);
#endif
        int vl;

	while(i < n)
	{
          setvl(vl, n - i);
          asm volatile ("vlds  v0, 0(%0), %1" : : "r" (&x[ix]), "r" (inc_x2 << STRIDE_W));
          asm volatile ("vlds  v1, 0(%0), %1" : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));
          asm volatile ("vsts  v1, 0(%0), %1" : : "r" (&x[ix]), "r" (inc_x2 << STRIDE_W));
          asm volatile ("vsts  v0, 0(%0), %1" : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));

          asm volatile ("vlds  v0, " STRIDE_O "(%0), %1" : : "r" (&x[ix]), "r" (inc_x2 << STRIDE_W));
          asm volatile ("vlds  v1, " STRIDE_O "(%0), %1" : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));
          asm volatile ("vsts  v1, " STRIDE_O "(%0), %1" : : "r" (&x[ix]), "r" (inc_x2 << STRIDE_W));
          asm volatile ("vsts  v0, " STRIDE_O "(%0), %1" : : "r" (&y[iy]), "r" (inc_y2 << STRIDE_W));


          i = i + vl;
          ix = ix + vl * inc_x2;
          iy = iy + vl * inc_y2;
	}
	return(0);

}


