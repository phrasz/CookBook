/* $Id: perf_tool2.h,v 1.4 2006/09/06 14:36:49 roca Exp $ */

/*  LDPC/LDGM extended performance tool.
 *  (c) Copyright 2002-2006 INRIA - All rights reserved
 *  Main authors: Christoph Neumann (christoph.neumann@inrialpes.fr)
 *                Vincent Roca      (vincent.roca@inrialpes.fr)
 *		  Julien Laboure   (julien.laboure@inrialpes.fr)
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 *  USA.
 */

#ifdef WIN32		/* Windows specific includes */
#include <Winsock2.h>
#include <windows.h>
#else	/* UNIX */	/* Unix specific includes */
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>	/* for gettimeofday */
#endif	/* OS */
#include <stdlib.h>

#include "../src/ldpc_fec.h"
#include "../src/ldpc_scheme.h"
#include "../src/macros.h"

#ifdef WIN32
#include "../src/getopt.h"
#endif


/*
 * OS dependant definitions
 */
#if defined(LINUX) || defined(SOLARIS)
#define SOCKET		int
#define SOCKADDR	sockaddr
#define	SOCKADDR_IN	sockaddr_in
#define INVALID_SOCKET	(-1)
#define SOCKET_ERROR	(-1)
#endif	/* OS */


/*
 * Default simulation parameters
 */
#define PKTSZ	1024	// Packets size, in bytes (multiple of 4).
#define SYMBOLSZ 1024	// Symbol size, in bytes (multiple of 4).
#define NBDATA	2000	// Number of original DATA symbols to send.
#define NBFEC	1000		// Number of FEC symbols to build.
#define NBPKT	(NBDATA+NBFEC)	// Total number of packets to send.
#define LEFT_DEGREE	3	// Left degree of data nodes in the bipartite
						// graph
#define VERBOSITY	0	// Define the verbosity level
#define MAX_N_FOR_RSE	256	// RSE only


/*
 * If defined, then check if rebuilt packets match original ones
 */
#define CHECK_INTEGRITY


/*
 * Various codecs available.
 * Be careful to keep the order compliant with the eperf_tool -c<n> argument.
 */
typedef enum {
	CODEC_LDGM,
	CODEC_LDPC_STAIRCASE,
	CODEC_LDPC_TRIANGLE
} codec_t;


/*
 * Various transmission modes.
 * Be careful to keep the order compliant with the eperf_tool -t<n> argument.
 */
typedef enum {
	TX_MODE_NORMAL,		// random permutation of all packets
	TX_MODE_SEQUENTIAL_SRC_THEN_RANDOM_FEC
} tx_mode_t;


/*
 * Additional macros, required by rse_fec.cpp
 */
#define mcl_stdout	stdout
#define mcl_stderr	stderr

#define PRINT_OUT(a) { \
		fprintf a; \
		fflush(mcl_stdout); \
	}

#define PRINT_ERR(a) { \
		fprintf a; \
		fflush(mcl_stderr); \
	}

/*
 * Additional defines and includes, required by mcl_blocking_struct.cpp
 */

#ifndef UINT32
#define INT8	char
#define	INT16	short
#define INT32	int		// also true in LP64 64-bit architectures
#define	UINT8	unsigned char
#define	UINT16	unsigned short
#define	UINT32	unsigned int	// also true in LP64 64-bit architectures
#endif


#include "mcl_blocking_struct.h"

typedef struct {
	//int		seq;
	int		k;
	int		n;
	int		first_pkt_idx;	// index of first data packet in the
					// global dataOrig[nbPKT] table (!=seq)
	int		first_fec_pkt_idx; // index of first FEC packet in the
					// global dataOrig[nbPKT] table (!=seq)
	/* receiver specific fields */
	bool		decoded;	// used by rx: true if decoded
	int		pkt_rx;		// number of packets received
} block_t;

typedef struct {
	int		seq;
	int		blk_seq;
} packet_t;

