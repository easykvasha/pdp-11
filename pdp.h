typedef unsigned char byte; //8 bit
typedef unsigned short int word; //16 bit
typedef word Adress; //16 bit

typedef unsigned char byte; //8 bit
typedef unsigned short int word; //16 bit
typedef word Adress; //16 bit

#define MEMSIZE (64*1024)
#define pc reg[7]

#define n_pr 0
#define y_xo 1                  //xo= XX Offset (8 bits, -128 to +127)
#define y_ss (1<<1)
#define y_dd (1<<2)
#define y_sup (1<<3)           // sup=NN Number (6 bits)


#define odata 0177566			// регистр данных дисплея
#define ostat 0177564			// регистр данных ввода


struct Command {
    word mask;
    word opcode;
    char * name;
    void (*do_func)();
    byte pr;
};

struct SSDD {
    word val;		//значение аргумента
    word adr;       //адрес аргумента
};



void b_write(Adress adr, byte b);   //
byte b_read(Adress adr);            // чтение и запись
void w_write(Adress adr, word b);   //
word w_read(Adress adr);            //
void test_mem();

void load_file();                 // чтение файла
void mem_dump(Adress adr, word n); // печать стандартного потока
void trace(int type, const char * str, ...);   // трассировка
void settings(char * opt);
void Flags(word w);
void run();
void print_reg();
void trace_reg(int type_reg, const char * str, ...);

void do_halt();    // остановка
void do_mov();     //
void do_add();     //
void do_movb();    //
void do_sob();     //
void do_clr();     //
void do_br();      //
void do_beq();     //
void do_bpl();     //
void do_tst();
void do_tstb();






extern int type, type_reg;
extern int N, Z, C;
extern int bt, xo;
extern word wd;
extern byte mem[MEMSIZE];
extern word reg[8];
extern struct Command cmmd[];
extern struct SSDD ss, dd, sup;
