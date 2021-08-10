#include <stdio.h>
#include <string.h>

void            main(int argc, char **argv)
{
    FILE           *in[7],
                   *out[7];
    char           *prefix = "chr",
                   *suffix = ".lex",
                   *suffix1 = ".inx",
                   *ptr,
                    name[9],
                    name1[9],
                    charlen[2],
                    pathname[81],
                    string[3000],
                    head[3];
    int             i,
                    j,
                    wordlen,
                    length,
                    counter;
    long            offset;

    if (argc != 2) {
        printf("MKLEX3 reads CHR2.LEX - CHR8.LEX created by MKLEX2.EXE and produces an index\n");
        printf("file for each of them. The index files will be CHR2.INX - CHR8.INX.\n");
        printf("This program is part of the MMSEG project for Chinese word identification.\n");
        printf("Last modified 4/29/96 by Chih-Hao Tsai. Email: c-tsai4@uiuc.edu\n\n");
        printf("Usage: MKLEX3 path\n");
        printf("       path: where CHR2.TXT - CHR8.TXT can be found\n\n");
        printf("Example: MKLEX3 c:\\word\\\n");
        exit(1);
    }
    strcpy(pathname, argv[1]);

    for (i = 0; i <= 6; i++) {
        charlen[0] = i + 0x32;
        charlen[1] = 0x0;

        strcpy(name, prefix);
        strcat(name, charlen);
        strcat(name, suffix);
        printf("Open %s for reading ....", name);
        if ((in[i] = fopen(name, "rt")) == NULL) {
            fprintf(stderr, "Cannot open %s.\n", name);
            fcloseall();
            exit(1);
        }
        printf("OK\n");

        strcpy(name1, prefix);
        strcat(name1, charlen);
        strcat(name1, suffix1);
        printf("Open %s for writing ....", name1);
        if ((out[i] = fopen(name1, "wt")) == NULL) {
            fprintf(stderr, "Cannot open %s.\n", name1);
            fcloseall();
            exit(1);
        }
        printf("OK\n");
    }

    printf("Writing chr2.inx - chr8.inx, please wait.\n");

    for (i = 0; i <= 6; i++) {
        offset = 0L;
        wordlen = (i + 2) * 2;
        fseek(in[i], 0L, SEEK_SET);
        while (!feof(in[i])) {
            fgetpos(in[i], &offset);
            fgets(string, 3000, in[i]);
            length = strlen(string);
            ptr = strrchr(string, 0x0d);
            if (ptr)
                length -= 1;
            counter = (length - 2) / (wordlen - 2);
            strncpy(head, string, 2);
            head[2] = 0x00;
            fprintf(out[i], "%s", head);
            fprintf(out[i], "%6ld", offset);
            fprintf(out[i], "%3d\n", counter);
        }
    }

    printf("Done. Close all files.\n");

    fcloseall();
}
