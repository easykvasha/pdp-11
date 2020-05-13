#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "pdp.h"


int type=0;

void load_file(const char * fl) {
    FILE * test = fopen(fl, "r");
    Adress a;
    unsigned int bit;
    word wrd;

    if (test == NULL) {
        perror(fl);
        exit(1);
    }
    while (fscanf(test, "%hx%hx", &a, &wrd) == 2) {
        for(int i = 0; i < wrd; i++) {
            fscanf(test, "%x", &bit);
            b_write(a+i, bit);
        }
    }
    mem_dump(01000, wrd);

    fclose(test);
}

void mem_dump(Adress adr, word n){
    for (int i = 0; i <= n; i += 2) {
        printf("%06o : %06ho\n",adr + i, w_read(adr + i));
    }
}

void settings(char * opt) {
    printf("settings: %s [-t] FILE\n", opt);
    printf("-t\t - turn on tracing\n");
    printf("FILE\t - record file\n");
}



void trace(int type, const char * str, ...) {
    if (type == 0)
        return;
    if (type == 1) {
        va_list ap;
        va_start(ap, str);
        vprintf(str, ap);
        va_end(ap);
    }
}















