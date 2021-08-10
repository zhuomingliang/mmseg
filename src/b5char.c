/* MMSEG: A Word Identification System for Mandarin Chinese Text  */
/*   Based On Two Variations of the Maximum Matching Algorithm    */
/*                         Chih-Hao Tsai                          */
/*           University of Illinois at Urbana-Champaign           */
/*   c-tsai4@uiuc.edu  http://casper.beckman.uiuc.edu/~c-tsai4    */
/*                            May 1996                            */

/*  B5CHAR.C: BIG-5 encoding related functions                    */

#include "mmseg.h"

unsigned int    big5_character_test(unsigned char first, unsigned char second)
{
    unsigned int    sernum,
                    serbase;

    if (first == 0xc6 && second >= 0xa1) {
        serbase = 0x8001;
        first -= 0xc6;
        second = second - 0xa1;
        sernum = first * 0x9d + second + serbase;
        return (sernum);
    }
    if (first >= 0xa1 && first <= 0xa3) {
        serbase = 0x8400;
        first -= 0xa1;
    } else if (first >= 0xa4 && first <= 0xc6) {
        serbase = 0x8800;
        first -= 0xa4;
    } else if (first >= 0xc7 && first <= 0xc8) {
        serbase = 0x805f;
        first -= 0xc7;
    } else if (first >= 0xc9 && first <= 0xf9) {
        serbase = 0x9d19;
        first -= 0xc9;
    } else
        return (0);

    if (second >= 0x40 && second <= 0x7e)
        second -= 0x40;
    else if (second >= 0xa1 && second <= 0xfe)
        second = second - 0xa1 + 0x3f;
    else
        return (0);

    sernum = first * 0x9d + second + serbase;
    return (sernum);
}

int             big5_charactertype_test(unsigned sernum)
{
    if (sernum >= 0x8001 && sernum <= 0x816d)
        return (0);
    else if (sernum >= 0x8400 && sernum <= 0x8597)
        return (0);
    else if (sernum >= 0x8800 && sernum <= 0xBB03)
        return (1);
    else if (sernum >= 0xBB04 && sernum <= 0xBB25)
        return (0);
    else
        return (-1);
}
