/* asmhead.nas */
struct BOOTINFO { /* 0x0ff0-0x0fff */
	char cyls; /* �u�[�g�Z�N�^�͂ǂ��܂Ńf�B�X�N��ǂ񂾂̂� */
	char leds; /* �u�[�g���̃L�[�{�[�h��LED�̏�� */
	char vmode; /* �r�f�I���[�h  ���r�b�g�J���[�� */
	char reserve;
	short scrnx, scrny; /* ��ʉ𑜓x */
	char *vram;
};
#define ADR_BOOTINFO	0x00000ff0

/* naskfunc.nas */
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
int io_in8(int port);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
int load_cr0(void);
void store_cr0(int cr0);
void asm_inthandler20(void);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);
unsigned int memtest_sub(unsigned int start, unsigned int end);

/* fifo.c */
struct FIFO32 {
	int *buf;
	int p, q, size, free, flags;
};
void fifo32_init(struct FIFO32 *fifo, int size, int *buf);
int fifo32_put(struct FIFO32 *fifo, int data);
int fifo32_get(struct FIFO32 *fifo);
int fifo32_status(struct FIFO32 *fifo);

/* graphic.c */
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);
void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void init_screen8(char *vram, int x, int y);
void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize);
#define COL8_000000		0
#define COL8_FF0000		1
#define COL8_00FF00		2
#define COL8_FFFF00		3
#define COL8_0000FF		4
#define COL8_FF00FF		5
#define COL8_00FFFF		6
#define COL8_FFFFFF		7
#define COL8_C6C6C6		8
#define COL8_840000		9
#define COL8_008400		10
#define COL8_848400		11
#define COL8_000084		12
#define COL8_840084		13
#define COL8_008484		14
#define COL8_848484		15

/* dsctbl.c */
struct SEGMENT_DESCRIPTOR {
	short limit_low, base_low;
	char base_mid, access_right;
	char limit_high, base_high;
};
struct GATE_DESCRIPTOR {
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar);
#define ADR_IDT			0x0026f800
#define LIMIT_IDT		0x000007ff
#define ADR_GDT			0x00270000
#define LIMIT_GDT		0x0000ffff
#define ADR_BOTPAK		0x00280000
#define LIMIT_BOTPAK	0x0007ffff
#define AR_DATA32_RW	0x4092
#define AR_CODE32_ER	0x409a
#define AR_INTGATE32	0x008e

/* int.c */
void init_pic(void);
void inthandler27(int *esp);
#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1

/* keyboard.c */
void inthandler21(int *esp);
void wait_KBC_sendready(void);
void init_keyboard(struct FIFO32 *fifo, int data0);
#define PORT_KEYDAT		0x0060
#define PORT_KEYCMD		0x0064

/* mouse.c */
struct MOUSE_DEC {
	unsigned char buf[3], phase;
	int x, y, btn;
};
void inthandler2c(int *esp);
void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);

/* memory.c */
#define MEMMAN_FREES		4090	/* ����Ŗ�32KB */
#define MEMMAN_ADDR			0x003c0000
struct FREEINFO {	/* ������� */
	unsigned int addr, size;
};
struct MEMMAN {		/* �������Ǘ� */
	int frees, maxfrees, lostsize, losts;
	struct FREEINFO free[MEMMAN_FREES];
};
unsigned int memtest(unsigned int start, unsigned int end);
void memman_init(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);
unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size);
int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size);

/* sheet.c */
#define MAX_SHEETS		256
struct SHEET {
	unsigned char *buf;
	int bxsize, bysize, vx0, vy0, col_inv, height, flags;
	struct SHTCTL *ctl;
};
struct SHTCTL {
	unsigned char *vram, *map;
	int xsize, ysize, top;
	struct SHEET *sheets[MAX_SHEETS];
	struct SHEET sheets0[MAX_SHEETS];
};
struct SHTCTL *shtctl_init(struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize);
struct SHEET *sheet_alloc(struct SHTCTL *ctl);
void sheet_setbuf(struct SHEET *sht, unsigned char *buf, int xsize, int ysize, int col_inv);
void sheet_updown(struct SHEET *sht, int height);
void sheet_refresh(struct SHEET *sht, int bx0, int by0, int bx1, int by1);
void sheet_slide(struct SHEET *sht, int vx0, int vy0);
void sheet_free(struct SHEET *sht);

/* timer.c */
#define MAX_TIMER		500
struct TIMER {
	struct TIMER *next;
	unsigned int timeout, flags;
	struct FIFO32 *fifo;
	int data;
};
struct TIMERCTL {
	unsigned int count, next;
	struct TIMER *t0;
	struct TIMER timers0[MAX_TIMER];
};
extern struct TIMERCTL timerctl;
void init_pit(void);
struct TIMER *timer_alloc(void);
void timer_free(struct TIMER *timer);
void timer_init(struct TIMER *timer, struct FIFO32 *fifo, int data);
void timer_settime(struct TIMER *timer, unsigned int timeout);
void inthandler20(int *esp);

/* Final Task Files */

/* PacMan.c */
	/* figures size */
#define MAP_SIZE			110
#define	GHOST_SIZE			50
#define PAC_SIZE			50
#define SUPER_BEAN_SIZE 	8
#define NORMAL_BEAN_SIZE	4
	/* speed values */
#define PAC_ATTACK_SPEED	16
#define PAC_DEFEND_SPEED	10
#define GHOST_ATTACK_SPEED	12
#define GHOST_DEFEND_SPEED	12
	/* prop num */
#define GHOST_NUM			4
#define SUPER_BEAN_NUM		5
	/* Map */

#define boundary_flag		'#'
#define path_flag			'^'
#define normal_bean_flag	'.'
#define super_beans_flag	'O'

#define START_BGC COL8_000000
#define TXT_BGC COL8_FFFFFF
#define TXT_EDGE_BGC COL8_000000
#define LOGO_BGC COL8_000000
#define LOGO_MAIN_C COL8_00FFFF
#define TIP_C COL8_FFFFFF
#define BACK_BGC COL8_000000
#define TASK_BGC COL8_C6C6C6
#define PAC_C COL8_FFFF00
#define GHO_C 1
#define GAME_BGC COL8_000000
#define THRID_C COL8_000000
#define MAP_C COL8_FF00FF



#define MAZE_W 268
#define MAZE_H 266

#define GAM_X_MIN (1024)/2-500
#define GAM_X_MAX (1024)/2+300
#define GAM_Y_MIN (742)/2-340
#define GAM_Y_MAX (742)/2+360
//Ghost
struct Ghost{ 
	struct SHEET* sht_gho;
	unsigned char buf_gho[GHOST_SIZE][GHOST_SIZE];
	int status;//[0,1] for ['living','dead']
	int suit;
	unsigned char color;
	int cur_x,cur_y;
	int next_x,next_y;
	int direction;			/* [1,-1,2,-2] for	*/
	int speed;
};
//Pac-Man
struct Pac_Man{
	struct SHEET* sht_pac;
	unsigned char buf_pac[PAC_SIZE][PAC_SIZE];
	int status;//[0,1] for ['living','dead']
	int suit;
	unsigned char color;
	int cur_x,cur_y;
	int next_x,next_y;
	int direction;			/* [1,-1,2,-2] for	*/
	int speed;
};
//Game: The one used to control everything
struct Game{
	/* status-value */
	int game_status;		/* [-1,1,0] for ['to boot','running','game over'] */
	int pac_status;			/*  [1,0]   for 	 ['attack','defend']	      */
	int ghost_status;		/*  [1,0]   for 	 ['attack','defend']		  */
	/* numerical-value */
	int pac_speed;
	int ghost_speed;
	int super_beans_num;
	int ghost_num;
	int ture_man_time;
	int using_time;
	int score;

	char*game_map;
	/* structural-value */
	struct Pac_Man* pacman;				/* Only one instance */
	struct Ghost*   ghosts[GHOST_NUM];	/*  */
};

struct Bean{
	struct SHEET* sht_bea;
	unsigned char buf_bea[PAC_SIZE][PAC_SIZE];
	int kind;
	unsigned char color;
	int cur_x,cur_y;
	int next_x,next_y;
};

void Game_Init(struct Game* game, struct MEMMAN *memman, struct Pac_Man* pac_man, struct Ghost* ghost);
void Game_Start(struct Game* game, struct Pac_Man* pacman, struct Ghost* ghost);
void Game_Over(struct Pac_Man* pac_man, struct Ghost* ghost);

void Pac_Init(struct SHTCTL *shtctl, struct Game* game, struct Pac_Man* pac_man,int start_x, int start_y);
void Gho_Init(struct SHTCTL *shtctl, struct Game* game, struct Ghost* ghost,int start_x,int start_y,int gho_c);

void Pac_Show(struct Pac_Man* pac_man);
void Gho_Show(struct Ghost* ghost);
void Pac_Move(struct SHEET*sht,struct Pac_Man* pac_man);
void Gho_Move(struct SHEET*sht,struct Ghost* ghost);

int Pac_Meet_Ghost(struct Game* game,struct Pac_Man* pac_man, struct Ghost* ghost);
int Pac_Meet_Bean(struct SHEET*sht, struct Game* game,struct Pac_Man* pac_man, struct Bean* bean,int xsize, int ysize);
int Gho_Meet_Bean(struct SHEET*sht, struct Game* game,struct Ghost* ghost, struct Bean* bean,int xsize, int ysize);
void Pac_Die(struct Pac_Man* pac_man,struct Ghost* ghost);
void Gho_Die(struct Ghost* ghost);
void Set_Status(struct Ghost* ghost,int status);

void Creat_Baen_Randomly(struct SHEET*sht ,struct Bean* bean,int xsize, int ysize);
void Pac_Eat_Super_Bean();