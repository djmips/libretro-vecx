#ifndef __VECX_H
#define __VECX_H

enum {
	VECTREX_MHZ		= 1500000, /* speed of the vectrex being emulated */
	VECTREX_COLORS  = 128,     /* number of possible colors ... grayscale */

//	ALG_MAX_X		= 33000,
//	ALG_MAX_Y		= 41000
	ALG_MAX_X		= 43000,
	ALG_MAX_Y		= 45000
};

typedef struct vector_type {
	long x0, y0; /* start coordinate */
	long x1, y1; /* end coordinate */

	/* color [0, VECTREX_COLORS - 1], if color = VECTREX_COLORS, then this is
	 * an invalid entry and must be ignored.
	 */
	unsigned char color;
    int speed;
} vector_t;

extern unsigned char rom[8192];
extern unsigned char get_cart(unsigned pos);
extern void set_cart(unsigned pos, unsigned char data);
extern void set_cartSize(int size);

extern unsigned snd_regs[16];
extern unsigned alg_jch0;
extern unsigned alg_jch1;
extern unsigned alg_jch2;
extern unsigned alg_jch3;

extern long vector_draw_cnt;
extern long vector_erse_cnt;
extern vector_t *vectors_draw;
extern vector_t *vectors_erse;

int vecx_statesz();
int vecx_serialize(char* dst, int size);
int vecx_deserialize(char* dst, int size);

void vecx_reset (void);
int vecx_emu (long cycles);

/* Light pen in controller port 2: its sensor pulls VIA CA1 low while the
 * visible beam is within VECX_LIGHTPEN_REACH of it, and games read the edges
 * as IFR bit 1. Position is in ALG units (0..ALG_MAX_X, 0..ALG_MAX_Y, y=0 at
 * the top); active is 0 when the pen is lifted, which is what vecx_reset
 * leaves. Host input, not part of the saved state. `seen` counts the cycles
 * the pen saw light; vecx_reset zeroes it, otherwise only the host does. */
#define VECX_LIGHTPEN_REACH 0x100
extern int vecx_lightpen_active;
extern long vecx_lightpen_x;
extern long vecx_lightpen_y;
extern long vecx_lightpen_seen;
int vecx_get_ca1 (void);

/* VecVox: a SpeakJet speech chip in controller port 2. The game sends it
 * bytes at 9600 baud 8N1 on the port's button 1 line, which is PSG port A
 * bit 4: low while port A is an output (reg 7 bit 6) with that bit clear,
 * high while port A is an input. The core watches the PSG writes, decodes
 * the bytes and queues them in the chip's 64-byte FIFO; the host takes them
 * out as the chip would play them and drives the "room in the buffer" line.
 * Not part of the saved state. Off unless the host sets
 * vecx_vecvox_enabled (--vecvox). */
extern int vecx_vecvox_enabled;
void vecx_vecvox_line (int level, long cycle);   /* the data line changed */
void vecx_vecvox_clock (long cycle);             /* time passed with no change */
int  vecx_vecvox_pop (void);                     /* next byte, or -1 */
int  vecx_vecvox_count (void);                   /* bytes queued */
long vecx_vecvox_received (void);                /* bytes received since reset */
void vecx_vecvox_push (int byte);                /* as if received (tests, MCP) */


#ifdef VECX_HOOKS
/* vecx_emu() return bits; bit 0 already meant "a frame was rendered" */
enum { VECX_EMU_FRAME = 1, VECX_EMU_STOPPED = 2 };

/* called before every instruction; nonzero makes vecx_emu() return before
   running it, with VECX_EMU_STOPPED set */
extern int (*vecx_instruction_hook) (unsigned pc, int bank);

/* called when the 64K/256K cart bank changes (not by vecx_reset) */
extern void (*vecx_bank_hook) (int old_bank, int new_bank);

int vecx_get_bank (void);

/* every CPU write below $C000 (cart space), with the bank it lands in */
extern void (*vecx_cart_write_hook) (unsigned address, int bank, unsigned char data);

/* every CPU read below $C000; nonzero return means "use *data instead of the ROM
   byte" (a flash chip answering with its ID or its busy status) */
extern int (*vecx_cart_read_hook) (unsigned address, int bank, unsigned char *data);

/* nonzero: an external flash model owns cart writes; the built-in FLASH_SUPPORT
   sequence sniffing and ID answers are switched off */
extern int vecx_flash_external;
#endif

#endif
