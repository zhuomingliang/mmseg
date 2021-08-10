# MMSEG: A Word Identification System for Mandarin Chinese Text
#  Based On Two Variations of the Maximum Matching Algorithm  
#                        Chih-Hao Tsai                        
#          University of Illinois at Urbana-Champaign         
#  c-tsai4@uiuc.edu  http://casper.beckman.uiuc.edu/~c-tsai4  
#                           May 1996                          
#
# MAKEFILE for MMSEG.EXE                                      

###################################################################
#	FreeBSD 2.2		Modified By Francis M.J. Hsieh
###################################################################
CC	= gcc
CFLAGS	= -O3 -Wall -Wno-unused #-pipe
DEBUG	= -g
F77	= g77
FFLAG	= -O3 -ff77 -fno-automatic #-pipe
LLIB	= -lm

###################################################################
SOURCE	= mmseg.o message.o iofiles.o lexicon.o search.o b5char.o getchunk.o \
	  segment.o mmsegmn2.o
###################################################################
mmseg: $(SOURCE)
	$(CC) $(CFLAG) $(DEBUG) $(SOURCE) $(LLIB) -o $(HOME)./mmseg.exe

mklex1: mklex1.c
	$(CC) $(CFLAG) $(DEBUG) mklex1.c $(LLIB) -o $(HOME)/bin/mklex1

mklex2: mklex2.c
	$(CC) $(CFLAG) $(DEBUG) $> $(LLIB) -o $(HOME)/bin/mklex2
###################################################################

.f.o:
	$(F77) $(FFLAGS) $(DEBUG) -c $*.f
.c.o:
	$(CC) $(CFLAGS) $(DEBUG) -c $*.c
clean:
	rm -f *.o core
# BACKUP
#
#CPP = tcc -Ff=10240 -mc
#SOURCE = mmseg.c message.c iofiles.c lexicon.c search.c b5char.c getchunk.c \
#         segment.c mmsegmn1.c

#mmseg.exe: $(SOURCE)
#  $(CPP) -c $(SOURCE)
#  $(CPP) $(SOURCE:.c=.obj)
# END of Makefile
