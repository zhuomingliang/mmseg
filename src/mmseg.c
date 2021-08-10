/* MMSEG: A Word Identification System for Mandarin Chinese Text  */
/*   Based On Two Variations of the Maximum Matching Algorithm    */
/*                         Chih-Hao Tsai                          */
/*           University of Illinois at Urbana-Champaign           */
/*   c-tsai4@uiuc.edu  http://casper.beckman.uiuc.edu/~c-tsai4    */
/*                            May 1996                            */

/* MMSEG.C: Top-level program                                     */

#include "mmseg.h"

extern FILE    *in,
               *out;
int             complexity = 0,
                verbose = 0,
                ascii = 0,
                b5symbol = 0,
                b5char = 0,
                b5word = 0,
                totalamb = 0,
                mmstat = 0,
                svarstat = 0,
                lavgstat = 0,
                logcfstat = 0;

int             main(int argc, char **argv)
{
    char           *ptr,
                    string[3200],
                    pathname[81],
                    infilename[81],
                    outfilename[81];
    int             cmp,
                    len;

    if (argc < 4)
        show_message_and_exit();
    else if (argc == 4)
        complexity = 0;
    else {
/*        cmp = stricmp(argv[4], "complex");*//* mjhsieh */
        cmp = strcmp(argv[4], "complex");
        if (cmp == 0)
            complexity = 1;
/*        cmp = stricmp(argv[5], "verbose");*//* mjhsieh */
        cmp = strcmp(argv[5], "verbose");
        if (cmp == 0)
            verbose = 2;
/*        cmp = stricmp(argv[5], "standard");*//* mjhsieh */
        cmp = strcmp(argv[5], "standard");
        if (cmp == 0)
            verbose = 1;
    }

    strcpy(infilename, argv[1]);
    strcpy(outfilename, argv[2]);
    strcpy(pathname, argv[3]);
    open_iofiles(infilename, outfilename);
    open_lexicon(pathname);
    load_index(pathname);
    load_logcharfreq(pathname);

    while (!feof(in)) {
        fgets(string, 3200, in);
        if (feof(in))
            break;
        ptr = strchr(string, 0x0a);
        if (ptr)
            ptr[0] = 0x00;
        get_chunks_and_segment_them(string);
        fputc('\n', out);
    }
    close_iofiles();
    close_lexicon();

    printf("ASCII characters = %d\n", ascii);
    printf("BIG-5 symbols = %d\n", b5symbol);
    printf("BIG-5 Chinese characters = %d\n", b5char);
    printf("BIG-5 Chinese words = %d\n", b5word);
    if (b5word != 0)
        printf("Average BIG-5 Chinese word length = %f\n", (float) b5char / b5word);
    if (complexity == 1) {
        printf("Ambiguity resolution statistics:\n");
        printf("%d total instances of ambiguity: \n", totalamb);
        printf("%3.2f resolved by Maximum Matching rule\n", (float) mmstat / totalamb * 100);
        printf("%3.2f resolved by Largest Average Word Lengths rule\n", (float) lavgstat / totalamb * 100);
        printf("%3.2f resolved by Smallest Variance of Word Lengths rule\n", (float) svarstat / totalamb * 100);
        printf("%3.2f resolved by Largest Sum of Log(CharFreq) rule\n", (float) logcfstat / totalamb * 100);
    }
    exit(0);
}
