#include "common.h"
int CNAME(BLASLONG bm,BLASLONG bn,BLASLONG bk,FLOAT alpha,FLOAT* ba,FLOAT* bb,FLOAT* C,BLASLONG ldc
#ifdef TRMMKERNEL
		,BLASLONG offset
#endif
		)
{
   BLASLONG i,j,k;
   FLOAT *C0,*C1,*ptrba,*ptrbb;
   FLOAT res0,res1,res2,res3,load0,load1,load2,load3,load4,load5,load6,load7;
   int vl, stvl;
   resetvcfg();

#if defined(DOUBLE)
   setvcfg0(VFP64, // load0
            VFP64, // load1
            VFP64, // load2
            VFP64);// load3
   setvcfg2(VFP64, // res0
            VFP64, // res1
            VFP64, // res2
            VFP64);// res3
#else
   setvcfg0(VFP32, // load0
            VFP32, // load1
            VFP32, // load2
            VFP32);// load3
   setvcfg2(VFP32, // res0
            VFP32, // res1
            VFP32, // res2
            VFP32);// res3
#endif
   setvl(vl, bk);
   log2floor(stvl, vl);
   setvl(vl, stvl);
   for (j=0; j<bn/2; j+=1)
     {
        C0 = C;
        C1 = C0+ldc;
        ptrba = ba;
        for (i=0; i<bm/2; i+=1)
          {
             ptrbb = bb;
             setvl(vl, stvl);
             asm volatile("vsne v4, v4, v4");
             asm volatile("vsne v5, v5, v5");
             asm volatile("vsne v6, v6, v6");
             asm volatile("vsne v7, v7, v7");
             k = 0;

             while (k<bk)
               {
                 while (bk - k < vl)
                   {
                     asm volatile ("vslide v0, v4, %0" : : "r" (vl >> 1));
                     asm volatile ("vslide v1, v5, %0" : : "r" (vl >> 1));
                     asm volatile ("vslide v2, v6, %0" : : "r" (vl >> 1));
                     asm volatile ("vslide v3, v7, %0" : : "r" (vl >> 1));
                     setvl(vl, vl >> 1);
                     asm volatile ("vadd   v4, v4, v0");
                     asm volatile ("vadd   v5, v5, v1");
                     asm volatile ("vadd   v6, v6, v2");
                     asm volatile ("vadd   v7, v7, v3");
                   }

                  asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                  asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));
                  asm volatile ("vmadd v4, v0, v1, v4");
                  asm volatile ("vlds  v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
                  asm volatile ("vmadd v5, v2, v1, v5");
                  asm volatile ("vlds  v3, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));
                  asm volatile ("vmadd v6, v0, v3, v6");
                  asm volatile ("vmadd v7, v2, v3, v7");
                  k += vl;
                  ptrba = ptrba + vl * 2;
                  ptrbb = ptrbb + vl * 2;
               }

             while (vl > 1)
               {
                 asm volatile ("vslide v0, v4, %0" : : "r" (vl >> 1));
                 asm volatile ("vslide v1, v5, %0" : : "r" (vl >> 1));
                 asm volatile ("vslide v2, v6, %0" : : "r" (vl >> 1));
                 asm volatile ("vslide v3, v7, %0" : : "r" (vl >> 1));
                 setvl(vl, vl >> 1);
                 asm volatile ("vadd   v4, v4, v0");
                 asm volatile ("vadd   v5, v5, v1");
                 asm volatile ("vadd   v6, v6, v2");
                 asm volatile ("vadd   v7, v7, v3");
               }

             asm volatile ("vextract %0, v4, x0" : "=r" (res0));
             asm volatile ("vextract %0, v5, x0" : "=r" (res1));
             asm volatile ("vextract %0, v6, x0" : "=r" (res2));
             asm volatile ("vextract %0, v7, x0" : "=r" (res3));

             C0[0] = C0[0]+res0*alpha;
             C0[1] = C0[1]+res1*alpha;
             C1[0] = C1[0]+res2*alpha;
             C1[1] = C1[1]+res3*alpha;
             C0 = C0+2;
             C1 = C1+2;
          }
        for (i=0; i<(bm&1); i+=1)
          {
             ptrbb = bb;
             setvl(vl, stvl);
             asm volatile("vsne v4, v4, v4");
             asm volatile("vsne v5, v5, v5");
             k = 0;
             while (k<bk)
               {
                 while (bk - k < vl)
                   {
                     asm volatile ("vslide v0, v4, %0" : : "r" (vl >> 1));
                     asm volatile ("vslide v1, v5, %0" : : "r" (vl >> 1));
                     setvl(vl, vl >> 1);
                     asm volatile ("vadd   v4, v4, v0");
                     asm volatile ("vadd   v5, v5, v1");
                   }

                  asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (1 << STRIDE_W));
                  asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));
                  asm volatile ("vmadd v4, v0, v1, v4");
                  asm volatile ("vlds  v2, " STRIDE_O "(%0), %1" : : "r" (ptrbb), "r" (2 << STRIDE_W));
                  asm volatile ("vmadd v5, v0, v2, v5");
                  k += vl;
                  ptrba = ptrba + vl;
                  ptrbb = ptrbb + vl * 2;
               }
             while (vl > 1)
               {
                 asm volatile ("vslide v0, v4, %0" : : "r" (vl >> 1));
                 asm volatile ("vslide v1, v5, %0" : : "r" (vl >> 1));
                 setvl(vl, vl >> 1);
                 asm volatile ("vadd   v4, v4, v0");
                 asm volatile ("vadd   v5, v5, v1");
               }
             asm volatile ("vextract %0, v4, x0" : "=r" (res0));
             asm volatile ("vextract %0, v5, x0" : "=r" (res1));
             C0[0] = C0[0]+res0*alpha;
             C1[0] = C1[0]+res1*alpha;
             C0 = C0+1;
             C1 = C1+1;
          }
        k = (bk<<1);
        bb = bb+k;
        i = (ldc<<1);
        C = C+i;
     }
   for (j=0; j<(bn&1); j+=1)
     {
       C0 = C;
       ptrba = ba;
       for (i=0; i<bm/2; i+=1)
         {
           ptrbb = bb;
           setvl(vl, stvl);
           asm volatile("vsne v4, v4, v4");
           asm volatile("vsne v5, v5, v5");
           k = 0;
           while (k<bk)
             {
               while (bk - k < vl)
                 {
                   asm volatile ("vslide v0, v4, %0" : : "r" (vl >> 1));
                   asm volatile ("vslide v1, v5, %0" : : "r" (vl >> 1));
                   setvl(vl, vl >> 1);
                   asm volatile ("vadd   v4, v4, v0");
                   asm volatile ("vadd   v5, v5, v1");
                 }
               
               asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
               asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (1 << STRIDE_W));
               asm volatile ("vmadd v4, v0, v1, v4");
               asm volatile ("vlds  v2, " STRIDE_O "(%0), %1" : : "r" (ptrba), "r" (2 << STRIDE_W));
               asm volatile ("vmadd v5, v2, v1, v5");
               k += vl;
               ptrba = ptrba + vl * 2;
               ptrbb = ptrbb + vl;
             }
           while (vl > 1)
             {
               asm volatile ("vslide v0, v4, %0" : : "r" (vl >> 1));
               asm volatile ("vslide v1, v5, %0" : : "r" (vl >> 1));
               setvl(vl, vl >> 1);
               asm volatile ("vadd   v4, v4, v0");
               asm volatile ("vadd   v5, v5, v1");
             }
           asm volatile ("vextract %0, v4, x0" : "=r" (res0));
           asm volatile ("vextract %0, v5, x0" : "=r" (res1));
           
           C0[0] = C0[0]+res0*alpha;
           C0[1] = C0[1]+res1*alpha;
           C0 = C0+2;
         }
      
        for (i=0; i<(bm&1); i+=1)
          {
             ptrbb = bb;
             setvl(vl, stvl);
             asm volatile("vsne v4, v4, v4");
             k = 0;
             while (k<bk)
               {
                 while (bk - k < vl)
                   {
                     asm volatile ("vslide v0, v4, %0" : : "r" (vl >> 1));
                     setvl(vl, vl >> 1);
                     asm volatile ("vadd   v4, v4, v0");
                   }

                  asm volatile ("vlds  v0, 0(%0), %1" : : "r" (ptrba), "r" (1 << STRIDE_W));
                  asm volatile ("vlds  v1, 0(%0), %1" : : "r" (ptrbb), "r" (1 << STRIDE_W));
                  asm volatile ("vmadd v4, v0, v1, v4");
                  k += vl;
                  ptrba = ptrba + vl;
                  ptrbb = ptrbb + vl;
               }
             while (vl > 1)
               {
                 asm volatile ("vslide v0, v4, %0" : : "r" (vl >> 1));
                 setvl(vl, vl >> 1);
                 asm volatile ("vadd   v4, v4, v0");
               }
             asm volatile ("vextract %0, v4, x0" : "=r" (res0));
             C0[0] = C0[0]+res0*alpha;
             C0 = C0+1;
          }
        k = (bk<<0);
        bb = bb+k;
        C = C+ldc;
     }
   return 0;
}
