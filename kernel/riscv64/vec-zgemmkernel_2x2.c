#include "common.h"
/********************************
  ADD1 a*c
  ADD2 b*c
  ADD3 a*d
  ADD4 b*d
*********************************/
int CNAME(BLASLONG bm,BLASLONG bn,BLASLONG bk,FLOAT alphar,FLOAT alphai,FLOAT* ba,FLOAT* bb,FLOAT* C,BLASLONG ldc
#ifdef	TRMMKERNEL
		, BLASLONG offset
#endif
		)
{
   BLASLONG i,j,k;
   FLOAT *C0,*C1,*ptrba,*ptrbb;
   FLOAT res0,res1,res2,res3,res4,res5,res6,res7,load0,load1,load2,load3,load4,load5,load6,load7,load8,load9,load10,load11,load12,load13,load14,load15;

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
   for (j=0; j<bn/2; j+=1)
     {
        C0 = C;
        C1 = C0+2*ldc;
        ptrba = ba;
        for (i=0; i<bm/2; i+=1)
          {
             ptrbb = bb;
             asm volatile ("vsne v8,  v8,  v8");
             asm volatile ("vsne v9,  v9,  v9");
             asm volatile ("vsne v10, v10, v10");
             asm volatile ("vsne v11, v11, v11");
             asm volatile ("vsne v12, v12, v12");
             asm volatile ("vsne v13, v13, v13");
             asm volatile ("vsne v14, v14, v14");
             asm volatile ("vsne v15, v15, v15");
             res0 = 0;
             res1 = 0;
             res2 = 0;
             res3 = 0;
             res4 = 0;
             res5 = 0;
             res6 = 0;
             res7 = 0;
             k = 0;
             while (k < bk)
               {
                 while (bk - k < vl)
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

             load0 = res0*alphar;
             C0[0] = C0[0]+load0;
             load1 = res1*alphar;
             C0[1] = C0[1]+load1;
             load0 = res1*alphai;
             C0[0] = C0[0]-load0;
             load1 = res0*alphai;
             C0[1] = C0[1]+load1;
             load2 = res2*alphar;
             C0[2] = C0[2]+load2;
             load3 = res3*alphar;
             C0[3] = C0[3]+load3;
             load2 = res3*alphai;
             C0[2] = C0[2]-load2;
             load3 = res2*alphai;
             C0[3] = C0[3]+load3;
             load4 = res4*alphar;
             C1[0] = C1[0]+load4;
             load5 = res5*alphar;
             C1[1] = C1[1]+load5;
             load4 = res5*alphai;
             C1[0] = C1[0]-load4;
             load5 = res4*alphai;
             C1[1] = C1[1]+load5;
             load6 = res6*alphar;
             C1[2] = C1[2]+load6;
             load7 = res7*alphar;
             C1[3] = C1[3]+load7;
             load6 = res7*alphai;
             C1[2] = C1[2]-load6;
             load7 = res6*alphai;
             C1[3] = C1[3]+load7;
             C0 = C0+4;
             C1 = C1+4;
          }
        for (i=0; i<(bm&1); i+=1)
          {
             ptrbb = bb;
             res0 = 0;
             res1 = 0;
             res2 = 0;
             res3 = 0;
             for (k=0; k<bk; k+=1)
               {
#if   defined(NN) || defined(NT) || defined(TN) || defined(TT)
                  load0 = ptrba[2*0+0];
                  load1 = ptrbb[4*0+0];
                  res0 = res0+load0*load1;
                  load2 = ptrba[2*0+1];
                  res1 = res1+load2*load1;
                  load3 = ptrbb[4*0+1];
                  res0 = res0-load2*load3;
                  res1 = res1+load0*load3;
                  load4 = ptrbb[4*0+2];
                  res2 = res2+load0*load4;
                  res3 = res3+load2*load4;
                  load5 = ptrbb[4*0+3];
                  res2 = res2-load2*load5;
                  res3 = res3+load0*load5;
#endif
#if   defined(NR) || defined(NC) || defined(TR) || defined(TC)
				  load0 = ptrba[2*0+0];
                  load1 = ptrbb[4*0+0];
                  res0 = res0+load0*load1;
                  load2 = ptrba[2*0+1];
                  res1 = res1+load2*load1;
                  load3 = ptrbb[4*0+1];
                  res0 = res0+load2*load3;
                  res1 = res1-load0*load3;
                  load4 = ptrbb[4*0+2];
                  res2 = res2+load0*load4;
                  res3 = res3+load2*load4;
                  load5 = ptrbb[4*0+3];
                  res2 = res2+load2*load5;
                  res3 = res3-load0*load5;
#endif
#if   defined(RN) || defined(RT) || defined(CN) || defined(CT)
                  load0 = ptrba[2*0+0];
                  load1 = ptrbb[4*0+0];
                  res0 = res0+load0*load1;
                  load2 = ptrba[2*0+1];
                  res1 = res1-load2*load1;
                  load3 = ptrbb[4*0+1];
                  res0 = res0+load2*load3;
                  res1 = res1+load0*load3;
                  load4 = ptrbb[4*0+2];
                  res2 = res2+load0*load4;
                  res3 = res3-load2*load4;
                  load5 = ptrbb[4*0+3];
                  res2 = res2+load2*load5;
                  res3 = res3+load0*load5;
#endif
#if   defined(RR) || defined(RC) || defined(CR) || defined(CC)
                  load0 = ptrba[2*0+0];
                  load1 = ptrbb[4*0+0];
                  res0 = res0+load0*load1;
                  load2 = ptrba[2*0+1];
                  res1 = res1-load2*load1;
                  load3 = ptrbb[4*0+1];
                  res0 = res0-load2*load3;
                  res1 = res1-load0*load3;
                  load4 = ptrbb[4*0+2];
                  res2 = res2+load0*load4;
                  res3 = res3-load2*load4;
                  load5 = ptrbb[4*0+3];
                  res2 = res2-load2*load5;
                  res3 = res3-load0*load5;
#endif
                  ptrba = ptrba+2;
                  ptrbb = ptrbb+4;
               }
             load0 = res0*alphar;
             C0[0] = C0[0]+load0;
             load1 = res1*alphar;
             C0[1] = C0[1]+load1;
             load0 = res1*alphai;
             C0[0] = C0[0]-load0;
             load1 = res0*alphai;
             C0[1] = C0[1]+load1;
             load2 = res2*alphar;
             C1[0] = C1[0]+load2;
             load3 = res3*alphar;
             C1[1] = C1[1]+load3;
             load2 = res3*alphai;
             C1[0] = C1[0]-load2;
             load3 = res2*alphai;
             C1[1] = C1[1]+load3;
             C0 = C0+2;
             C1 = C1+2;
          }
        k = (bk<<2);
        bb = bb+k;
        i = (ldc<<2);
        C = C+i;
     }
   for (j=0; j<(bn&1); j+=1)
     {
        C0 = C;
        ptrba = ba;
        for (i=0; i<bm/2; i+=1)
          {
             ptrbb = bb;
             res0 = 0;
             res1 = 0;
             res2 = 0;
             res3 = 0;
             for (k=0; k<bk; k+=1)
               {
#if   defined(NN) || defined(NT) || defined(TN) || defined(TT)
                  load0 = ptrba[4*0+0];
                  load1 = ptrbb[2*0+0];
                  res0 = res0+load0*load1;
                  load2 = ptrba[4*0+1];
                  res1 = res1+load2*load1;
                  load3 = ptrbb[2*0+1];
                  res0 = res0-load2*load3;
                  res1 = res1+load0*load3;
                  load4 = ptrba[4*0+2];
                  res2 = res2+load4*load1;
                  load5 = ptrba[4*0+3];
                  res3 = res3+load5*load1;
                  res2 = res2-load5*load3;
                  res3 = res3+load4*load3;
#endif
#if   defined(NR) || defined(NC) || defined(TR) || defined(TC)
                  load0 = ptrba[4*0+0];
                  load1 = ptrbb[2*0+0];
                  res0 = res0+load0*load1;
                  load2 = ptrba[4*0+1];
                  res1 = res1+load2*load1;
                  load3 = ptrbb[2*0+1];
                  res0 = res0+load2*load3;
                  res1 = res1-load0*load3;
                  load4 = ptrba[4*0+2];
                  res2 = res2+load4*load1;
                  load5 = ptrba[4*0+3];
                  res3 = res3+load5*load1;
                  res2 = res2+load5*load3;
                  res3 = res3-load4*load3;
#endif
#if   defined(RN) || defined(RT) || defined(CN) || defined(CT)
                  load0 = ptrba[4*0+0];
                  load1 = ptrbb[2*0+0];
                  res0 = res0+load0*load1;
                  load2 = ptrba[4*0+1];
                  res1 = res1-load2*load1;
                  load3 = ptrbb[2*0+1];
                  res0 = res0+load2*load3;
                  res1 = res1+load0*load3;
                  load4 = ptrba[4*0+2];
                  res2 = res2+load4*load1;
                  load5 = ptrba[4*0+3];
                  res3 = res3-load5*load1;
                  res2 = res2+load5*load3;
                  res3 = res3+load4*load3;
#endif
#if   defined(RR) || defined(RC) || defined(CR) || defined(CC)
                  load0 = ptrba[4*0+0];
                  load1 = ptrbb[2*0+0];
                  res0 = res0+load0*load1;
                  load2 = ptrba[4*0+1];
                  res1 = res1-load2*load1;
                  load3 = ptrbb[2*0+1];
                  res0 = res0-load2*load3;
                  res1 = res1-load0*load3;
                  load4 = ptrba[4*0+2];
                  res2 = res2+load4*load1;
                  load5 = ptrba[4*0+3];
                  res3 = res3-load5*load1;
                  res2 = res2-load5*load3;
                  res3 = res3-load4*load3;
#endif
                  ptrba = ptrba+4;
                  ptrbb = ptrbb+2;
               }
             load0 = res0*alphar;
             C0[0] = C0[0]+load0;
             load1 = res1*alphar;
             C0[1] = C0[1]+load1;
             load0 = res1*alphai;
             C0[0] = C0[0]-load0;
             load1 = res0*alphai;
             C0[1] = C0[1]+load1;
             load2 = res2*alphar;
             C0[2] = C0[2]+load2;
             load3 = res3*alphar;
             C0[3] = C0[3]+load3;
             load2 = res3*alphai;
             C0[2] = C0[2]-load2;
             load3 = res2*alphai;
             C0[3] = C0[3]+load3;
             C0 = C0+4;
          }
        for (i=0; i<(bm&1); i+=1)
          {
             ptrbb = bb;
             res0 = 0;
             res1 = 0;
             for (k=0; k<bk; k+=1)
               {
#if   defined(NN) || defined(NT) || defined(TN) || defined(TT)
                  load0 = ptrba[2*0+0];
                  load1 = ptrbb[2*0+0];
                  res0 = res0+load0*load1;
                  load2 = ptrba[2*0+1];
                  res1 = res1+load2*load1;
                  load3 = ptrbb[2*0+1];
                  res0 = res0-load2*load3;
                  res1 = res1+load0*load3;
#endif
#if   defined(NR) || defined(NC) || defined(TR) || defined(TC)
                  load0 = ptrba[2*0+0];
                  load1 = ptrbb[2*0+0];
                  res0 = res0+load0*load1;
                  load2 = ptrba[2*0+1];
                  res1 = res1+load2*load1;
                  load3 = ptrbb[2*0+1];
                  res0 = res0+load2*load3;
                  res1 = res1-load0*load3;
#endif
#if   defined(RN) || defined(RT) || defined(CN) || defined(CT)
                  load0 = ptrba[2*0+0];
                  load1 = ptrbb[2*0+0];
                  res0 = res0+load0*load1;
                  load2 = ptrba[2*0+1];
                  res1 = res1-load2*load1;
                  load3 = ptrbb[2*0+1];
                  res0 = res0+load2*load3;
                  res1 = res1+load0*load3;
#endif
#if   defined(RR) || defined(RC) || defined(CR) || defined(CC)
                  load0 = ptrba[2*0+0];
                  load1 = ptrbb[2*0+0];
                  res0 = res0+load0*load1;
                  load2 = ptrba[2*0+1];
                  res1 = res1-load2*load1;
                  load3 = ptrbb[2*0+1];
                  res0 = res0-load2*load3;
                  res1 = res1-load0*load3;
#endif
                  ptrba = ptrba+2;
                  ptrbb = ptrbb+2;
               }
             load0 = res0*alphar;
             C0[0] = C0[0]+load0;
             load1 = res1*alphar;
             C0[1] = C0[1]+load1;
             load0 = res1*alphai;
             C0[0] = C0[0]-load0;
             load1 = res0*alphai;
             C0[1] = C0[1]+load1;
             C0 = C0+2;
          }
        k = (bk<<1);
        bb = bb+k;
        i = (ldc<<1);
        C = C+i;
     }
   return 0;
}
