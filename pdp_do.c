#include <stdio.h>
#include <stdlib.h>
#include "pdp.h"
#include <string.h>
#include <stdarg.h>



word reg[8];
int xo;
struct SSDD ss, dd;



void do_halt() {
	
	trace (type,"\n\nHALTED!\n\n");
    int i;
    for (i = 0; i < 8; i += 2)
        trace_reg(type_reg,"R%d = %06o   ", i, reg[i]);
    trace_reg(type_reg, "\n");
    for (i = 1; i < 8; i += 2)
        trace_reg (type_reg, "R%d = %06o    ", i, reg[i]);
    trace_reg (type_reg, "\n");
	exit(0);
}

void do_mov() {

	w_write(dd.adr, ss.val);
	Flags(ss.val);
}

void do_add() {

    w_write(dd.adr, (dd.val + ss.val));
    Flags((dd.val + ss.val));
}

void do_movb() {

	b_write(dd.adr, (byte)ss.val);
	if (dd.adr == odata)
		fprintf(stderr, "%c %d\n", ss.val, ss.val);
    Flags(ss.val);
}

void do_sob() {

	reg[sup.adr]--;
	if (reg[sup.adr] != 0)
		pc = pc - 2 * sup.val;
}
		
void do_clr() {
	dd.val = 0;
	w_write(dd.adr, dd.val);
	N = 0;
	Z = 1;
	Flags(dd.val);

}

void do_br() {
	pc = pc + xo*2 ;
    printf("%o ", pc);
}

void do_beq() {
	if (Z == 1)
		do_br();
	else
        printf("%o ", (pc + xo*2));
}

void do_bpl() {
	if (N == 0)
		do_br();
	else
        printf("%o ", (pc - 6));
}
void do_tstb() {
    Flags(dd.val);
}



void do_tst() {
    Flags(dd.val);
}


