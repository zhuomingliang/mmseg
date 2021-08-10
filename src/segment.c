/* MMSEG: A Word Identification System for Mandarin Chinese Text  */
/*   Based On Two Variations of the Maximum Matching Algorithm    */
/*                         Chih-Hao Tsai                          */
/*           University of Illinois at Urbana-Champaign           */
/*   c-tsai4@uiuc.edu  http://casper.beckman.uiuc.edu/~c-tsai4    */
/*                            May 1996                            */

/* SEGMENT.C: Simple maximum matching function                    */

#include "mmseg.h"

extern FILE    *out;
extern int      b5word;
extern long     lexicon_offset[12000];
extern int      lexicon_counter[12000];

void            segment(char *chunk)
{
    char            head[3],
                    body[17];
    int             longest = 0,
                    i = 0,
                    j = 2,
                    k,
                    l,
                    index_offset,
                    body_found;

    while ((i * 2) < strlen(chunk)) {
        if (((strlen(chunk) / 2) - i) < 8)
            l = (strlen(chunk) / 2) - i;
        else
            l = 8;
        longest = 0;
        if (l >= 2) {
            for (j = l; j >= 2; j--) {
                strncpy(head, &chunk[i * 2], 2);
                head[2] = 0x00;
                index_offset = search_head(j, head);
                if (index_offset >= 0) {
                    strncpy(body, &chunk[i * 2 + 2], (j - 1) * 2);
                    body[(j - 1) * 2] = 0x00;
                    body_found = search_body(j, lexicon_offset[index_offset], lexicon_counter[index_offset], body);
                    if (body_found >= 0) {
                        longest = j;
                        break;
                    }
                }
            }
            if (longest) {
                for (k = 0; k < (longest * 2); k++)
                    fputc(chunk[i * 2 + k], out);
                fputc(' ', out);
                i += longest;
                b5word++;
            } else {
                fputc(chunk[i * 2], out);
                fputc(chunk[i * 2 + 1], out);
                fputc(' ', out);
                i++;
                b5word++;
            }
        } else {
            fputc(chunk[i * 2], out);
            fputc(chunk[i * 2 + 1], out);
            fputc(' ', out);
            i++;
            b5word++;
        }
    }
}
