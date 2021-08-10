/* MMSEG: A Word Identification System for Mandarin Chinese Text  */
/*   Based On Two Variations of the Maximum Matching Algorithm    */
/*                         Chih-Hao Tsai                          */
/*           University of Illinois at Urbana-Champaign           */
/*   c-tsai4@uiuc.edu  http://casper.beckman.uiuc.edu/~c-tsai4    */
/*                            May 1996                            */

/* IOFILES.C: Open and close files for input/output text          */

#include "mmseg.h"

FILE           *in,
               *out;

void            open_iofiles(char *infilename, char *outfilename)
{
    if ((in = fopen(infilename, "rt")) == NULL) {
        fprintf(stderr, "Cannot open %s.\n", infilename);
        exit(1);
    }
    if ((out = fopen(outfilename, "wt")) == NULL) {
        fprintf(stderr, "Cannot open %s.\n", outfilename);
        exit(1);
    }
}

void            close_iofiles(void)
{
    fclose(in);
    fclose(out);
}
