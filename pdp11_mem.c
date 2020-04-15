#include "pdp.h"
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

#define MEMSIZE (64*1024)

byte mem[MEMSIZE];

void test_mem() {
    byte b0 = 0x0a;
    //пишем байт, читаем байт
    b_write(2, b0);
    byte bres = b_read(2);
    printf("%02hhx = %02hhx\n", b0, bres);

   //пишем слово читаем слово
    word w0 = 0xcb0a;
    w_write(2,w0);
    word wres = w_read(2);
    printf("%04hx = %04hx\n", w0, wres);

    //пишем 2 байта читаем слово
    Adress a = 4;
    byte b1 = 0x0b;
    byte b2 = 0x0a;
    b_write(a, b2);
    b_write(a + 1, b1);
    word wrest = w_read(a);
    printf("br/ww \t %04hx=%02hhx%02hhx\n", wrest, b1, b2);

    //пишем слово читаем 2 байт
    Adress z = 4;
    byte b3 = 0x0b;
    byte b4 = 0x0a;
    word w = 0x0b0a;
    w_write(z, w);
    word b3res = b_read(z);
    word b4res = b_read(z+1);
    printf("ww/br \t %04hx=%02hhx%02hhx\n", w, b4res, b3res);
}

int main() {
    test_mem();
    return 0;
}

void w_write(Adress adr, word w ) {
    mem[adr]= (byte)(w & 0xFF);
    mem[adr+1]= (byte)((w >> 8) & 0xFF);
}

word w_read(Adress adr) {
    word w= ((b_read(adr+1) << 8) | (b_read(adr) & 0xFF));
    return w;
}

void b_write(Adress adr, byte b) {
    mem[adr]=b;
}
byte b_read(Adress adr) {
    return mem[adr];
}