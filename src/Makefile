SYSTEM     = x86-64_linux
#SYSTEM     = x86-64_osx
#LIBFORMAT  = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

ILOGDIR = /opt/ibm/ILOG/CPLEX_Studio127
#ILOGDIR = /opt/applis/cplex-12.10
#ILOGDIR	= /Users/fclautiaux/Applications/IBM/ILOG/CPLEX_Studio129
CPLEXDIR      = ${ILOGDIR}/cplex
CONCERTDIR    = ${ILOGDIR}/concert


# ---------------------------------------------------------------------
# Compiler selection
# ---------------------------------------------------------------------

CCC = g++

# ---------------------------------------------------------------------
# Compiler options
# ---------------------------------------------------------------------

CCOPT = -m64 -O3 -fPIC -fexceptions -DNDEBUG -DIL_STD -std=c++0x
# CCOPT = -m64 -O -fPIC -fexceptions -g -DIL_STD

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -lpthread -ldl

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)




all:


heuristique_p.o: heuristique_p.cpp
	$(CCC) -c $(CCFLAGS) heuristique_p.cpp -o heuristique.o

	
	
	
clean:
	rm -f *.o




run:
	MHLOP_MaxN $(ARGS)
