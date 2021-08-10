#include <stdio.h>
#include <string.h>

void            main(int argc, char **argv)
{
    FILE           *in,
                   *out[7];
    char           *prefix = "chr",
                   *suffix = ".txt",
                    name[9],
                    charlen[2],
                    filename[81],
                    string[23];
    int             i,
                    len;

    if (argc != 2) {
        printf("MKLEX1 reads all entries from a sorted list of BIG-5 encoded Chinese words and\n");
        printf("divides them into seven files (CHR2.TXT - CHR8.TXT) according to word lengths.\n");
        printf("One-character words and words longer than eight characters will be ignored.\n");
        printf("This program is part of the MMSEG project for Chinese word identification.\n");
        printf("Last modified 4/29/96 by Chih-Hao Tsai. Email: c-tsai4@uiuc.edu\n\n");
        printf("Usage: MKLEX1 file\n\n");
        printf("Example: MKLEX1 c:\\word\\tsaiword.txt\n");
        exit(1);
    }
    strcpy(filename, argv[1]);

    printf("Open %s for reading ....", filename);
    if ((in = fopen(filename, "rt")) == NULL) {
        fprintf(stderr, "Cannot open %s.\n", filename);
        fcloseall();
        exit(1);
    }
    printf("OK\n");
    for (i = 0; i <= 6; i++) {
        strcpy(name, prefix);
        charlen[0] = i + 0x32;
        charlen[1] = 0x0;
        strcat(name, charlen);
        strcat(name, suffix);
        printf("Open %s for writing ....", name);
        if ((out[i] = fopen(name, "wt")) == NULL) {
            fprintf(stderr, "Cannot open %s.\n", name);
            fcloseall();
            exit(1);
        }
        printf("OK\n");
    }

    printf("Writing chr2.txt - chr8.txt, please wait.\n");

    while (!feof(in)) {
        fgets(string, 23, in);
        len = (strlen(string) - 1) / 2;
        if (2 <= len <= 8)
            fputs(string, out[len - 2]);
    }

    printf("Done. Close all files.\n");

    fcloseall();
}
