/* MMSEG: A Word Identification System for Mandarin Chinese Text  */
/*   Based On Two Variations of the Maximum Matching Algorithm    */
/*                         Chih-Hao Tsai                          */
/*           University of Illinois at Urbana-Champaign           */
/*   c-tsai4@uiuc.edu  http://casper.beckman.uiuc.edu/~c-tsai4    */
/*                            May 1996                            */

/* GETCHUNK.C: Get a chunk of Chinese characters from text        */

#include "mmseg.h"

extern FILE    *out;
extern int      complexity,
                ascii,
                b5symbol,
                b5char;

void            get_chunks_and_segment_them(char *line)
{
    int             i = 0,
                    len,
                    big5_type,
                    big5_type1,
                    chunk_empty = 1,
                    extra_space = 1;
    unsigned        sernum,
                    sernum1;
    char           *chunk;

    len = strlen(line);
    chunk[0] = 0x00;

    while (i < len) {
        if ((unsigned) line[i] < 0xa1) {
            ascii++;
            if (chunk_empty)
                fputc(line[i], out);
            else {
                if (complexity == 1)
                    mmsegment(chunk);
                else
                    segment(chunk);
                if (line[i] != 0x20)
                    fputc(line[i], out);
                chunk[0] = 0x00;
                chunk_empty = 1;
            }
            i++;
        } else {
            sernum = big5_character_test((unsigned) line[i], (unsigned) line[i + 1]);
            big5_type = big5_charactertype_test(sernum);
            if (big5_type > 0) {
                b5char++;
                strncat(chunk, &line[i], 2);
                chunk_empty = 0;
                if ((strlen(chunk) == 2) && (i != 0)) {
                    if (line[i - 1] != 0x20) {
                        if (i > 1) {
                            sernum1 = big5_character_test((unsigned) line[i - 2], (unsigned) line[i - 1]);
                            big5_type1 = big5_charactertype_test(sernum1);
                            if (big5_type1 <= 0)
                                extra_space = 1;
                        } else
                            extra_space = 0;
/*                      if ((unsigned) line[i - 2] < 0xa1) */
/*                              extra_space = 0;           */
                    }
                    if (!extra_space) {
                        fputc(' ', out);
                        extra_space = 1;
                    }
                }
            } else {
                b5symbol++;
                if (!chunk_empty) {
                    if (complexity == 1)
                        mmsegment(chunk);
                    else
                        segment(chunk);
                } else if ((i != 0) && (line[i - 1] != 0x20)) {
                    sernum1 = big5_character_test((unsigned) line[i - 2], (unsigned) line[i - 1]);
                    big5_type1 = big5_charactertype_test(sernum1);
                    if (big5_type1 == 1) {
                        fputc(' ', out);
                        extra_space = 1;
                    }
                }
                chunk[0] = 0x00;
                chunk_empty = 1;
                fputc(line[i], out);
                fputc(line[i + 1], out);
                if ((i < (len - 2)) && (line[i + 2] != 0x20)) {
                    sernum1 = big5_character_test((unsigned) line[i + 2], (unsigned) line[i + 3]);
                    big5_type1 = big5_charactertype_test(sernum1);
                    if (big5_type1 == 1) {
                        fputc(' ', out);
                        extra_space = 1;
                    }
                }
            }
            i += 2;
        }
        if (i >= len) {
            if (!chunk_empty) {
                if (complexity == 1)
                    mmsegment(chunk);
                else
                    segment(chunk);
            }
            chunk[0] = 0x00;
            chunk_empty = 1;
        }
    }
}
