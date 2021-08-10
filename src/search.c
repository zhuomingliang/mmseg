/* MMSEG: A Word Identification System for Mandarin Chinese Text  */
/*   Based On Two Variations of the Maximum Matching Algorithm    */
/*                         Chih-Hao Tsai                          */
/*           University of Illinois at Urbana-Champaign           */
/*   c-tsai4@uiuc.edu  http://casper.beckman.uiuc.edu/~c-tsai4    */
/*                            May 1996                            */

/* SEARCH.C: Lexicon search functions                             */

#include "mmseg.h"

extern FILE    *lexicon[7];
extern char     lexicon_head[12000][2];
extern long     lexicon_offset[12000];
extern int      lexicon_counter[12000],
                index_top[7],
                index_bottom[7];

int             search_head(int wordlen, char *head)
{
    int             l = index_top[wordlen - 2],
                    r = index_bottom[wordlen - 2],
                    x,
                    cmp;

    while (r >= l) {
        x = (l + r) / 2;
        cmp = strncmp(head, &lexicon_head[x][0], 2);
        if (cmp < 0)
            r = x - 1;
        else
            l = x + 1;
        if (cmp == 0)
            return x;
    }
    return -1;
}

int             search_body(int wordlen, long offset, int counter, char *body)
{
    int             l = 0,
                    r = counter - 1,
                    x,
                    cmp;
    int             i,
                    unit_length,
                    total_length;
    char            string[3000];

    unit_length = (wordlen - 1) * 2;
    total_length = (wordlen - 1) * 2 * counter;
    fseek(lexicon[wordlen - 2], offset + 2L, 0);
    fread(string, total_length, 1, lexicon[wordlen - 2]);

    while (r >= l) {
        x = (l + r) / 2;
        cmp = strncmp(body, &string[x * unit_length], unit_length);
        if (cmp < 0)
            r = x - 1;
        else
            l = x + 1;
        if (cmp == 0)
            return x;
    }
    return -1;
}
