/***************************************************************************
!*
!*  This is the 'C' version of the sequence include file. This file
!*  declares structures and parameters for reading and writing sequences.
!*  
!* $Header: /usr/users/unix2/gcgsource/include/RCS/sequence_c.h,v 1.3 1991/03/05 20:21:15 rose Exp $
!*
!* $Copyright:  Copyright 1982, 1983, 1984, 1985, 1986, 1987, 1989, and 1991
!*          Genetics Computer Group, Inc.  All rights reserved. $
!*
!***************************************************************************/

/* #include "gendefs.h"				system definitions	*/

#define MAXSEGLEN	350000			/* future use...	*/
#define MAXPROTLEN	100000			/* max protein length	*/
#define MAXOVERLAP	512			/* max overlap (future) */
#define MAXSEQNAME 	128		/* max seq name length	*/
#define MAXACCNUM 	7			/* max acc number length*/
#define MAXDOCSTRING 	256			/* max seq definition   */
#define MAXMSFSEQNAME   39

/* the sequence structure. This must correspond EXACTLY to the */
/* to the fortran structure defined in sequence.inc            */

struct sequence {
	char seq[MAXPROTLEN];			/* sequence 		*/
	int check;				/* checksum 		*/
	int len;				/* sequence length	*/
	int rev;				/* reverse = true	*/
	char name[MAXSEQNAME];			/* sequence name	*/
	char number[MAXACCNUM+1];		/* accession number	*/
	char doc[MAXDOCSTRING+1];		/* sequence definition	*/
	int format;				/* format in database	*/
	int type;				/* type (nucleic, a.a.) */
	int isnbrf;				/* true if nbrf data	*/
	int ismsf;				/* true if from msf file*/
	float weight;				/* relative importance of seq*/
};

