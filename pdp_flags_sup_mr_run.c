#include <stdio.h>
#include <stdlib.h>
#include "pdp.h"
#include <string.h>
#include <stdarg.h>


byte mem[MEMSIZE];
word reg[8];
int N, Z, C, bt, xo;
struct SSDD  ss , dd, sup;
struct SSDD get_mr(word w, int z);



struct Command cmd[] = {
        {0177777, 0000000, "halt", do_halt, n_pr},
        {0170000, 0010000, "mov",  do_mov, y_dd | y_ss},
        {0170000, 0060000, "add",  do_add, y_dd | y_ss},
        {0170000, 0110000, "movb", do_movb, y_dd | y_ss},
        {0177000, 0077000, "sob",  do_sob, y_sup},
        {0017700, 0005000, "clr",  do_clr, y_dd},
        {0177400, 0000400, "br",   do_br, y_xo},
        {0177400, 0001400, "beq",  do_beq, y_xo},
        {0177400, 0100000, "bpl",  do_bpl, y_xo},
        {0177700, 0105700, "tstb", do_tstb, y_dd},
        {0177700, 0005700, "tst",  do_tst,  y_dd}
};


void Flags (word w) {

    N = (bt ? (w >> 15) : (w >> 7)) & 1;
    Z = (w == 0);
    C = (bt ? (w >> 16) : (w >> 8)) & 1;

    trace(type, N==1 ? "N": "-");
    trace(type, Z==1 ? "Z": "-");
    trace(type, C==1 ? "C": "-");
}

struct SSDD get_sup (word w) {
    struct SSDD res;
    res.val = w & 077;
    res.adr = (w >> 6) & 07;
    trace (type, "R%d, %o ", 1, pc - 2 * res.val);
    return res;
}

struct SSDD get_mr(word w, int z) {
    struct SSDD res;
    int rgtr = w & 7;					// номер регистра
    int mode = (w >> 3) & 7;		// номер моды
    word wd ;

    switch(mode) {

        case 0:
            res.adr = rgtr;
            res.val = reg[rgtr];
            trace (type, "R%o ", rgtr);
            break;

            case 1:
                res.adr = reg[rgtr];
                res.val = z ? b_read(res.adr) : w_read(res.adr);
                trace (type, "(R%o) ", rgtr);
                break;

                case 2:
                    res.adr = reg[rgtr];
                    res.val = z ? b_read(res.adr) : w_read(res.adr);

                    if (rgtr == 7) {
                        trace(type, "#%o ", res.val);
                    }else {
                        trace(type, "(R%o)+ ", rgtr);
                    }

                    if (z == 0 || rgtr == 6 || rgtr == 7)
                        reg[rgtr] += 2;

                    else {
                        reg[rgtr]++;
                    }
                    break;

                    case 3:
                        res.adr = reg[rgtr];
                        res.adr = w_read(reg[rgtr]);
                        reg[rgtr] += 2;
                        if (z == 0 || rgtr == 6 || rgtr == 7){
                            res.val = w_read(res.adr);
                            if (res.val != 0)
                            trace (type, "@#%o ", ostat);
                            else{
                                trace (type, "@#%o ", odata);
                            }
            }else
                {
                            res.val = b_read (res.adr);
                            trace (type, "@(R%o)+", rgtr);
            }
                        break;

                        case 4:
                            if (z == 0 || rgtr == 6 || rgtr == 7){
                                reg[rgtr] -= 2;
                                res.adr = reg[rgtr];
                                res.val = w_read (res.adr);
                            }else {
                                reg[rgtr]--;
                                res.adr = reg[rgtr];
                                res.val = b_read (res.adr);
            }
                            if (pc == 000016) {
                                trace (type,"\n\nHALTED!\n\n");
                                print_reg();
                                exit(1);
                            }else{
                                trace(type, "-(R%d) ", rgtr);
                                break;
                            }

                            case 5:
                                trace (type, "@-(R%o) ", rgtr);
                                reg[rgtr] -= 2;
                                res.adr = reg[rgtr];
                                res.adr = w_read(res.adr);
                                res.val = w_read(res.adr);
                                break;

                                case 6:
                                    wd= w_read(pc);
                                    pc += 2;
                                    res.adr = (reg[rgtr] + wd)& 0xFFFF;
                                    res.val = w_read(res.adr);

                                    if (rgtr == 7) {
                                        trace(type, "%o ", res.adr);
                                    }
                                    else
                                        trace (type, "%o(R%d) ", wd, rgtr);
                                    break;


                                    default:
                                        fprintf (stderr, "Mode %o not realised yet\n", mode);
                                        exit(1);

    }
    return res;

}


void run() {
    trace (type, "\n\nBEGIN TRACING\n");
    pc = 01000;
    w_write(ostat, 0xFF);
    trace (type, "\n");
    while (1) {
        word w = w_read(pc);
        trace (type, "%06o %06o  :  ", pc, w);
        pc += 2;
        int i;
        int size = sizeof(cmd)/sizeof(struct Command);
        struct Command cmmd;
        for (i = 0; i < size; i++) {
            cmmd = cmd[i];
            if ((cmmd.mask & w) == cmmd.opcode) {
                trace (type, "%s    ", cmmd.name);
                if (cmmd.pr & y_ss) {
                    ss = get_mr(w >> 6, w >> 15);
                    //trace (type, "\n ss = %o, %o\n", ss.val, ss.adr);
                }
                if (cmmd.pr & y_dd) {
                    dd = get_mr(w, w >> 15);
                    //trace (type, "\n dd = %o, %o\n", dd.val, dd.adr);
                }
                if (cmmd.pr & y_sup) {
                    sup = get_sup(w);
                    //trace (type, "\n sup = %o, %o\n", sup.val, sup.adr);
                }
                if (cmmd.pr & y_xo) {
                    xo = (char)(w & 255);
                }
                cmmd.do_func();
            }
        }
        //trace (type, "\n");
        print_reg();
    }
}
