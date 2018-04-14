#include "rvv.h"

size_t c_setvl(int rs) {
  size_t out = 0;
  asm volatile("csrw vl, %0" : : "r" (rs));
  asm volatile("csrr %0, vl" : "=r" (out));
  return out;
}
