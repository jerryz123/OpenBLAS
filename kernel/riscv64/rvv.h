#ifndef __RVV_H
#define __RVV_H
#include <stdint.h>

// vtypes
#define INT     (0 << 6) // integer
#define UINT    (1 << 6) // unsigned integer
#define FP      (3 << 6) // floating point

#define SCALAR  (0 << 11)
#define VECTOR  (4 << 11)

#define W128    48 // 128 bits
#define W64     32 // 64 bits
#define W32     24 // 32 bits
#define W16     16 // 16 bits
#define W8      8  // 8 bits

#define VUINT64 ((int64_t)0x2020)
#define VINT64 ((int64_t)0x2060)
#define VFP64 ((int64_t)0x20e0)
#define SUINT64 ((int64_t)0x0020)
#define SINT64 ((int64_t)0x0060)
#define SFP64 ((int64_t)0x00e0)
#define VUINT32 ((int64_t)0x2018)
#define VINT32 ((int64_t)0x2058)
#define VFP32 ((int64_t)0x20d8)
#define SUINT32 ((int64_t)0x0018)
#define SINT32 ((int64_t)0x0058)
#define SFP32 ((int64_t)0x00d8)
#define VUINT16 ((int64_t)0x2010)
#define VINT16 ((int64_t)0x2050)
#define VFP16 ((int64_t)0x20d0)
#define SUINT16 ((int64_t)0x0010)
#define SINT16 ((int64_t)0x0050)
#define SFP16 ((int64_t)0x00d0)

#if defined(DOUBLE)
#define ABS fabs
#define STRIDE_W 3
#define STRIDE_O "8"
#else
#define ABS fabsf
#define STRIDE_W 2
#define STRIDE_O "4"
#endif
/* #define setvcfg(vcfg, vtype0, vtype1, vtype2, vtype3) \ */
/*   li t0, ((vtype0) | ((vtype1) << 16) | ((vtype2) << 32) | ((vtype3) << 48)) ; \ */
/*   csrw vcfg, t0 */

#define setvcfg0(vtype0, vtype1, vtype2, vtype3) \
  asm volatile ("csrw vcfg0, %0" : : "r" (((vtype0) | ((vtype1) << 16) | ((vtype2) << 32) | ((vtype3) << 48)))) \

#define setvcfg2(vtype0, vtype1, vtype2, vtype3) \
  asm volatile ("csrw vcfg2, %0" : : "r" (((vtype0) | ((vtype1) << 16) | ((vtype2) << 32) | ((vtype3) << 48)))) \

#define setvcfg4(vtype0, vtype1, vtype2, vtype3)                        \
  asm volatile ("csrw vcfg4, %0" : : "r" (((vtype0) | ((vtype1) << 16) | ((vtype2) << 32) | ((vtype3) << 48)))) \

#define resetvcfg() \
  asm volatile ("csrw vcfg0, x0"); \
  asm volatile ("csrw vcfg2, x0"); \
  asm volatile ("csrw vcfg4, x0"); \
  asm volatile ("csrw vcfg6, x0"); \
  asm volatile ("csrw vcfg8, x0"); \
  asm volatile ("csrw vcfg10, x0"); \
  asm volatile ("csrw vcfg12, x0"); \
  asm volatile ("csrw vcfg14, x0"); \



#define setvl(rd, rs) \
  asm volatile("csrw vl, %0": : "r" (rs)); \
  asm volatile("csrr %0, vl": "=r" (rd)); \

#endif // __RVV_H
