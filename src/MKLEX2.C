#include <stdio.h>
#include <string.h>

void            main(int argc, char **argv)
{
    FILE           *in[7],
                   *out[7];
    char           *prefix = "chr",
                   *suffix = ".txt",
                   *suffix1 = ".lex",
                    name[9],
                    name1[9],
                    charlen[2],
                    pathname[81],
                    string[23],
                    head[3],
                    lasthead[3],
                    body[15];
    int             i,
                    j,
                    top,
                    same,
                    len,
                    totallen,
                    longest;

    if (argc != 2) {
        printf("MKLEX2 reads CHR2.TXT - CHR8.TXT created by MKLEX1.EXE and converts them to a\n");
        printf("more condensed form. The condensed files will be CHR2.LEX - CHR8.LEX.\n");
        printf("This program is part of the MMSEG project for Chinese word identification.\n");
        printf("Last modified 4/29/96 by Chih-Hao Tsai. Email: c-tsai4@uiuc.edu\n\n");
        printf("Usage: MKLEX2 path\n");
        printf("       path: where CHR2.TXT - CHR8.TXT can be found\n\n");
        printf("Example: MKLEX2 c:\\word\\\n");
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

    printf("Writing chr2.lex - chr8.lex, please wait.\n");

    for (i = 0; i <= 6; i++) {
        len = (i + 2) * 2;
        fseek(in[i], 0L, SEEK_SET);
        lasthead[0] = 0x00;
        top = 1;
        same = 1;
        totallen = 2;
        longest = 0;
        while (!feof(in[i])) {
            fgets(string, 23, in[i]);
            if (feof(in[i]))
                break;
            for (j = 0; j <= (len - 1); j++) {
                if (j <= 1)
                    head[j] = string[j];
                else
                    body[j - 2] = string[j];
            }
            head[2] = 0x00;
            body[len - 2] = 0x00;
            totallen = totallen + (len - 2);
            same = strcmp(head, lasthead);
            if (same == 0) {
                fputs(body, out[i]);
            } else {
                if (top == 0)
                    fputs("\n", out[i]);
                fputs(head, out[i]);
                fputs(body, out[i]);
                totallen = totallen - (len - 2);
                if (totallen > longest)
                    longest = totallen;
                totallen = len;
            }
            strcpy(lasthead, head);
            top = 0;
        }
        charlen[0] = i + 0x32;
        charlen[1] = 0x0;
        strcpy(name1, prefix);
        strcat(name1, charlen);
        strcat(name1, suffix1);
        printf("Info: The longest record of %s has %d bytes.\n", name1, longest);
    }

    printf("Done. Close all files.\n");

    fcloseall();
}
