/* MMSEG: A Word Identification System for Mandarin Chinese Text  */
/*   Based On Two Variations of the Maximum Matching Algorithm    */
/*                         Chih-Hao Tsai                          */
/*           University of Illinois at Urbana-Champaign           */
/*   c-tsai4@uiuc.edu  http://casper.beckman.uiuc.edu/~c-tsai4    */
/*                            May 1996                            */

/* MMSEG.H: Header file for all source code                       */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void            show_message_and_exit(void);
void            open_iofiles(char *, char *);
void            close_iofiles(void);
void            open_lexicon(char *);
void            close_lexicon(void);
void            load_index(char *);
void            load_logcharfreq(char *);
int             search_head(int, char *);
int             search_body(int, long, int, char *);
void            segment(char *);
void            mmsegment(char *);
void            get_chunks_and_segment_them(char *);
unsigned int    big5_character_test(unsigned char, unsigned char);
int             big5_charactertype_test(unsigned int);
