/* $Id: perf_tool2.cpp,v 1.7 2006/09/05 15:59:49 roca Exp $ */

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

#include "perf_tool2.h"

/*
 * Supported FEC codes
 */
#define LDGM
#define LDPC_STAIRCASE
#define LDPC_TRIANGLE

/*
 * Global variables.
 */
int	nbDATA		= NBDATA;	  /* k parameter */
int	nbFEC		= NBFEC;	  /* n - k parameter */
int	nbSYMBOLS	= NBDATA + NBFEC; /* n parameter */

int	pktSize		= PKTSZ;	  /* packet size */
int	symbolSize	= SYMBOLSZ;	  /* symbol size */

int	nbSymbolsPerPkt;
int 	nbDATAPkts;
int	nbFECPkts;
int	nbPKT; 	  

int	objSize		= 0;
double	fec_ratio	= 0.0;

#ifdef LDGM
int	left_degree	= LEFT_DEGREE;	  /* left degree in bipartite graph (LDGM only) */
#endif
int	verbosity	= VERBOSITY;	  /* verbosity 0: nothing, 1: all */
codec_t	codec		= CODEC_LDPC_STAIRCASE;	/* codec type */
tx_mode_t tx_mode	= TX_MODE_NORMAL; /* transmission mode */
int	suggested_seed	= 0;		  /* by default seed is choosen randomly*/


/*
 * Control the number of source data packets sent 
 */
int	use_ratio	= 1;	/* use_ratio==1 :use the value of src_pkt_ratio instead of 
				 * the value of src_pkt_nbr */ 
int	src_pkt_ratio	= 20;	/* % of source data packets are sent default value */
int	src_pkt_nbr	= 0;	/* % of source data packets are sent   */

int	simul_losses_state = 0;	/* loss state 0: NO_LOSS,  1: LOSS */
int	loss_model 	= 0;
int	tx_simul_loss	= 0;	/* number of simulated losses */

/*
 * number of (simulated) losses in % when the previous packet was OK (default value)
 */
#define P_LOSS_WHEN_OK	1
/*
 * number of succesful tx in % when the previous packet was LOST (default value)
 */
#define P_SUCCESS_WHEN_LOSSES	25

float	p_loss_when_ok 	= P_LOSS_WHEN_OK;
float	p_success_when_losses	= P_SUCCESS_WHEN_LOSSES;		


/* blocking variables */
mcl_blocking_struct_t bs;
int	max_k;
int	max_n;
int	tot_blk_nb;
int	k_for_this_blk;


/* Functions prototypes */
int	LDPC_LDGM_codes		();
void	init_prg_with_random_seed (int suggested_seed);
void 	ParseCommandLine	(int argc, char *argv[]);
void	printUsage		(char *cmdName);
void	randomize_array		(int *array, int arrayLen);
int	randomize_pkt_order	(int *array);
int 	RandomLoss		();


/**
 * Entry point for this application.
 */
int
main   (int	argc,
	char	**argv )
{
	int	err;

	ParseCommandLine(argc, argv);

	init_prg_with_random_seed(suggested_seed);

	switch ((int)codec) {
#ifdef LDGM
	case CODEC_LDGM:
#endif
#ifdef LDPC_STAIRCASE
	case CODEC_LDPC_STAIRCASE:
#endif
#ifdef LDPC_TRIANGLE
	case CODEC_LDPC_TRIANGLE:
#endif
		err = LDPC_LDGM_codes();
		break;
	default:
		EXIT(("ERROR: FEC codec %d not supported!\n", codec))
		break;
	}
	return err;
}


int
LDPC_LDGM_codes ()
{
	LDPCFecScheme	MyFecScheme;
	int		symbolseq;	/* symbol sequence number */
	int		pktseq;		/* pkt sequence number */
	int		sourceseq;	/* source symbol sequence number */
	int		fecseq;		/* fec symbol sequence number */
	char	**dataOrig;	/* filled with original data symbols AND
				  	   built FEC symbols */
	char	**PKTSOrig;	/* filled with original data PKTS AND
					   built FEC PKTS */
	int 		*pktIdxToESI;	/* mapping from packet index to ESI of first symbol */				   		   
	int		*randOrder;	/* table used by the tx randomization process */
	int		maxSteps;	/* max number of packets transmitted */
#ifdef WIN32
	LARGE_INTEGER tv0;	/* start */
	LARGE_INTEGER tv1;	/* end */
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
#else
	struct timeval	tv0;	/* start */
	struct timeval	tv1;	/* end */
	struct timeval	tv_delta;/* difference tv1 - tv0 */
#endif	// OS_DEP


	/*
	 * CODING PART
	 */

	/*
	 * step 1: initialize the LDPC FEC session/scheme
	 */
#ifdef WIN32
	QueryPerformanceCounter(&tv0);
	printf("init_start=%lI64f\n", (double)tv0.QuadPart/(double)freq.QuadPart);
#else
	gettimeofday(&tv0, NULL);
	printf("init_start=%ld.%ld\n", tv0.tv_sec, tv0.tv_usec);
#endif
	MyFecScheme.SetVerbosity(verbosity);
	if (objSize > 0) {
		if (MyFecScheme.DetermineSymbolSize(objSize, pktSize, &symbolSize,
						&nbDATA) == LDPC_ERROR) {
			EXIT(("ERROR: MyFecScheme.DetermineSymbolSize() failed. Check the -ps value provided.\n"))
		}
		if (fec_ratio < 1.0) {
			EXIT(("ERROR: invalid FEC ratio %.3f (must be >= 1.0).\nMake sure one has been specified with -fec<n>\n", fec_ratio))
		}
		nbFEC		= (int)ceil((fec_ratio - 1.0) * nbDATA);
		nbSYMBOLS	= nbDATA + nbFEC;
	}
	/* initialize the FEC Session first */
	switch ((int)codec) {
#ifdef LDGM
	case CODEC_LDGM:
		if (MyFecScheme.InitSession(nbDATA, nbFEC, symbolSize, FLAG_BOTH,
				rand(), TypeLDGM, left_degree) == LDPC_ERROR)
		{
			EXIT(("ERROR: Unable to initialize LDGM FEC session!\n"))
		}
		break;
#endif
#ifdef LDPC_STAIRCASE
	case CODEC_LDPC_STAIRCASE:
		if (MyFecScheme.InitSession(nbDATA, nbFEC, symbolSize, FLAG_BOTH,
				rand(), TypeSTAIRS) == LDPC_ERROR)
		{
			EXIT(("ERROR: Unable to initialize LDPC_STAIRCASE FEC session!\n"))
		}
		break;
#endif
#ifdef LDPC_TRIANGLE
	case CODEC_LDPC_TRIANGLE:
		if (MyFecScheme.InitSession(nbDATA, nbFEC, symbolSize, FLAG_BOTH,
				rand(), TypeTRIANGLE) == LDPC_ERROR)
		{
			EXIT(("ERROR: Unable to initialize LDPC_TRIANGLE FEC session!\n"))
		}
		break;
#endif
	default:
		EXIT(("ERROR: FEC codec %d not supported!\n", codec))
		break;
	}
	/* then initialize the FEC Scheme */
	if (MyFecScheme.InitScheme(symbolSize, pktSize) == LDPC_ERROR) {
		EXIT(("ERROR: Unable to initialize LDPC_TRIANGLE FEC scheme!\n"))
	}
	/* and now adjust the various counters */

	nbSymbolsPerPkt = MyFecScheme.getNbSymbolsPerPkt();
 	nbDATAPkts = MyFecScheme.getNbSourcePkts();
	nbFECPkts = MyFecScheme.getNbParityPkts();
	nbPKT = nbDATAPkts + nbFECPkts;
	if (objSize == 0)
		objSize = nbDATA * symbolSize;

#ifdef WIN32
	QueryPerformanceCounter(&tv1);
	printf("init_end=%I64f  init_time=%I64f\n",
		(double)tv1.QuadPart/(double)freq.QuadPart,
		(double)(tv1.QuadPart-tv0.QuadPart)/(double)freq.QuadPart );
#else
	gettimeofday(&tv1, NULL);
	timersub(tv1, tv0, tv_delta);
	printf("init_end=%ld.%ld  init_time=%ld.%06ld\n",
		tv1.tv_sec, tv1.tv_usec, tv_delta.tv_sec, tv_delta.tv_usec);
#endif

	MyFecScheme.MoreAbout(stdout);
#ifdef LDGM
	if (codec == CODEC_LDGM) {
		printf("\nLDPC/LDGM extended performance tool\ndata_symbols=%d  fec_symbols=%d  symbol_size=%d  nb_symbol_per_pkt=%d  total_nb_pkts=%d  pkt_size=%d  object_size=%d  left_degree=%d\n\n",
		nbDATA, nbFEC, symbolSize, nbSymbolsPerPkt, nbPKT, pktSize, objSize, left_degree);
	} else
#endif
	{
		printf("\nLDPC/LDGM extended performance tool\ndata_symbols=%d  fec_symbols=%d  symbol_size=%d  nb_symbol_per_pkt=%d  total_nb_pkts=%d  pkt_size=%d  object_size=%d  left_degree=3\n\n",
		nbDATA, nbFEC, symbolSize, nbSymbolsPerPkt, nbPKT, pktSize, objSize);
	}

	/*
	 * step 2: allocate and generate the original DATA symbols
	 */
	PRINT_LVL(1, ("\nAllocating and generating random source symbols...\n"))
	if ((dataOrig = (char**)calloc(nbSYMBOLS, sizeof(char*))) == NULL) {
		goto no_mem;
	}
	for (sourceseq = 0; sourceseq < nbDATA; sourceseq++) {
		/*
		 * buffer is 0'ed... Leave it like that, except for the first
		 * four bytes where we copy the pkt seq number.
		 */
		if ((dataOrig[sourceseq] = (char*)calloc(symbolSize, 1)) == NULL) {
			goto no_mem;
		}
		*(int *)dataOrig[sourceseq] = (int)sourceseq;
	}

	/*
	 * step 3: build FEC symbols...
	 */
	/* first allocate FEC symbol buffers */
	for (fecseq = 0; fecseq < nbFEC; fecseq++) {
		if ((dataOrig[fecseq + nbDATA] = (char*)calloc(symbolSize, 1))
		    == NULL)  {
			goto no_mem;
		}
	}
	PRINT_LVL(1, ("Building FEC symbols...\n"))
#ifdef WIN32
	QueryPerformanceCounter(&tv0);
	printf("build_fec_start=%lI64f\n", (double)tv0.QuadPart/(double)freq.QuadPart);
#else
	gettimeofday(&tv0, NULL);
	printf("build_fec_start=%ld.%ld\n", tv0.tv_sec, tv0.tv_usec);
#endif
	/* and now do FEC encoding */
	for (fecseq = 0; fecseq < nbFEC; fecseq++) {
		MyFecScheme.BuildParitySymbol((void**)dataOrig, fecseq,
						dataOrig[fecseq + nbDATA]);
	}
#ifdef WIN32
	QueryPerformanceCounter(&tv1);
	printf("build_fec_end=%I64f  build_fec_time=%I64f\n",
		(double)tv1.QuadPart/(double)freq.QuadPart,
		(double)(tv1.QuadPart-tv0.QuadPart)/(double)freq.QuadPart );

#else
	gettimeofday(&tv1, NULL);
	timersub(tv1, tv0, tv_delta);
	printf("build_fec_end=%ld.%ld  build_fec_time=%ld.%06ld\n",
		tv1.tv_sec, tv1.tv_usec, tv_delta.tv_sec, tv_delta.tv_usec);
#endif


	/*
	 * step 4: build pkts...
	 */
	pktIdxToESI = (int*) calloc(nbPKT,sizeof(int));
	if ((PKTSOrig = (char**)calloc(nbPKT, sizeof(char*))) == NULL) {
		goto no_mem;
	}	
	for (pktseq = 0; pktseq < nbPKT; pktseq++) {
		if ((PKTSOrig[pktseq] = (char*)calloc(1, pktSize)) == NULL)  {
			goto no_mem;
		}
		MyFecScheme.BuildPkt (pktseq, (void**) &PKTSOrig[pktseq],
					(void**) dataOrig, &pktIdxToESI[pktseq]);
		//PRINT_LVL(1,("PktIdx %i mapped to first symbol ESI %i\n",pktseq,pktIdxToESI[pktseq]))
	}


	/*
	 * step 5: randomize pkts order...
	 * this order is used for the "transmissions" of packets
	 */
	init_prg_with_random_seed(suggested_seed);	/* fix the seed here also */
	if ((randOrder = (int*)calloc(nbPKT, sizeof(int))) == NULL) {
		goto no_mem;
	}
	maxSteps = randomize_pkt_order(randOrder);

	/*
	 * DECODING PART
	 */

	char**	dataDest;
	int	DecodingSteps;	/* decoding step */
	int	txSteps;	/* transmission steps */
	int	newPktIdx;	/* index in PKTSOrig[] of packet received */
	char*	newPkt;		/* data buffer of packet received */

	PRINT_LVL(1, ("Decoding in progress...\n"))
#ifdef WIN32
	QueryPerformanceCounter(&tv0);
	printf("decoding_start=%lI64f\n", (double)tv0.QuadPart/(double)freq.QuadPart);
#else
	gettimeofday(&tv0, NULL);
	printf("decoding_start=%ld.%ld\n", tv0.tv_sec, tv0.tv_usec);
#endif
	if ((dataDest = (char**)calloc(nbDATA, sizeof(char*))) == NULL) {
		goto no_mem;
	}
	for (DecodingSteps = 0, txSteps = 0; ; txSteps++) {
		/*
		 * progress in the decoding with the new pkt received,
		 * of index newPktIdx
		 */
		switch (loss_model) {
		case 0:
			newPktIdx = randOrder[txSteps];
			newPkt = PKTSOrig[newPktIdx];
			ASSERT(newPkt);
			PRINT_LVL(1, ("------ tx step %d, pkt sent %d ------\n",
				txSteps, newPktIdx))
			break;
		case 1:
			newPktIdx = randOrder[txSteps];
			newPkt = PKTSOrig[newPktIdx];
			ASSERT(newPkt);
			PRINT_LVL(1, ("------ tx step %d, pkt sent %d ------\n",
				txSteps, newPktIdx))
			if (RandomLoss()==1)
			{
				PRINT_LVL(1, ("------ pkt %d lost ------\n", newPktIdx))
				if (txSteps >= maxSteps && (loss_model == 0 || loss_model ==  1)) break;
				else continue;
			}
			break;
		case 2:
			newPktIdx = rand() % nbPKT;
			newPkt = PKTSOrig[newPktIdx];
			ASSERT(newPkt);
			PRINT_LVL(1, ("------ tx step %d, pkt sent %d ------\n",
				txSteps, newPktIdx))
			break;
		default:
			EXIT(("ERROR, unknown loss model %d\n", loss_model))
			break;
		}

		MyFecScheme.DecodingStepWithPkt((void**)dataDest, newPkt, pktIdxToESI[newPktIdx], true);
		/* done, incr the step counter now */
		DecodingSteps++;
		/* check if completed if we received nbDATA packets or more */
		if (DecodingSteps >= nbDATAPkts &&
		    MyFecScheme.IsDecodingComplete((void**)dataDest)) {
			/* done! */
#ifdef WIN32
			QueryPerformanceCounter(&tv1);
			printf("decoding_end=%I64f  decoding_time=%I64f  decoding_steps=%d  inefficiency_ratio=%.3f\n",
				(double)tv1.QuadPart/(double)freq.QuadPart,
				(double)(tv1.QuadPart-tv0.QuadPart)/(double)freq.QuadPart,
				DecodingSteps, (float)DecodingSteps/(float)nbDATAPkts);
#else
			gettimeofday(&tv1, NULL);
			timersub(tv1, tv0, tv_delta);
			printf("decoding_end=%ld.%ld  decoding_time=%ld.%06ld  decoding_steps=%d  inefficiency_ratio=%.3f\n",
				tv1.tv_sec, tv1.tv_usec,
				tv_delta.tv_sec, tv_delta.tv_usec,
				DecodingSteps, (float)DecodingSteps/(float)nbDATAPkts);
#endif

			PRINT_LVL(1, ("Done! All DATA packets rebuilt in %d steps\n",
				DecodingSteps))
#ifdef CHECK_INTEGRITY
			/*
			 * check that data received/recovered is the
			 * same as data sent
			 */
			PRINT_LVL(1, ("Now checking DATA integrity...\n"))
			for (symbolseq = 0; symbolseq < nbDATA; symbolseq++) {
				if (memcmp(dataOrig[symbolseq], dataDest[symbolseq], symbolSize) != 0 ) {
					EXIT(("ERROR: symbol %d received/rebuilt doesn\'t match original\n", symbolseq))
				}
			}
			PRINT_LVL(1, ( "Mission accomplished, all data packets OK!\nLeaving...\n" ))
#endif // CHECK_INTEGRITY
			break;
		}
		if (txSteps >= maxSteps && (loss_model == 0 || loss_model ==  1)) break;
	}
	if (txSteps >= maxSteps && (loss_model == 0 || loss_model ==  1)) {
#ifdef WIN32
		QueryPerformanceCounter(&tv1);
		printf("decoding_end=%I64f  decoding_time=%I64f  decoding_steps=-1, decoding_failed\n",
			(double)tv1.QuadPart/(double)freq.QuadPart,
			(double)(tv1.QuadPart-tv0.QuadPart)/(double)freq.QuadPart);
#else
		gettimeofday(&tv1, NULL);
		timersub(tv1, tv0, tv_delta);
		printf("decoding_end=%ld.%ld  decoding_time=%ld.%06ld  decoding_steps=-1, decoding_failed\n",
			tv1.tv_sec, tv1.tv_usec,
			tv_delta.tv_sec, tv_delta.tv_usec);
#endif
		printf("ERROR: all packets received but decoding failed!\n");
	}
	/*
	 * close and free everything
	 */
	MyFecScheme.EndSession();
	/* free buffer allocated internally for decoded packets */
	for (symbolseq = 0; symbolseq < nbDATA; symbolseq++) {
		if (dataDest[symbolseq] != NULL) {
			/* this packet has been decoded by the codec */
			free(dataDest[symbolseq]);
		}
	}
	free(dataDest);
	/* free all data and FEC packets created by the source */
	for (symbolseq = 0; symbolseq < nbSYMBOLS; symbolseq++) {
		free(dataOrig[symbolseq]);
	}
	free(dataOrig);
	for (pktseq = 0; pktseq < nbPKT; pktseq++) {
		free(PKTSOrig[pktseq]);
	}
	free(PKTSOrig);
	free(pktIdxToESI);	
	free(randOrder);
	return 0;

no_mem:
	fprintf(stderr, "ERROR: no memory.\n");
	exit(-1);
}


/**
 * Initialize the Pseudo-random number generator with a random seed.
 */
void
init_prg_with_random_seed (int	suggested_seed)
{
	int	seed=0;	/* random seed for the srand() function */

	if (suggested_seed != 0) {
		seed = suggested_seed;
	} else {
		/* determine our own random seed */
#ifdef WIN32
		seed = timeGetTime();
#else  /* UNIX */
		struct timeval	tv;
		if (gettimeofday(&tv, NULL) < 0) {
			perror("init_prg_with_random_seed: ERROR, gettimeofday() failed:");
			exit(-1);
		}
		seed = (int)tv.tv_usec;
#endif /* OS */
	}
	srand(seed);
	printf("random seed=%d\n", seed);
}


/**
 * Randomize an array of integers
 */
void
randomize_array (int	*array,
		 int	arrayLen)
{
	int	backup = 0;
	int	randInd = 0;
	int	i;

	//init_prg_with_random_seed();
	for (i = 0; i < arrayLen; i++ ) {
		array[i] = i;
	}
	for (i = 0; i < arrayLen; i++) {
		backup = array[i];
		randInd = rand() % arrayLen;
		array[i] = array[randInd];
		array[randInd] = backup;
	}
}


int
randomize_pkt_order (int	*array)
{
	int	maxSteps;	// total number of packets sent
	int	i;

	switch (tx_mode) {
	case TX_MODE_NORMAL:		// random permutation of all packets
		PRINT_LVL(1, ("Randomizing source/FEC packets for tx...\n"))
		maxSteps = nbPKT;
		randomize_array(array, maxSteps);
		break;

	case TX_MODE_SEQUENTIAL_SRC_THEN_RANDOM_FEC:	
		PRINT_LVL(1, ("Sequential tx src pkts, then randomly FEC pkts...\n"))		
		maxSteps = nbPKT;
		randomize_array(array+nbDATAPkts, nbFECPkts);		
		for (i = 0; i < maxSteps; i++ ) {
			if (i < nbDATAPkts) {
				array[i] = i;
			} else {
				// it's a FEC packet
				array[i] += nbDATAPkts;
			}
		}
		break;

	default:
		EXIT(("ERROR: transmission mode %d not supported!\n", tx_mode))
	}
	return maxSteps;
}


/**
 * Print "usage" message to user
 */
void
printUsage (char *cmdName)
{
	LDPCFecSession	MyFecSession;
	printf("LDPC/LDGM extended performance evaluator\n");
	if (MyFecSession.InitSession(32, 32, 32, FLAG_BOTH) != LDPC_ERROR) {
		MyFecSession.MoreAbout(stdout);
	}
	printf("Usage: %s [options]\n", cmdName);
	printf("SOURCE/PARITY SPECIFICATION METHOD 1\n");
	printf("    -k<n>       set number of source symbols to n\n");
	printf("    -r<n>       set number of parity symbols (AKA repair symbols) to n\n");
	printf("    -ss<n>      sets symbol size to n (only for LDPC/LDGM codes)\n");
	printf("    -ps<n>      sets packets size to n\n");
	printf("SOURCE/PARITY SPECIFICATION METHOD 2\n");
	printf("    -os<n>      sets object size to n\n");
	printf("    -fec<n>  set FEC expansion ratio (n/k) to n (float)\n");
	printf("                (NB: this is 1/\"code rate\")\n");
	printf("    -ps<n>      sets packets size to n\n");
	printf("\n");
	printf("GENERAL PURPOSE OPTIONS:\n");
	printf("    -h[elp]     this help\n");
	printf("    -v<n>       sets verbosity to n\n");
	printf("CODE RELATED OPTIONS:\n");
	printf("    -c<n>       sets the codec:\n");
	printf("                  0: LDGM\n");
	printf("                  1: LDPC staircase (default)\n");
	printf("                  2: LDPC triangle\n");
#ifdef LDGM
	printf("    -l<n>       set left degree in bipartite graph (LDGM only)\n");	
#endif
	printf("TRANSMISSION RELATED OPTIONS:\n");
	printf("    -loss<n>    set the loss model:\n");
	printf("                  0: no losses (default)\n");
	printf("                  1: simulate random losses using two state markov model\n");
	printf("                  2: simulate losses by randomly choosing one packet\n");
	printf("                     out of all each step (overwrites transmission type)\n");
	printf("    -lossprob<n1>/<n2> set the loss probabilities (in %%) \n");
	printf("                       for the two state markov model:\n");
	printf("                  n1: probability of loss in state OK (integer, default %i)\n", P_LOSS_WHEN_OK);
	printf("                  n2: probability success in state NOK (integer, default %i)\n", P_SUCCESS_WHEN_LOSSES);
	printf("    -t<n>       sets transmission type:\n");
	printf("                  0: randomly send all FEC + source pkts (default)\n");
	printf("                  1: sequentially send all src pkts first, then randomly FEC pkts\n");		
	printf("    -seed<n>    sets the PRNG seed to n (to reproduce some experiments).\n");
	printf("                   Guaranties same matrix is created and same transmission order is used\n");
	exit(-1);
}


/* Parse the command line and its options */
void ParseCommandLine (int argc, char *argv[])
{
	int	c;
	char *OptList = "c:k:r:l:s:t:v:h:p:o:f:";

#ifdef SOLARIS
	extern char *optarg;
#elif defined(WIN32)
	char *optarg = NULL;
#endif
	if(argc < 1)
		printUsage(argv[0]);

#ifdef WIN32
	while ((c = GetOption(argc, argv, OptList, &optarg)) != 0)
#else
	while ((c = getopt(argc, argv, OptList)) != EOF)
#endif
	{
		switch (c) {
		case 'c':
			if (isdigit((int)*optarg)) {
				codec = (codec_t)atoi(optarg);
			} else
				EXIT(("bad argument -c%s\n", optarg))
			break;

		case 'k':
			if (isdigit((int)*optarg)) {
				nbDATA = atoi(optarg);
				nbSYMBOLS = nbDATA + nbFEC;
				nbPKT = nbDATA + nbFEC;	// TODO: only valid for RSE, change...
			} else
				EXIT(("bad argument -k%s\n", optarg))
			break;

		case 'r':
			if (isdigit((int)*optarg)) {
				nbFEC = atoi(optarg);
				nbSYMBOLS = nbDATA + nbFEC;
				nbPKT = nbDATA + nbFEC;	// TODO: only valid for RSE, change...
			} else
				EXIT(("bad argument -r%s\n", optarg))
			break;

		case 'l':
#ifdef LDGM
			if (isdigit((int)*optarg)) {
				left_degree = atoi(optarg);
			} else
#endif
			 if(!strncmp(optarg, "ossprob", 7)) {
				char	tmp_str[256];
				char		*p;
				strncpy(tmp_str, optarg+7, sizeof(tmp_str));
				if ((p = strchr(tmp_str, '/')) != NULL) {
					*p = '\0';
					/* in host order */
					p_success_when_losses = (unsigned short)atoi(p+1);
				}
				p_loss_when_ok = (float) atoi(tmp_str);
			} else if(!strncmp(optarg, "oss", 3) &&
				  isdigit((int)*(optarg+3))) {
				loss_model = atoi(optarg+3);
			} else
				EXIT(("bad argument -l%s\n", optarg))
			break;

		case 's':
			if (!strncmp(optarg, "s", 1) && isdigit((int)*(optarg+1))) {
				symbolSize = atoi(optarg+1);
			} else if(!strncmp(optarg, "eed", 3) &&
				  isdigit((int)*(optarg+3))) {
				suggested_seed	= atoi(optarg+3);
			} else
				EXIT(("bad argument -s%s\n", optarg))
			break;

		case 't':
			if (isdigit((int)*optarg)) {
				if (loss_model != 2) tx_mode = (tx_mode_t)atoi(optarg);
			} else
				EXIT(("bad argument -t%s\n", optarg))
			break;

		case 'v':
			if (isdigit((int)*optarg)) {
				verbosity = atoi(optarg);
				if (verbosity != 0 && verbosity != 1) {
					EXIT(("bad argument -v%s\n", optarg))
				}
			} else
				EXIT(("bad argument -v%s\n", optarg))
			break;

		case 'p':			
			if (!strncmp(optarg, "s", 1) && isdigit((int)*(optarg+1))) {
				pktSize = atoi(optarg+1);
			} else
				EXIT(("bad argument -p%s\n", optarg))
			break;

		case 'o':			
			if (!strncmp(optarg, "s", 1) && isdigit((int)*(optarg+1))) {
				objSize = atoi(optarg+1);
			} else
				EXIT(("bad argument -o%s\n", optarg))
			break;

		case 'f':
			if (!strncmp(optarg, "ec", 2)) {
				if (isdigit((int)*(optarg+2))) {
					fec_ratio = atof(optarg+2);
				}
			} else
				EXIT(("bad argument -f%s\n", optarg))
			break;
			
		case 'h':
			printUsage(argv[0]);
			break;

		default:
			/*
			 * NB: getopt returns '?' when finding an
			 * unknown argument; avoid the following
			 * error msg in that case
			 */
			if (c != '?')
				fprintf(stderr, "ERROR, bad argument\n");
			printUsage(argv[0]);
			break;
		}
	}
}


/*
 * Simulate packets losses randomly
 * Returns 0 if OK, 1 if packet should be lost...
 */
int RandomLoss()
{
	int IsLost = 0;

	switch ( simul_losses_state ) {
	case 0: /* last packet was sent OK. */
		if ((float)(rand()%100) < (float)p_loss_when_ok)
		{
			IsLost = 1;
			simul_losses_state = 1;
		}
		break;
	case 1: /* last packet was lost */
		if ((float)(rand()%100) < (float)p_success_when_losses)
		{
			simul_losses_state = 0;
		}
		else
			IsLost = 1;
		break;
	default:
		EXIT(("RandomLoss: unknown state"))
		break;
	}
	if (IsLost)
		tx_simul_loss++;
	return IsLost;
}


