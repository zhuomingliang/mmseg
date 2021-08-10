/* MMSEG: A Word Identification System for Mandarin Chinese Text  */
/*   Based On Two Variations of the Maximum Matching Algorithm    */
/*                         Chih-Hao Tsai                          */
/*           University of Illinois at Urbana-Champaign           */
/*   c-tsai4@uiuc.edu  http://casper.beckman.uiuc.edu/~c-tsai4    */
/*                            May 1996                            */

/* MESSAGE.C: Show usage and example of command line arguments    */

#include "mmseg.h"

void            show_message_and_exit(void)
{
    printf("MMSEG is a word identification program for Chinese text based (mainly) on two\n");
    printf("variants of the maximum matching algorithm. These algorithms are discussed\n");
    printf("in MMSEG.TXT. By Chih-Hao Tsai. Email: c-tsai4@uiuc.edu. Last modified 3/6/98.\n\n");
    printf("Usage: MMSEG file1 file2 path [complexity] [report level]\n\n");
    printf("file1           Source file to be processed\n");
    printf("file2           Target file to write segmented text to\n");
    printf("path            Where the lexicon can be found\n");
    printf("complexity      (Optional) Complexity of matching algorithm:\n");
    printf("       simple   Simple (1 word) matching (default)\n");
    printf("      complex   Complex (3-word chunk) matching\n");
    printf("report level    (Optional) (For complex matching only): Progress report sent\n");
    printf("                to standard output (screen) during segmentation:\n");
    printf("      verbose   Display (1) All ambiguous segmentations and the length,\n");
    printf("                variance of word lengths, average word length, and sum of\n");
    printf("                log(frequency) for each each segmentation (2) Number of\n");
    printf("                ambiguous segmentations not resolved by each disambiguation\n");
    printf("                rule, and at which rule the ambiguity is resolved\n");
    printf("     standard   Display (2) only\n");
    printf("        quiet   None of the above information will be displayed\n\n");
    printf("Example: mmseg in.txt out.txt ./lexicon/ complex quiet\n");
    exit(1);
}
