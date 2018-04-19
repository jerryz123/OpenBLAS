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
   for (j=0; j<bn/2; j+=1)
     {
        C0 = C;
        C1 = C0+ldc;
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
                  load0 = ptrba[2*0+0];
                  load1 = ptrbb[2*0+0];
                  load2 = ptrba[2*0+1];
                  load3 = ptrbb[2*0+1];

                  res0 = res0+load0*load1;
                  res1 = res1+load2*load1;
                  res2 = res2+load0*load3;
                  res3 = res3+load2*load3;
                  ptrba = ptrba+2;
                  ptrbb = ptrbb+2;
               }
             res0 = res0*alpha;
             res1 = res1*alpha;
             res2 = res2*alpha;
             res3 = res3*alpha;
             C0[0] = C0[0]+res0;
             C0[1] = C0[1]+res1;
             C1[0] = C1[0]+res2;
             C1[1] = C1[1]+res3;
             C0 = C0+2;
             C1 = C1+2;
          }
        for (i=0; i<(bm&1); i+=1)
          {
             ptrbb = bb;
             res0 = 0;
             res1 = 0;
             for (k=0; k<bk; k+=1)
               {
                  load0 = ptrba[0+0];
                  load1 = ptrbb[2*0+0];
                  load2 = ptrbb[2*0+1];

                  res0 = res0+load0*load1;
                  res1 = res1+load0*load2;
                  ptrba = ptrba+1;
                  ptrbb = ptrbb+2;
               }
             res0 = res0*alpha;
             res1 = res1*alpha;
             C0[0] = C0[0]+res0;
             C1[0] = C1[0]+res1;
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
             res0 = 0;
             res1 = 0;
             for (k=0; k<bk; k+=1)
               {
                  load0 = ptrba[2*0+0];
                  load1 = ptrbb[0+0];
                  res0 = res0+load0*load1;
                  load2 = ptrba[2*0+1];
                  res1 = res1+load2*load1;
                  ptrba = ptrba+2;
                  ptrbb = ptrbb+1;
               }
             res0 = res0*alpha;
             C0[0] = C0[0]+res0;
             res1 = res1*alpha;
             C0[1] = C0[1]+res1;
             C0 = C0+2;
          }
        for (i=0; i<(bm&1); i+=1)
          {
             ptrbb = bb;
             res0 = 0;
             for (k=0; k<bk; k+=1)
               {
                  load0 = ptrba[0+0];
                  load1 = ptrbb[0+0];
                  res0 = res0+load0*load1;
                  ptrba = ptrba+1;
                  ptrbb = ptrbb+1;
               }
             res0 = res0*alpha;
             C0[0] = C0[0]+res0;
             C0 = C0+1;
          }
        k = (bk<<0);
        bb = bb+k;
        C = C+ldc;
     }
   return 0;
}
