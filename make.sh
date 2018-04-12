make TARGET=RISCV64 \
     ARCH=riscv64 \
     CC=riscv64-unknown-linux-gnu-gcc \
     HOSTCC=gcc \
     USE_THREAD=0 \
     NO_LAPACK=1 \
     NO_LAPACKE=1 \
     FC=riscv64-unknown-linux-gnu-gfortran \
     all ctest

# make PREFIX=riscv-build \
#      TARGET=RISCV64 \
#      ARCH=rv64 \
#      CC=riscv64-unknown-linux-gnu-gcc \
#      HOSTCC=gcc \
#      USE_THREAD=0 \
#      ONLY_CBLAS=1 \
#      NO_LAPACK=1 \
#      NO_LAPACKE=1 \
#      FC=riscv64-unknown-linux-gnu-gfortran \
#     install	
