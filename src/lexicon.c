/* MMSEG: A Word Identification System for Mandarin Chinese Text  */
/*   Based On Two Variations of the Maximum Matching Algorithm    */
/*                         Chih-Hao Tsai                          */
/*           University of Illinois at Urbana-Champaign           */
/*   c-tsai4@uiuc.edu  http://casper.beckman.uiuc.edu/~c-tsai4    */
/*                            May 1996                            */

/* LEXICON.C: Open lexicon,load indices and character frequency   */

#include "mmseg.h"

FILE           *lexicon[7];
char            lexicon_head[12000][2];
long            lexicon_offset[12000];
int             lexicon_counter[12000],
                index_top[7],
                index_bottom[7],
                logcharfreq[13060];

void            open_lexicon(char *pathname)
{
    int             i;
    char            charlen[2],
                    name[129],
                   *prefix = "chr",
                   *suffix = ".lex";

    charlen[1] = 0x0;

    for (i = 0; i <= 6; i++) {
        charlen[0] = i + 0x32;
        strcpy(name, pathname);
        strcat(name, prefix);
        strcat(name, charlen);
        strcat(name, suffix);
        if ((lexicon[i] = fopen(name, "rb")) == NULL) {
            fprintf(stderr, "Cannot open %s.\n", name);
            exit(1);
        }
    }
}

void            close_lexicon(void)
{
    int             i;
    for (i = 0; i < 7; i++)
        fclose(lexicon[i]);
}

void            load_index(char *pathname)
{
    FILE           *lex_index[7];
    int             i,
                    j = 0;
    char            charlen[2],
                    name[129],
                    string[15],
                    offset[7],
                    counter[4],
                   *prefix = "chr",
                   *suffix = ".inx";

    charlen[1] = 0x0;

    for (i = 0; i <= 6; i++) {
        charlen[0] = i + 0x32;
        strcpy(name, pathname);
        strcat(name, prefix);
        strcat(name, charlen);
        strcat(name, suffix);
        if ((lex_index[i] = fopen(name, "rt")) == NULL) {
            fprintf(stderr, "Cannot open %s.\n", name);
            exit(1);
        }
        index_top[i] = j;
        while (!feof(lex_index[i])) {
            fgets(string, 15, lex_index[i]);
            if (feof(lex_index[i]))
                break;
            strncpy(lexicon_head[j], string, 2);
            strncpy(offset, &string[2], 6);
            offset[6] = 0x00;
            lexicon_offset[j] = atol(offset);
            strncpy(counter, &string[8], 3);
            counter[3] = 0x00;
            lexicon_counter[j] = atoi(counter);
            j++;
        }
        index_bottom[i] = j - 1;
        fclose(lex_index[i]);
    }
}

void            load_logcharfreq(char *pathname)
{
    FILE           *character;
    int             i = 0;
    double          d_freq;
    float           log_freq;
    char
                    name[129],
                    string[15],
                    c_freq[9];

    strcpy(name, pathname);
    strcat(name, "charfreq.dat");

    if ((character = fopen(name, "rt")) == NULL) {
        fprintf(stderr, "Cannot open %s.\n", name);
        exit(1);
    }
    while (!feof(character)) {
        fgets(string, 15, character);
        if (feof(character))
            break;
        strncpy(c_freq, &string[2], 7);
        c_freq[7] = 0x00;
        d_freq = atof(c_freq);
        logcharfreq[i] = (int) (log(d_freq) * 100);
        i++;
    }
    fclose(character);
}
