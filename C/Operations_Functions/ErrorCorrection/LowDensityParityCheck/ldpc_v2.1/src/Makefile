# $Id: Makefile,v 1.60 2006/09/06 14:55:39 roca Exp $
#
# Makefile
#

###### OS Dependant Definitions ######

# they are defined in the following file...
# edit Makefile.common as appropriate
include ../Makefile.common


ifeq (${OS},linux)
	# Linux version
	# A few libs for memory debug:
	# -lefence	for memory debug on Linux (and other plateforms)
	#		Requires: export EF_PROTECT_FREE=1
	#			  export EF_PROTECT_BELOW=1
	#
	#LIBRARIES = -Wl,-Bstatic -lefence -Wl,-Bdynamic -lc -lm -lpthread
	LIBRARIES = -lc -lm -lpthread 
	BINDIR = ../bin/linux
endif

ifeq (${OS},solaris)
	# Solaris version: requires -lsocket -lnsl and -L/usr/ucblib -lucb
	#
	LIBRARIES = -lc -lm -lsocket -lnsl -L/usr/ucblib -lucb -lpthread
	BINDIR = ../bin/solaris
endif

ifeq (${OS},freebsd)
	# FreeBSD version
	BINDIR = ../bin/freebsd
	LIBRARIES = -lm -pthread
endif


###### Other Variables ######

# List of source, object and include files
#
SRCFILES  = ldpc_create_pchk.cpp ldpc_fec.cpp ldpc_fec_iterative_decoding.cpp tools.cpp ldpc_matrix_sparse.cpp ldpc_rand.cpp ldpc_scheme.cpp
OFILES = ${SRCFILES:.cpp=.o}

# you can add here specific flags for the ldpc library ...
#CFLAGS	+= 


###### Dependancies ######
# by default, create the MCL lib
all: lib

# Rule for C++ .cpp extension
.cpp.o:
	$(CC) -c $(CFLAGS) -o $*.o $*.cpp

lib : ${OFILES}
	@echo "** Create the LDPC library..."
	${AR} -rcsv ${BINDIR}/libldpc.a ${OFILES}
	@echo "done"

tags:
	etags $(SRCS)

clean :
	@for f in core ${OFILES}; do \
		if [ -f "$${f}" ] ; then \
			rm "$${f}"; \
		fi \
	done

cleanall : clean
	@for f in ${BINDIR}/libldpc.a ${BINDIR}/*.pchk ; do \
		if [ -f "$${f}" ] ; then \
			rm $${f} ; \
		fi \
	done

depend :
	# Use -Y to prevent including the system header files that won't
	# change. Send all error messages to /dev/null
	${DEPEND} -- ${CFLAGS} -Y -- ${SRCFILES} 2>/dev/null


# Dependencies automatically created by makedepend (see depend target above).
#
# DO NOT DELETE THIS LINE -- make depend depends on it.
