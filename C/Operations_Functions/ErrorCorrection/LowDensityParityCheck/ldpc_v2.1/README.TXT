	$Id: README.TXT,v 1.17 2006/03/08 14:15:25 roca Exp $

	--------------------------------------------
	LDPC/LDGM Large Block FEC Codec, README File
	--------------------------------------------


This software is written in C++, and has been compiled and tested on 
different operating systems :
- Linux (g++ 3.x and 4.x)
- Solaris 2.8
- FreeBSD
- Windows 2000/XP (with Microsoft Visual Studio 2003, solution files included)

This LDPC codec is the only Open-Source, patent free, large block
FEC codec for the Packet Erasure Channel (e.g. Internet) available
as of today, to the best of our knowledge.

Traditional small block Forward Error Correction (FEC) codes, like the 
Reed-Solomon erasure (RSE) code, are known to raise efficiency problems
in particular when they are applied to the Asynchronous Layered Coding
(ALC) reliable multicast protocol. We have therefore designed a large
block FEC codec, derived from the well known Low-Density Parity Check
(LDPC) codes, and capable of operating on source blocks that are several
tens or hundreds of megabytes long. This work is based on the theoretical
work done by Gallager in the early 1960's.

A very detailed description and performance evaluation of the codec 
is available:``Design, Evaluation and Comparison of Four Large Block 
FEC Codecs, LDPC, LDGM, LDGM Staircase and LDGM Triangle, plus a 
Reed-Solomon Small Block FEC Codec'',
V. Roca, C. Neumann, - INRIA Research Report RR-5225, June 2004.
Other documents are also available on the web site, including a
detailed specification that enables independant interoperable
implementations (draft-ietf-rmt-fec-bb-ldpc-03.txt). All these
documents and others are available at URL:
	http://planete-bcast.inrialpes.fr/

This codec is both integrated in our MCLv3 library (more infos at
http://planete-bcast.inrialpes.fr/) and distributed 
independantly (in the LDPC source distribution). Therefore, it can
easily be used by other external applications or libraries. Several
such sample applications are provided in the LDPC distribution to
facilitate its integration into legacy applications, and information
provided in both the man pages and FAQ.TXT file. 

This codec is: 

Copyright (c) 2002-2006 INRIA - All rights reserved
Copyright (c) 1995-2003 by Radford M. Neal

It includes large portions of code written by Radford Neal
available at http://www.cs.toronto.edu/~radford/ldpc.software.html

