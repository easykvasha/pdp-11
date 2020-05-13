#include <stdio.h>
#include <stdlib.h>
#include "pdp.h"
#include <string.h>
#include <stdarg.h>

int main(int arg, char * argm[]) {
    if (arg == 1) {
        settings(argm[0]);
        exit(1);
    }

    for (int i = 1; i < arg-1; i++) {
        if (0 == strcmp(argm[i], "-t"))
          type = 1;
        else if (0 == strcmp(argm[i], "-st")) {
            type= 1;
            type_reg = 1;
        } else {
            printf("Option can't be done\n");
            settings(argm[0]);
            exit(1);
        }
    }
    load_file(argm[arg-1]);
    run();

    return 0;
}
