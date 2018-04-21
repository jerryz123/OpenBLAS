#include "common.h"
/********************************
  ADD1 a*c
  ADD2 b*c
  ADD3 a*d
  ADD4 b*d
 *********************************/
int CNAME(BLASLONG bm,BLASLONG bn,BLASLONG bk,FLOAT alphar,FLOAT alphai,FLOAT* ba,FLOAT* bb,
		FLOAT* C,BLASLONG ldc, BLASLONG offset)
{
	BLASLONG i,j,k;
	FLOAT *C0,*C1,*ptrba,*ptrbb;
	FLOAT res0,res1,res2,res3,res4,res5,res6,res7,load0,load1,load2,load3,load4,load5,load6,load7,load8,load9,load10,load11,load12,load13,load14,load15;
	BLASLONG off, temp;
        resetvcfg();
#if defined(DOUbLE)
        setvcfg0(VFP64, // load0
                 VFP64, // load1
                 VFP64, // load2
                 VFP64);// load3
        setvcfg2(VFP64, // load4
                 VFP64, // load5
                 VFP64, // load6
                 VFP64);// load7
        setvcfg4(VFP64, // res0
                 VFP64, // res1
                 VFP64, // res2
                 VFP64);// res3
        setvcfg6(VFP64, // res4
                 VFP64, // res5
                 VFP64, // res6
                 VFP64);// res7
#else
        setvcfg0(VFP32, // load0
                 VFP32, // load1
                 VFP32, // load2
                 VFP32);// load3
        setvcfg2(VFP32, // load4
                 VFP32, // load5
                 VFP32, // load6
                 VFP32);// load7
        setvcfg4(VFP32, // res0
                 VFP32, // res1
                 VFP32, // res2
                 VFP32);// res3
        setvcfg6(VFP32, // res4
                 VFP32, // res5
                 VFP32, // res6
                 VFP32);// res7
#endif
        int vl;
        int stvl;
        setvl(vl, bk);
        log2floor(stvl, vl);
        setvl(vl, stvl);
#if defined(TRMMKERNEL) && !defined(LEFT)
	off = -offset;
#endif
	for (j=0; j<bn/2; j+=1)
	{
#if defined(TRMMKERNEL) &&  defined(LEFT)
		off = offset;
#endif
		C0 = C;
		C1 = C0+2*ldc;
		ptrba = ba;
		for (i=0; i<bm/2; i+=1)
		{
#if (defined(LEFT) &&  defined(TRANSA)) || (!defined(LEFT) && !defined(TRANSA))
                  ptrbb = bb;
#else
                  ptrba += off*2*2;
                  ptrbb = bb+off*2*2;
#endif
                  res0 = 0;
                  res1 = 0;
                  res2 = 0;
                  res3 = 0;
                  res4 = 0;
                  res5 = 0;
                  res6 = 0;
                  res7 = 0;
#if (defined(LEFT) && !defined(TRANSA)) || (!defined(LEFT) && defined(TRANSA))
                  temp = bk - off;
#elif defined(LEFT)
                  temp = off + 2;
#else
                  temp = off + 2;
#endif
                  asm volatile ("vsne v8,  v8,  v8");
                  asm volatile ("vsne v9,  v9,  v9");
                  asm volatile ("vsne v10, v10, v10");
                  asm volatile ("vsne v11, v11, v11");
                  asm volatile ("vsne v12, v12, v12");
                  asm volatile ("vsne v13, v13, v13");
                  asm volatile ("vsne v14, v14, v14");
                  asm volatile ("vsne v15, v15, v15");

                  k = 0;
                  while (k < temp)
                    {
                      while (temp - k < vl)
                        {
                          asm volatile ("vslide v0, v8,  %0" : : "r" (vl >> 1));
                          asm volatile ("vslide v1, v9,  %0" : : "r" (vl >> 1));
                          asm volatile ("vslide v2, v10, %0" : : "r" (vl >> 1));
                          asm volatile ("vslide v3, v11, %0" : : "r" (vl >> 1));
                          asm volatile ("vslide v4, v12, %0" : : "r" (vl >> 1));
                          asm volatile ("vslide v5, v13, %0" : : "r" (vl >> 1));
                          asm volatile ("vslide v6, v14, %0" : : "r" (vl >> 1));
                          asm volatile ("vslide v7, v15, %0" : : "r" (vl >> 1));
                          setvl(vl, vl >> 1);
                          asm volatile ("vadd   v8,  v8,  v0");
                          asm volatile ("vadd   v9,  v9,  v1");
                          asm volatile ("vadd   v10, v10, v2");
                          asm volatile ("vadd   v11, v11, v3");
                          asm volatile ("vadd   v12, v12, v4");
                          asm volatile ("vadd   v13, v13, v5");
                          asm volatile ("vadd   v14, v14, v6");
                          asm volatile ("vadd   v15, v15, v7");
                        }
#if   defined(NN) || defined(NT) || defined(TN) || defined(TT)
                      asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                      asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd v8, v0, v1, v8");
                      asm volatile ("vlds  v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd v9, v2, v1, v9");
                      asm volatile ("vlds  v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vnmsub v8, v2, v3, v8");
                      asm volatile ("vmadd  v9, v0, v3, v9");

                      asm volatile ("vlds  v4, " STRIDE_O2"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd v10, v4, v1, v10");
                      asm volatile ("vlds  v5, " STRIDE_O3"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd  v11, v5, v1, v11");
                      asm volatile ("vnmsub v10, v5, v3, v10");
                      asm volatile ("vmadd  v11, v4, v3, v11");

                      asm volatile ("vlds  v6, " STRIDE_O2"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd v12, v0, v6, v12");
                      asm volatile ("vmadd v13, v2, v6, v13");
                      asm volatile ("vlds  v7, " STRIDE_O3"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vnmsub v12, v2, v7, v12");
                      asm volatile ("vmadd  v13, v0, v7, v13");
                      asm volatile ("vmadd  v14, v4, v6, v14");
                      asm volatile ("vmadd  v15, v5, v6, v15");
                      asm volatile ("vnmsub v14, v5, v7, v14");
                      asm volatile ("vmadd  v15, v4, v7, v15");
#endif
#if   defined(NR) || defined(NC) || defined(TR) || defined(TC)
                      asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                      asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd v8, v0, v1, v8");
                      asm volatile ("vlds  v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd v9, v2, v1, v9");
                      asm volatile ("vlds  v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd  v8, v2, v3, v8");
                      asm volatile ("vnmsub v9, v0, v3, v9");

                      asm volatile ("vlds  v4, " STRIDE_O2"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd v10, v4, v1, v10");
                      asm volatile ("vlds  v5, " STRIDE_O3"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd  v11, v5, v1, v11");
                      asm volatile ("vmadd  v10, v5, v3, v10");
                      asm volatile ("vnmsub v11, v4, v3, v11");

                      asm volatile ("vlds  v6, " STRIDE_O2"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd v12, v0, v6, v12");
                      asm volatile ("vmadd v13, v2, v6, v13");
                      asm volatile ("vlds  v7, " STRIDE_O3"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd  v12, v2, v7, v12");
                      asm volatile ("vnmsub v13, v0, v7, v13");
                      asm volatile ("vmadd  v14, v4, v6, v14");
                      asm volatile ("vmadd  v15, v5, v6, v15");
                      asm volatile ("vmadd  v14, v5, v7, v14");
                      asm volatile ("vnmsub v15, v4, v7, v15");
#endif
#if   defined(RN) || defined(RT) || defined(CN) || defined(CT)
                      asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                      asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd  v8, v0, v1, v8");
                      asm volatile ("vlds   v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                      asm volatile ("vnmsub v9, v2, v1, v9");
                      asm volatile ("vlds   v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd  v8, v2, v3, v8");
                      asm volatile ("vmadd  v9, v0, v3, v9");

                      asm volatile ("vlds  v4, " STRIDE_O2"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd v10, v4, v1, v10");
                      asm volatile ("vlds  v5, " STRIDE_O3"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));

                      asm volatile ("vnmsub  v11, v5, v1, v11");
                      asm volatile ("vmadd   v10, v5, v3, v10");
                      asm volatile ("vmadd   v11, v4, v3, v11");

                      asm volatile ("vlds   v6, " STRIDE_O2"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd  v12, v0, v6, v12");
                      asm volatile ("vnmsub v13, v2, v6, v13");
                      asm volatile ("vlds   v7, " STRIDE_O3"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd  v12, v2, v7, v12");
                      asm volatile ("vmadd  v13, v0, v7, v13");
                      asm volatile ("vmadd  v14, v4, v6, v14");
                      asm volatile ("vnmsub v15, v5, v6, v15");
                      asm volatile ("vmadd  v14, v5, v7, v14");
                      asm volatile ("vmadd  v15, v4, v7, v15");
#endif
#if   defined(RR) || defined(RC) || defined(CR) || defined(CC)
                      asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                      asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd  v8, v0, v1, v8");
                      asm volatile ("vlds   v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                      asm volatile ("vnmsub v9, v2, v1, v9");
                      asm volatile ("vlds   v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vnmsub v8, v2, v3, v8");
                      asm volatile ("vnmsub v9, v0, v3, v9");

                      asm volatile ("vlds  v4, " STRIDE_O2"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd v10, v4, v1, v10");
                      asm volatile ("vlds  v5, " STRIDE_O3"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));

                      asm volatile ("vnmsub  v11, v5, v1, v11");
                      asm volatile ("vnmsub  v10, v5, v3, v10");
                      asm volatile ("vnmsub  v11, v4, v3, v11");

                      asm volatile ("vlds   v6, " STRIDE_O2"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd  v12, v0, v6, v12");
                      asm volatile ("vnmsub v13, v2, v6, v13");
                      asm volatile ("vlds   v7, " STRIDE_O3"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vnmsub v12, v2, v7, v12");
                      asm volatile ("vnmsub v13, v0, v7, v13");
                      asm volatile ("vmadd  v14, v4, v6, v14");
                      asm volatile ("vnmsub v15, v5, v6, v15");
                      asm volatile ("vnmsub v14, v5, v7, v14");
                      asm volatile ("vnmsub v15, v4, v7, v15");
#endif
                      ptrba = ptrba+4*vl;
                      ptrbb = ptrbb+4*vl;
                      k += vl;

                    }
                  while (vl > 1)
                    {
                      asm volatile ("vslide v0, v8,  %0" : : "r" (vl >> 1));
                      asm volatile ("vslide v1, v9,  %0" : : "r" (vl >> 1));
                      asm volatile ("vslide v2, v10, %0" : : "r" (vl >> 1));
                      asm volatile ("vslide v3, v11, %0" : : "r" (vl >> 1));
                      asm volatile ("vslide v4, v12, %0" : : "r" (vl >> 1));
                      asm volatile ("vslide v5, v13, %0" : : "r" (vl >> 1));
                      asm volatile ("vslide v6, v14, %0" : : "r" (vl >> 1));
                      asm volatile ("vslide v7, v15, %0" : : "r" (vl >> 1));
                      setvl(vl, vl >> 1);
                      asm volatile ("vadd   v8,  v8,  v0");
                      asm volatile ("vadd   v9,  v9,  v1");
                      asm volatile ("vadd   v10, v10, v2");
                      asm volatile ("vadd   v11, v11, v3");
                      asm volatile ("vadd   v12, v12, v4");
                      asm volatile ("vadd   v13, v13, v5");
                      asm volatile ("vadd   v14, v14, v6");
                      asm volatile ("vadd   v15, v15, v7");
                    }
                  asm volatile ("vextract %0, v8,  x0" : "=r" (res0));
                  asm volatile ("vextract %0, v9,  x0" : "=r" (res1));
                  asm volatile ("vextract %0, v10, x0" : "=r" (res2));
                  asm volatile ("vextract %0, v11, x0" : "=r" (res3));
                  asm volatile ("vextract %0, v12, x0" : "=r" (res4));
                  asm volatile ("vextract %0, v13, x0" : "=r" (res5));
                  asm volatile ("vextract %0, v14, x0" : "=r" (res6));
                  asm volatile ("vextract %0, v15, x0" : "=r" (res7));
                  load0 = res0*alphar-res1*alphai;
                  load1 = res1*alphar+res0*alphai;
                  C0[0] = load0;
                  C0[1] = load1;

                  load2 = res2*alphar-res3*alphai;
                  load3 = res3*alphar+res2*alphai;
                  C0[2] = load2;
                  C0[3] = load3;

                  load4 = res4*alphar-res5*alphai;
                  load5 = res5*alphar+res4*alphai;
                  C1[0] = load4;
                  C1[1] = load5;

                  load6 = res6*alphar-res7*alphai;
                  load7 = res7*alphar+res6*alphai;
                  C1[2] = load6;
                  C1[3] = load7;
#if ( defined(LEFT) &&  defined(TRANSA)) || \
			(!defined(LEFT) && !defined(TRANSA))
			temp = bk - off;
#ifdef LEFT
			temp -= 2;
#else
			temp -= 2;
#endif
			ptrba += temp*2*2;
			ptrbb += temp*2*2;
#endif
#ifdef LEFT
			off += 2;
#endif

			C0 = C0+4;
			C1 = C1+4;
		}
		for (i=0; i<(bm&1); i+=1)
		{
#if (defined(LEFT) &&  defined(TRANSA)) || (!defined(LEFT) && !defined(TRANSA))
                  ptrbb = bb;
#else
                  ptrba += off*2;
                  ptrbb = bb + off*2*2;
#endif
                  res0 = 0;
                  res1 = 0;
                  res2 = 0;
                  res3 = 0;
#if (defined(LEFT) && !defined(TRANSA)) || (!defined(LEFT) && defined(TRANSA))
                  temp = bk - off;
#elif defined(LEFT)
                  temp = off+1;
#else
                  temp = off+2;
#endif
                  setvl(vl, stvl);
                  asm volatile ("vsne v8,  v8,  v8");
                  asm volatile ("vsne v9,  v9,  v9");
                  asm volatile ("vsne v10, v10, v10");
                  asm volatile ("vsne v11, v11, v11");
                  
                  k = 0;
                  while (k < temp)
                    {
                      while (temp - k < vl)
                        {
                          asm volatile ("vslide v0, v8,  %0" : : "r" (vl >> 1));
                          asm volatile ("vslide v1, v9,  %0" : : "r" (vl >> 1));
                          asm volatile ("vslide v2, v10, %0" : : "r" (vl >> 1));
                          asm volatile ("vslide v3, v11, %0" : : "r" (vl >> 1));
                          setvl(vl, vl >> 1);
                          asm volatile ("vadd   v8,  v8,  v0");
                          asm volatile ("vadd   v9,  v9,  v1");
                          asm volatile ("vadd   v10, v10, v2");
                          asm volatile ("vadd   v11, v11, v3");
                        }
#if   defined(NN) || defined(NT) || defined(TN) || defined(TT)
                      asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                      asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd v8, v0, v1, v8");
                      asm volatile ("vlds  v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                      asm volatile ("vmadd v9, v2, v1, v9");
                      asm volatile ("vlds  v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vnmsub v8, v2, v3, v8");
                      asm volatile ("vmadd  v9, v0, v3, v9");

                      asm volatile ("vlds  v4, " STRIDE_O2"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd v10, v0, v4, v10");
                      asm volatile ("vmadd v11, v2, v4, v11");

                      asm volatile ("vlds  v5, " STRIDE_O3"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));
                      asm volatile ("vnmsub v10, v2, v5, v10");
                      asm volatile ("vmadd  v11, v0, v5, v11");
#endif
#if   defined(NR) || defined(NC) || defined(TR) || defined(TC)
                      asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                      asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd v8, v0, v1, v8");
                      asm volatile ("vlds  v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                      asm volatile ("vmadd v9, v2, v1, v9");
                      asm volatile ("vlds  v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd  v8, v2, v3, v8");
                      asm volatile ("vnmsub v9, v0, v3, v9");

                      asm volatile ("vlds  v4, " STRIDE_O2"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd v10, v0, v4, v10");
                      asm volatile ("vmadd v11, v2, v4, v11");

                      asm volatile ("vlds  v5, " STRIDE_O3"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd  v10, v2, v5, v10");
                      asm volatile ("vnmsub v11, v0, v5, v11");
#endif
#if   defined(RN) || defined(RT) || defined(CN) || defined(CT)
                      asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                      asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd  v8, v0, v1, v8");
                      asm volatile ("vlds   v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                      asm volatile ("vnmsub v9, v2, v1, v9");
                      asm volatile ("vlds   v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd  v8, v2, v3, v8");
                      asm volatile ("vmadd  v9, v0, v3, v9");

                      asm volatile ("vlds  v4, " STRIDE_O2"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd  v10, v0, v4, v10");
                      asm volatile ("vnmsub v11, v2, v4, v11");

                      asm volatile ("vlds  v5, " STRIDE_O3"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd  v10, v2, v5, v10");
                      asm volatile ("vmadd  v11, v0, v5, v11");
#endif
#if   defined(RR) || defined(RC) || defined(CR) || defined(CC)
                      asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                      asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vmadd  v8, v0, v1, v8");
                      asm volatile ("vlds   v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                      asm volatile ("vnmsub v9, v2, v1, v9");
                      asm volatile ("vlds   v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));

                      asm volatile ("vnmsub v8, v2, v3, v8");
                      asm volatile ("vnmsub v9, v0, v3, v9");

                      asm volatile ("vlds  v4, " STRIDE_O2"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));
                      asm volatile ("vmadd  v10, v0, v4, v10");
                      asm volatile ("vnmsub v11, v2, v4, v11");

                      asm volatile ("vlds   v5, " STRIDE_O3"(%0), %1" : : "r" (ptrbb), "r" (4 << STRIDE_W));
                      asm volatile ("vnmsub v10, v2, v5, v10");
                      asm volatile ("vnmsub v11, v0, v5, v11");
#endif
                      ptrba = ptrba+2*vl;
                      ptrbb = ptrbb+4*vl;
                      k += vl;
                    }
                  while (vl > 1)
                    {
                      asm volatile ("vslide v0, v8,  %0" : : "r" (vl >> 1));
                      asm volatile ("vslide v1, v9,  %0" : : "r" (vl >> 1));
                      asm volatile ("vslide v2, v10, %0" : : "r" (vl >> 1));
                      asm volatile ("vslide v3, v11, %0" : : "r" (vl >> 1));
                      setvl(vl, vl >> 1);
                      asm volatile ("vadd   v8,  v8,  v0");
                      asm volatile ("vadd   v9,  v9,  v1");
                      asm volatile ("vadd   v10, v10, v2");
                      asm volatile ("vadd   v11, v11, v3");
                    }
                  asm volatile ("vextract %0, v8,  x0" : "=r" (res0));
                  asm volatile ("vextract %0, v9,  x0" : "=r" (res1));
                  asm volatile ("vextract %0, v10, x0" : "=r" (res2));
                  asm volatile ("vextract %0, v11, x0" : "=r" (res3));
                  
                  load0 = res0*alphar-res1*alphai;
                  load1 = res1*alphar+res0*alphai;
                  C0[0] = load0;
                  C0[1] = load1;

                  load2 = res2*alphar-res3*alphai;
                  load3 = res3*alphar+res2*alphai;
                  C1[0] = load2;
                  C1[1] = load3;
#if ( defined(LEFT) &&  defined(TRANSA)) || \
  (!defined(LEFT) && !defined(TRANSA))
                  temp = bk - off;
#ifdef LEFT
                  temp -= 1;
#else
                  temp -= 2;
#endif
                  ptrba += temp*2;
                  ptrbb += temp*2*2;
#endif
#ifdef LEFT
                  off += 1;
#endif
                  C0 = C0+2;
                  C1 = C1+2;
		}
#if defined(TRMMKERNEL) && !defined(LEFT)
		off += 2;
#endif
		k = (bk<<2);
		bb = bb+k;
		i = (ldc<<2);
		C = C+i;
	}
	for (j=0; j<(bn&1); j+=1)
	{
          C0 = C;
#if defined(TRMMKERNEL) &&  defined(LEFT)
          off = offset;
#endif
          ptrba = ba;
          for (i=0; i<bm/2; i+=1)
            {
#if (defined(LEFT) &&  defined(TRANSA)) || (!defined(LEFT) && !defined(TRANSA))
              ptrbb = bb;
#else
              ptrba += off*2*2;
              ptrbb = bb+off*2;
#endif
              res0 = 0;
              res1 = 0;
              res2 = 0;
              res3 = 0;
#if (defined(LEFT) && !defined(TRANSA)) || (!defined(LEFT) && defined(TRANSA))
              temp = bk - off;
#elif defined(LEFT)
              temp = off + 2;
#else
              temp = off + 1;
#endif
              setvl(vl, stvl);
              asm volatile ("vsne v8,  v8,  v8");
              asm volatile ("vsne v9,  v9,  v9");
              asm volatile ("vsne v10, v10, v10");
              asm volatile ("vsne v11, v11, v11");
              k = 0;
               while (k < temp)
               {
                 while (temp - k < vl)
                   {
                     asm volatile ("vslide v0, v8,  %0" : : "r" (vl >> 1));
                     asm volatile ("vslide v1, v9,  %0" : : "r" (vl >> 1));
                     asm volatile ("vslide v2, v10, %0" : : "r" (vl >> 1));
                     asm volatile ("vslide v3, v11, %0" : : "r" (vl >> 1));
                     setvl(vl, vl >> 1);
                     asm volatile ("vadd   v8,  v8,  v0");
                     asm volatile ("vadd   v9,  v9,  v1");
                     asm volatile ("vadd   v10, v10, v2");
                     asm volatile ("vadd   v11, v11, v3");
                   }
#if   defined(NN) || defined(NT) || defined(TN) || defined(TT)
                 asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vmadd v8, v0, v1, v8");
                 asm volatile ("vlds  v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vmadd v9, v2, v1, v9");
                 asm volatile ("vlds  v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vnmsub v8, v2, v3, v8");
                 asm volatile ("vmadd  v9, v0, v3, v9");

                 asm volatile ("vlds  v4, " STRIDE_O2"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vmadd v10, v4, v1, v10");

                 asm volatile ("vlds  v5, " STRIDE_O3"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vmadd v11, v5, v1, v11");
                 asm volatile ("vnmsub v10, v5, v3, v10");
                 asm volatile ("vmadd  v11, v4, v3, v11");
#endif
#if   defined(NR) || defined(NC) || defined(TR) || defined(TC)
                 asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vmadd v8, v0, v1, v8");
                 asm volatile ("vlds  v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vmadd v9, v2, v1, v9");
                 asm volatile ("vlds  v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vmadd  v8, v2, v3, v8");
                 asm volatile ("vnmsub v9, v0, v3, v9");

                 asm volatile ("vlds  v4, " STRIDE_O2"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vmadd v10, v4, v1, v10");

                 asm volatile ("vlds  v5, " STRIDE_O3"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vmadd  v11, v5, v1, v11");
                 asm volatile ("vmadd  v10, v5, v3, v10");
                 asm volatile ("vnmsub v11, v4, v3, v11");
#endif
#if   defined(RN) || defined(RT) || defined(CN) || defined(CT)
                 asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vmadd  v8, v0, v1, v8");
                 asm volatile ("vlds   v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vnmsub v9, v2, v1, v9");
                 asm volatile ("vlds   v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vmadd  v8, v2, v3, v8");
                 asm volatile ("vmadd  v9, v0, v3, v9");

                 asm volatile ("vlds  v4, " STRIDE_O2"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vmadd v10, v4, v1, v10");

                 asm volatile ("vlds  v5, " STRIDE_O3"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vnmsub v11, v5, v1, v11");
                 asm volatile ("vmadd  v10, v5, v3, v10");
                 asm volatile ("vmadd  v11, v4, v3, v11");
#endif
#if   defined(RR) || defined(RC) || defined(CR) || defined(CC)
                 asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vmadd  v8, v0, v1, v8");
                 asm volatile ("vlds   v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vnmsub v9, v2, v1, v9");
                 asm volatile ("vlds   v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vnmsub  v8, v2, v3, v8");
                 asm volatile ("vnmsub  v9, v0, v3, v9");

                 asm volatile ("vlds  v4, " STRIDE_O2"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vmadd v10, v4, v1, v10");

                 asm volatile ("vlds  v5, " STRIDE_O3"(%0), %1" : : "r" (ptrba), "r" (4 << STRIDE_W));
                 asm volatile ("vnmsub v11, v5, v1, v11");
                 asm volatile ("vnmsub v10, v5, v3, v10");
                 asm volatile ("vnmsub v11, v4, v3, v11");
#endif
                 ptrba = ptrba+4*vl;
                 ptrbb = ptrbb+2*vl;
                 k += vl;
               }
             while (vl > 1)
               {
                 asm volatile ("vslide v0, v8,  %0" : : "r" (vl >> 1));
                 asm volatile ("vslide v1, v9,  %0" : : "r" (vl >> 1));
                 asm volatile ("vslide v2, v10, %0" : : "r" (vl >> 1));
                 asm volatile ("vslide v3, v11, %0" : : "r" (vl >> 1));
                 setvl(vl, vl >> 1);
                 asm volatile ("vadd   v8,  v8,  v0");
                 asm volatile ("vadd   v9,  v9,  v1");
                 asm volatile ("vadd   v10, v10, v2");
                 asm volatile ("vadd   v11, v11, v3");
               }
             asm volatile ("vextract %0, v8,  x0" : "=r" (res0));
             asm volatile ("vextract %0, v9,  x0" : "=r" (res1));
             asm volatile ("vextract %0, v10, x0" : "=r" (res2));
             asm volatile ("vextract %0, v11, x0" : "=r" (res3));
		
             load0 = res0*alphar-res1*alphai;
             load1 = res1*alphar+res0*alphai;
             C0[0] = load0;
             C0[1] = load1;
             
             load2 = res2*alphar-res3*alphai;
             load3 = res3*alphar+res2*alphai;
             C0[2] = load2;
             C0[3] = load3;
#if ( defined(LEFT) &&  defined(TRANSA)) ||                     \
  (!defined(LEFT) && !defined(TRANSA))
             temp = bk-off;
#ifdef LEFT
             temp -= 2;
#else
             temp -= 1;
#endif
             ptrba += temp*2*2;
             ptrbb += temp*2;
#endif
#ifdef LEFT
             off += 2;
#endif
             C0 = C0+4;
            }
          for (i=0; i<(bm&1); i+=1)
            {
#if (defined(LEFT) &&  defined(TRANSA)) || (!defined(LEFT) && !defined(TRANSA))
              ptrbb = bb;
#else
              ptrba += off*2;
              ptrbb = bb + off*2;
#endif
              res0 = 0;
              res1 = 0;
#if (defined(LEFT) && !defined(TRANSA)) || (!defined(LEFT) && defined(TRANSA))
              temp = bk-off;
#elif defined(LEFT)
              temp = off + 1;
#else
              temp = off + 1;
#endif
              setvl(vl, stvl);
              asm volatile ("vsne v8,  v8,  v8");
              asm volatile ("vsne v9,  v9,  v9");
              k = 0;
               while (k < temp)
               {
                 while (temp - k < vl)
                   {
                     asm volatile ("vslide v0, v8,  %0" : : "r" (vl >> 1));
                     asm volatile ("vslide v1, v9,  %0" : : "r" (vl >> 1));
                     setvl(vl, vl >> 1);
                     asm volatile ("vadd   v8,  v8,  v0");
                     asm volatile ("vadd   v9,  v9,  v1");
                   }
#if   defined(NN) || defined(NT) || defined(TN) || defined(TT)
                 asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                 asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vmadd  v8, v0, v1, v8");
                 asm volatile ("vlds   v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                 asm volatile ("vmadd  v9, v2, v1, v9");
                 asm volatile ("vlds   v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vnmsub  v8, v2, v3, v8");
                 asm volatile ("vmadd   v9, v0, v3, v9");
#endif
#if   defined(NR) || defined(NC) || defined(TR) || defined(TC)
                 asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                 asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vmadd  v8, v0, v1, v8");
                 asm volatile ("vlds   v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                 asm volatile ("vmadd  v9, v2, v1, v9");
                 asm volatile ("vlds   v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vmadd  v8, v2, v3, v8");
                 asm volatile ("vnmsub v9, v0, v3, v9");
#endif
#if   defined(RN) || defined(RT) || defined(CN) || defined(CT)
                 asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                 asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vmadd  v8, v0, v1, v8");
                 asm volatile ("vlds   v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                 asm volatile ("vnmsub v9, v2, v1, v9");
                 asm volatile ("vlds   v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vmadd  v8, v2, v3, v8");
                 asm volatile ("vmadd  v9, v0, v3, v9");
#endif
#if   defined(RR) || defined(RC) || defined(CR) || defined(CC)
                 asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                 asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vmadd  v8, v0, v1, v8");
                 asm volatile ("vlds   v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                 asm volatile ("vnmsub v9, v2, v1, v9");
                 asm volatile ("vlds   v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));

                 asm volatile ("vnmsub v8, v2, v3, v8");
                 asm volatile ("vnmsub v9, v0, v3, v9");
#endif
                 ptrba = ptrba+2*vl;
                 ptrbb = ptrbb+2*vl;
                 k += vl;
               }
             while (vl > 1)
               {
                 asm volatile ("vslide v0, v8,  %0" : : "r" (vl >> 1));
                 asm volatile ("vslide v1, v9,  %0" : : "r" (vl >> 1));
                 setvl(vl, vl >> 1);
                 asm volatile ("vadd   v8,  v8,  v0");
                 asm volatile ("vadd   v9,  v9,  v1");
               }
             asm volatile ("vextract %0, v8,  x0" : "=r" (res0));
             asm volatile ("vextract %0, v9,  x0" : "=r" (res1));
             
             load0 = res0*alphar-res1*alphai;
             load1 = res1*alphar+res0*alphai;
             C0[0] = load0;
             C0[1] = load1;
#if ( defined(LEFT) &&  defined(TRANSA)) || \
  (!defined(LEFT) && !defined(TRANSA))
             temp = bk - off;
#ifdef LEFT
             temp -= 1;
#else
             temp -= 1;
#endif
             ptrba += temp*2;
             ptrbb += temp*2;
#endif
#ifdef LEFT
             off += 1;
#endif
             C0 = C0+2;
            }
          k = (bk<<1);
          bb = bb+k;
          i = (ldc<<1);
          C = C+i;
	}
	return 0;
}
