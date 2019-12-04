#
# compiler
#
#CC        = g++
CC       	  = clang++
CXXFLAGS 		= -Wall -Wextra -std=c++11 -O3 -DNDEBUG

#
# scots 
#
SCOTSROOT		= ../../..
SCOTSINC		= -I$(SCOTSROOT)/bdd -I$(SCOTSROOT)/utils 

#
# boost 
#
BOOSTINC		= -I/opt/local/include

#
# cudd 
#
CUDDPATH		=  $(SCOTSROOT)/cudd-3.0.0
CUDDINC 		= -I$(CUDDPATH)
CUDDLIBS		= -lcudd 
CUDDLPATH   	= -L$(CUDDPATH)/lib


all: dcdcLoadFromFile dcdc

%.o:%.cc
	$(CC) -c $(CXXFLAGS) $(CUDDINC) $(SCOTSINC) $(BOOSTINC) $< -o $@

dcdc: dcdc.o
	$(CC) $(CXXFLAGS) -o dcdc dcdc.o $(CUDDLPATH) $(CUDDLIBS)

dcdcLoadFromFile: dcdcLoadFromFile.o
	$(CC) $(CXXFLAGS) -o dcdcLoadFromFile dcdcLoadFromFile.o $(CUDDLPATH) $(CUDDLIBS)



clean:
	rm  ./dcdc ./dcdcLoadFromFile  ./dcdc.o ./dcdcLoadFromFile.o
