SRC_TEST = test.cpp

SRC_MAIN = Segs2Polys.cpp
DES_DLL = Segs2Polys.dll
O = Segs2Polys.o

COMPILER = g++

ARCH = -m32

# OPTIMIZE_DBG = -Og
# OPTIMIZE_DBG = -Ofast
# WARNINGALL_DBG = -Wall

# OPTIMIZE = -O0
# OPTIMIZE = -O1
# OPTIMIZE = -O2
# OPTIMIZE = -Os
OPTIMIZE = -Ofast # xp ok

dll: dll_dll
	dumpbin /exports $(DES_DLL)

dll_o: $(SRC_MAIN)
	$(COMPILER) \
	-c $(SRC_MAIN)

dll_dll: dll_o
	$(COMPILER) \
	$(O) \
	-o $(DES_DLL) \
	-s \
	$(ARCH) \
	-Wl,--kill-at \
	-std=c++11 \
	-fopenmp \
	-static \
	-shared \
	-fvisibility=hidden \
	-lShlwapi \
	$(OPTIMIZE) \
	-Wl,--subsystem,windows

	del $(O)

# ----------------------------------------------------------------

# xp ok
debug: $(SRC_TEST)
	$(COMPILER) \
	-std=c++11 \
	-fopenmp \
	$(WARNINGALL_DBG) \
	$(ARCH) \
	$(OPTIMIZE_DBG) \
	-D_DEBUG \
	$(SRC_TEST) \
	-static
