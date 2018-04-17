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
* 	 CTEST			: NoTest
* 	 TEST			: NoTest
*
**************************************************************************************/

#include "common.h"
#include <math.h>
#include "rvv.h"

#if defined(DOUBLE)
#define ABS fabs
#define STRIDE_W 3
#else
#define ABS fabsf
#define STRIDE_W 2
#endif


FLOAT CNAME(BLASLONG n, FLOAT *x, BLASLONG inc_x)
{
	BLASLONG i=0;
	BLASLONG ix=0;
	FLOAT maxf=0.0;

	if (n <= 0 || inc_x <= 0) return(maxf);
        resetvcfg();
#if defined(DOUBLE)
        setvcfg0(VFP64,    // *x
                 SFP64,    // init min
                 VFP64,    // *acc
                 SFP64);
#else
        setvcfg0(VFP32,    // *x
                 SFP32,    // init min
                 VFP32,    // *acc
                 SFP32);
#endif
        int vl = 0;
        int ct = 0;
        setvl(vl, n);
        while (vl > 1)
          {
            ct++;
            vl = vl >> 1;
          }
        vl = 1 << ct;
        setvl(vl, vl);
        asm volatile ("vinsert v1, %0, x0" : : "r" (x[0]));
        asm volatile ("vsgnj   v2, v1, v1");   // v2 =0
	while(i < n)
	{
          while (n - i < vl)
            {
              asm volatile ("vslide v0, v2, %0" : : "r" (vl >> 1));
              setvl(vl, vl >> 1);
              asm volatile ("vmin   v2, v2, v0");
            }
          asm volatile ("vlds   v0, 0(%0), %1" : : "r" (&x[ix]), "r" (inc_x << STRIDE_W));
          asm volatile ("vsgnjx v0, v0, v0");
          asm volatile ("vmin   v2, v0, v2"); // acc[] = x[]*y[]
          i = i + vl;
          ix = ix + vl * inc_x;
	}
        while (vl > 1)
          {
            asm volatile ("vslide v0, v2, %0" : : "r" (vl >> 1));
            setvl(vl, vl >> 1);
            asm volatile ("vmin   v2, v2, v0");
          }
        asm volatile ("vst      v2, 0(%0)" : : "r" (&maxf));
	return(maxf);

}
