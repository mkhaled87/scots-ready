#--------------------------------------------------------------------
# The main configurations
#--------------------------------------------------------------------
#--------------------------------------------------------------------
# - USE_MPI can be 'yes' or 'no'. If 'yes', it will define a flag and 
#    set the MPI conpilers and all other compiler settings below will
#    be neglegted.
# - BUILD_TYPE changes the build 'release' or 'debug'.
# - CXX and GCC change targeted compilers, unless USE_MPI is 'yes'.
# - OCL_INC and OCL_LNK specifiy special targeted OpenCL headers/lib
#    paths. Blank values mean the default include/link paths.
# - EXTRA_INC is used to add extra include directories. You have to
#    preceed every path with "-I"
# - EXTRA_LNK is used to add extra link directories. You have to
#    preceed every path with "-L" and the library name with "-l"
#--------------------------------------------------------------------
USE_MPI=no
BUILD_TYPE=release
DEMO_VERSION=no
CXX= g++
GCC= gcc
OCL_INC=
OCL_LNK=
EXTRA_INC=-I/usr/local/opt/openssl/include
EXTRA_LNK=-L/usr/local/opt/openssl/lib -lcpprest -lssl -lcrypto -lboost_system -ldl -lpthread

ifeq ($(shell uname),Darwin)
EXTRA_LNK += -lboost_thread-mt -lboost_chrono-mt
endif


ifneq ($(LIMITED_CONFIG),yes)
#--------------------------------------------------------------------
# Checking the OS for later use
#--------------------------------------------------------------------
OS= $(shell uname)

ifeq ($(OS),Gnu/Linux)
OS=Linux
endif



#--------------------------------------------------------------------
# Preparing bin/lib/include directories
# - modify OCL_INC to point to the include directories of OpenCL if
#   different from the default include directory
#--------------------------------------------------------------------
BIN_DIR=$(PROJECT_ROOT)/bin
LIB_DIR=$(PROJECT_ROOT)/lib
INC_DIR=$(PROJECT_ROOT)/include
SRC_DIR=$(PROJECT_ROOT)/src

LIB_CONFIG4CPP_DIR=$(LIB_DIR)/config4cpp/lib
INC_CONFIG4CPP_DIR=$(LIB_DIR)/config4cpp/include
BIN_CONFIG4CPP_DIR=$(LIB_DIR)/config4cpp/bin

LIB_BIGINT_DIR=$(LIB_DIR)/bigint
INC_BIGINT_DIR=$(LIB_DIR)/bigint

LIB_CUDD_DIR=$(LIB_DIR)/cudd/lib
INC_CUDD_DIR=$(LIB_DIR)/cudd

LIB_BITSET_DIR=$(LIB_DIR)/bitmap
INC_BITSET_DIR=$(LIB_DIR)/bitmap

LIB_MUPARSER_DIR=$(LIB_DIR)/muparser/lib
INC_MUPARSER_DIR=$(LIB_DIR)/muparser/include

LIB_SYMBOLICCPP_DIR=$(LIB_DIR)/symboliccpp/lib
INC_SYMBOLICCPP_DIR=$(LIB_DIR)/symboliccpp/include

INC_DYNOBJLOADER_DIR=$(LIB_DIR)/dynobjloader/include


INCLUDE_DIRECTORIES= -I$(INC_CONFIG4CPP_DIR)
INCLUDE_DIRECTORIES+= -I$(INC_BIGINT_DIR)
INCLUDE_DIRECTORIES+= -I$(INC_CUDD_DIR)
INCLUDE_DIRECTORIES+= -I$(INC_BITSET_DIR)
INCLUDE_DIRECTORIES+= -I$(INC_MUPARSER_DIR)
INCLUDE_DIRECTORIES+= -I$(INC_SYMBOLICCPP_DIR)
INCLUDE_DIRECTORIES+= -I$(INC_DYNOBJLOADER_DIR)


#--------------------------------------------------------------------
# Apply custom OCL Link-library paths
#--------------------------------------------------------------------
ifneq ($(OCL_INC),)
INCLUDE_DIRECTORIES+= -I$(OCL_INC)
endif

ifneq ($(OCL_LNK),)
LINK_WITH_OPENCL= -L$(OCL_LNK)
else
LINK_WITH_OPENCL=
endif

#--------------------------------------------------------------------
# Apply extra include paths
#--------------------------------------------------------------------
ifneq ($(EXTRA_INC),)
INCLUDE_DIRECTORIES+= $(EXTRA_INC)
endif

#--------------------------------------------------------------------
# Apply modifications if a debug version is requested
#--------------------------------------------------------------------
ifeq ($(BUILD_TYPE),debug)
EXE_PFACES=pfacesd
LINK_WITH_CONFIG4CPP=-L$(LIB_CONFIG4CPP_DIR) -lconfig4cppd
else
EXE_PFACES=pfaces
LINK_WITH_CONFIG4CPP=-L$(LIB_CONFIG4CPP_DIR) -lconfig4cpp
endif

#--------------------------------------------------------------------
# Special link options for MacOS
#--------------------------------------------------------------------
ifeq ($(OS),Darwin)
LINK_WITH_CONFIG4CPP += -lstdc++
LINK_WITH_OPENCL+=-framework OpenCL
CXX+= -DCL_SILENCE_DEPRECATION
GCC+= -DCL_SILENCE_DEPRECATION
else
LINK_WITH_OPENCL+=-lOpenCL
endif


#--------------------------------------------------------------------
# Setting link-library paths
#--------------------------------------------------------------------
LINK_WITH_CUDD=-L$(LIB_CUDD_DIR) -lcudd
LINK_WITH_BIGINT=$(LIB_BIGINT_DIR)/*.o
LINK_WITH_BITSET=$(LIB_BITSET_DIR)/*.o
LINK_WITH_MUPARSER=-L$(LIB_MUPARSER_DIR) -lmuparser
LINK_WITH_SYMOIOLICCPP=-L$(LIB_SYMBOLICCPP_DIR) -lsymboliccpp


#--------------------------------------------------------------------
# Update CXX_FLAGS based on BUILD_TYPE (debug or release)
#--------------------------------------------------------------------
ifeq ($(BUILD_TYPE),debug)
override CXX_FLAGS+= -I$(INC_DIR) -Wall -fPIC -Wextra -O0 -g -std=c++11
override GCC_FLAGS+= -I$(INC_DIR) -O0 -g -fPIC
else
override CXX_FLAGS+= -I$(INC_DIR) -Wall -fPIC -Wextra -std=c++11 -O3 -DNDEBUG
override GCC_FLAGS+= -I$(INC_DIR) -fPIC -O3 -DNDEBUG
endif


#--------------------------------------------------------------------
# Special compiler-wrappers when using MPI
#--------------------------------------------------------------------
ifdef USE_MPI
ifeq ($(USE_MPI),yes)
CXX		= mpic++ -DPFACES_USE_MPI
GCC		= mpicc -DPFACES_USE_MPI
PFACES_EXE_EXT=-mpi
override GCC_FLAGS+= -DPFACES_USE_MPI
endif
endif

#--------------------------------------------------------------------
# Special measuresd when compiling a demo version
#--------------------------------------------------------------------
ifdef DEMO_VERSION
ifeq ($(DEMO_VERSION),yes)
override GCC_FLAGS+= -DDEMO_VERSION
override CXX_FLAGS+= -DDEMO_VERSION
endif
endif


#--------------------------------------------------------------------
# Generic build rules
#--------------------------------------------------------------------
.SUFFIXES:	.cpp .c

.cpp.o:
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_DIRECTORIES) $<
	
.c.o:
	$(GCC) -c $(GCC_FLAGS) $(INCLUDE_DIRECTORIES) $<	

endif
