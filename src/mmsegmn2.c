/* MMSEG: A Word Identification System for Mandarin Chinese Text  */
/*   Based On Two Variations of the Maximum Matching Algorithm    */
/*                         Chih-Hao Tsai                          */
/*           University of Illinois at Urbana-Champaign           */
/*   c-tsai4@uiuc.edu  http://casper.beckman.uiuc.edu/~c-tsai4    */
/*                            May 1996                            */

/* MMSEGMN2.C: Complex matching function (Chen & Liu, 1992) and   */
/*             and four disambiguation rules                      */
/* Priority of ambiguity resolution rules:                        */
/*      1. Maximum Matching                                       */
/*      2. Largest Average Word Length                            */
/*      3. Smallest Variance of Word Lengths                      */
/*      4. Largest Sum of 100xlog(charfreq)                       */
/*                                                                */
/* 3/6/98. Bug report (and workaround). There is a bug in         */
/* mmsegmn1.c that when there are more than one segmentations     */
/* with the same largest sum of log(cf), the program will loop    */
/* forever.  Now the bug has been fixed.  (Well, actually it is   */
/* only a workaround. I cheated the program to let believe that   */
/* there is no ambiguity.)  Please replace the old mmsegmn1.c     */
/* with this: mmsegmn2.c.                                         */

#include "mmseg.h"

extern FILE    *out;
extern int      b5word,
                verbose,
                logcharfreq[13060];
extern long     lexicon_offset[12000];
extern int      lexicon_counter[12000];
extern int      totalamb,
                mmstat,
                svarstat,
                lavgstat,
                logcfstat;

void            mmsegment(char *chunk)
{
    int             i = 0,
                    j,
                    k,
                    k1,
                    l,
                    m,
                    n,
                    o,
                    p,
                    index_offset,
                    body_found,
                    w[3],
                    mlen[3],
                    pos,
                    longest,
                    found,
                    mmcounter,
                    llog,
                    logcf,
                    mmlen[128],
                    mmnword[128],
                    mmlogcf[128],
                    mmlongest[64],
                    mmshort[64],
                    smallvar[64],
                    largeavg[64],
                    largelog[64];
    unsigned        sernum;
    float           sumsq,
                    avglen,
                    svar,
                    lavg,
                    mmvarlen[128],
                    mmavglen[128];
    char            head[3],
                    body[17],
                    mword[3][17],
                    mmword[128][17];

    while ((i * 2) < strlen(chunk)) {
        longest = 0;
        mmcounter = 0;
        for (w[2] = 8; w[2] >= 0; w[2]--) {
            for (w[1] = 8; w[1] >= 0; w[1]--) {
                for (w[0] = 8; w[0] >= 1; w[0]--) {
                    if ((w[0] + w[1] + w[2]) <= (strlen(chunk) / 2 - i)) {
                        found = 1;
                        for (j = 0; j < 3; j++) {
                            switch (j) {
                            case 0:
                                pos = i * 2;
                                break;
                            case 1:
                                pos = (i + w[0]) * 2;
                                break;
                            case 2:
                                pos = (i + w[0] + w[1]) * 2;
                                break;
                            default:
                                pos = i * 2;
                            }
                            mword[j][0] = 0x00;
                            if (w[j] == 1) {
                                strncpy(mword[j], &chunk[pos], 2);
                                mword[j][2] = 0x00;
                            } else if (w[j] >= 2) {
                                strncpy(head, &chunk[pos], 2);
                                head[2] = 0x00;
                                index_offset = search_head(w[j], head);
                                if (index_offset >= 0) {
                                    strncpy(body, &chunk[pos + 2], (w[j] - 1) * 2);
                                    body[(w[j] - 1) * 2] = 0x00;
                                    body_found = search_body(w[j], lexicon_offset[index_offset], lexicon_counter[index_offset], body);
                                    if (body_found >= 0) {
                                        strncpy(mword[j], &chunk[pos], w[j] * 2);
                                        mword[j][w[j] * 2] = 0x00;
                                    } else
                                        found = 0;
                                } else
                                    found = 0;
                            }
                        }
                        if ((w[1] == 0) && (w[2] != 0))
                            found = 0;
                        if (found) {
                            mmlen[mmcounter] = 0;
                            mmnword[mmcounter] = 0;
                            mmavglen[mmcounter] = 0;
                            mmlogcf[mmcounter] = 0;
                            strcpy(mmword[mmcounter], mword[0]);
                            for (k = 0; k < 3; k++) {
                                mlen[k] = strlen(mword[k]) / 2;
                                mmlen[mmcounter] += mlen[k];
                                if (mlen[k] == 1) {
                                    sernum = big5_character_test((unsigned) mword[k][0], (unsigned) mword[k][1]);
                                    sernum -= 0x8800;
                                    logcf = logcharfreq[sernum];
                                    mmlogcf[mmcounter] += logcf;
                                }
                                if (mlen[k] > 0)
                                    mmnword[mmcounter]++;
                            }
                            avglen = (float) mmlen[mmcounter] / mmnword[mmcounter];
                            mmavglen[mmcounter] = avglen;
                            sumsq = 0;
                            for (k = 0; k < mmnword[mmcounter]; k++)
                                sumsq += (float) (mlen[k] - avglen) * (mlen[k] - avglen);
                            mmvarlen[mmcounter] = (float) sumsq / mmnword[mmcounter];
                            if (mmlen[mmcounter] >= longest)
                                longest = mmlen[mmcounter];
                            if (verbose == 2) {
                                printf("%d word(s)    Word1=%s Word2=%s Word3=%s\n",
                                       mmnword[mmcounter], mword[0], mword[1], mword[2]);
                                printf("SumLength=%d Variance=%f MeanLen=%f SumLogCF=%d\n",
                                       mmlen[mmcounter], mmvarlen[mmcounter], avglen, mmlogcf[mmcounter]);
                            }
                            mmcounter++;
                        }
                    }
                }
            }
        }
        if (verbose > 0)
            printf(">> %d possible 3-word to 1-word chunk(s).\n", mmcounter);
        l = 0;
        if (mmcounter > 1)
            totalamb++;
        for (k = 0; k < mmcounter; k++) {
            if (mmlen[k] == longest) {
                mmlongest[l] = k;
                l++;
            }
        }
        if (l > 1) {
            if (verbose > 0)
                printf(">>>> %d chunks with maximum length.\n", l);
            lavg = 0;
            for (k = 0; k < l; k++) {
                if (mmavglen[mmlongest[k]] >= lavg)
                    lavg = mmavglen[mmlongest[k]];
            }
            n = 0;
            for (k = 0; k < l; k++) {
                if (mmavglen[mmlongest[k]] == lavg) {
                    largeavg[n] = mmlongest[k];
                    n++;
                }
            }
            if (n > 1) {
                if (verbose > 0)
                    printf(">>>>>> %d chunks with largest mean length.\n", n);
                svar = 100;
                for (k = 0; k < n; k++) {
                    if (mmvarlen[largeavg[k]] <= svar)
                        svar = mmvarlen[largeavg[k]];
                }
                o = 0;
                for (k = 0; k < n; k++) {
                    if (mmvarlen[largeavg[k]] == svar) {
                        smallvar[o] = largeavg[k];
                        o++;
                    }
                }
                if (o > 1) {
                    if (verbose > 0)
                        printf(">>>>>>>> %d chunks with smallest variance of word lengths.\n", o);
                    llog = 0;
                    for (k = 0; k < o; k++) {
                        if (mmlogcf[smallvar[k]] >= llog) {
                            llog = mmlogcf[smallvar[k]];
                        }
                    }
                    p = 0;
                    for (k = 0; k < o; k++) {
                        if (mmlogcf[smallvar[k]] == llog) {
                            largelog[p] = smallvar[k];
                            p++;
                        }
                    }
                    /*if (p > 1) {
                        if (verbose > 0)
                            printf(">>>>>>>>>> %d chunks with largest sum of 100xlogCF.\n", p);
                        for (k = 1; k < p; k++) {
                            fputc('(', out);
                            for (k1 = 0; k1 < strlen(mmword[largelog[k]]); k1++)
                                fputc(mmword[largelog[k]][k1], out);
                            fputc(')', out);
                            fputc(' ', out);
                            i += strlen(mmword[largelog[0]]) / 2;
                            b5word++;
                        }
                    } else */{
                        logcfstat++;
                        if (verbose > 0)
                            printf("-------------- Largest_Sum_of_100xLogCF Rule takes ");
                        for (k = 0; k < strlen(mmword[largelog[0]]); k++) {
                            fputc(mmword[largelog[0]][k], out);
                            if (verbose > 0)
                                putchar(mmword[largelog[0]][k]);
                        }
                        fputc(' ', out);
                        if (verbose > 0)
                            putchar('\n');
                        i += strlen(mmword[largelog[0]]) / 2;
                        b5word++;
                    }
                } else {
                    svarstat++;
                    if (verbose > 0)
                        printf("-------------- Smallest_Variance_of_Word_Lengths Rule takes ");
                    for (k = 0; k < strlen(mmword[smallvar[0]]); k++) {
                        fputc(mmword[smallvar[0]][k], out);
                        if (verbose > 0)
                            putchar(mmword[smallvar[0]][k]);
                    }
                    fputc(' ', out);
                    if (verbose > 0)
                        putchar('\n');
                    i += strlen(mmword[smallvar[0]]) / 2;
                    b5word++;
                }
            } else {
                lavgstat++;
                if (verbose > 0)
                    printf("-------------- Largest_Average_Word_Length Rule takes ");
                for (k = 0; k < strlen(mmword[largeavg[0]]); k++) {
                    fputc(mmword[largeavg[0]][k], out);
                    if (verbose > 0)
                        putchar(mmword[largeavg[0]][k]);
                }
                fputc(' ', out);
                if (verbose > 0)
                    putchar('\n');
                i += strlen(mmword[largeavg[0]]) / 2;
                b5word++;
            }
        } else {
            if (mmcounter > 1)
                mmstat++;
            if (verbose > 0)
                printf("-------------- Maximum_Matching Rule takes ");
            for (k = 0; k < strlen(mmword[mmlongest[0]]); k++) {
                fputc(mmword[mmlongest[0]][k], out);
                if (verbose > 0)
                    putchar(mmword[mmlongest[0]][k]);
            }
            fputc(' ', out);
            if (verbose > 0)
                putchar('\n');
            i += strlen(mmword[mmlongest[0]]) / 2;
            b5word++;
        }
    }
}
