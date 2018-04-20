/*****************************************************************************
Copyright (c) 2011-2014, The OpenBLAS Project
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
      derived from this software without specific prior written 
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********************************************************************************/

/*********************************************************************/
/* Copyright 2009, 2010 The University of Texas at Austin.           */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/*   1. Redistributions of source code must retain the above         */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer.                                                  */
/*                                                                   */
/*   2. Redistributions in binary form must reproduce the above      */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer in the documentation and/or other materials       */
/*      provided with the distribution.                              */
/*                                                                   */
/*    THIS  SOFTWARE IS PROVIDED  BY THE  UNIVERSITY OF  TEXAS AT    */
/*    AUSTIN  ``AS IS''  AND ANY  EXPRESS OR  IMPLIED WARRANTIES,    */
/*    INCLUDING, BUT  NOT LIMITED  TO, THE IMPLIED  WARRANTIES OF    */
/*    MERCHANTABILITY  AND FITNESS FOR  A PARTICULAR  PURPOSE ARE    */
/*    DISCLAIMED.  IN  NO EVENT SHALL THE UNIVERSITY  OF TEXAS AT    */
/*    AUSTIN OR CONTRIBUTORS BE  LIABLE FOR ANY DIRECT, INDIRECT,    */
/*    INCIDENTAL,  SPECIAL, EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES    */
/*    (INCLUDING, BUT  NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE    */
/*    GOODS  OR  SERVICES; LOSS  OF  USE,  DATA,  OR PROFITS;  OR    */
/*    BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON ANY THEORY OF    */
/*    LIABILITY, WHETHER  IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*    (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY OUT    */
/*    OF  THE  USE OF  THIS  SOFTWARE,  EVEN  IF ADVISED  OF  THE    */
/*    POSSIBILITY OF SUCH DAMAGE.                                    */
/*                                                                   */
/* The views and conclusions contained in the software and           */
/* documentation are those of the authors and should not be          */
/* interpreted as representing official policies, either expressed   */
/* or implied, of The University of Texas at Austin.                 */
/*********************************************************************/

#ifndef COMMON_RISCV64
#define COMMON_RISCV64

#define MB  __sync_synchronize()
#define WMB __sync_synchronize()

#define INLINE inline

#ifndef ASSEMBLER


static inline int blas_quickdivide(blasint x, blasint y){
  return x / y;
}

#endif
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
#define STRIDE_O2 "16"
#define STRIDE_O3 "24"
#else
#define ABS fabsf
#define STRIDE_W 2
#define STRIDE_O "4"
#define STRIDE_O2 "8"
#define STRIDE_O3 "12"
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

#define setvcfg6(vtype0, vtype1, vtype2, vtype3)                        \
  asm volatile ("csrw vcfg6, %0" : : "r" (((vtype0) | ((vtype1) << 16) | ((vtype2) << 32) | ((vtype3) << 48)))) \

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


#define log2floor(rd, rs) \
  int _temp0 = rs; \
  int _temp1 = 0;\
  while (_temp0 >>= 1) ++_temp1;\
  rd = 1 << _temp1; \

#define BUFFER_SIZE     ( 32 << 20)
#define SEEK_ADDRESS

#endif
