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


#ifdef VECX_HOOKS
/* vecx_emu() return bits; bit 0 already meant "a frame was rendered" */
enum { VECX_EMU_FRAME = 1, VECX_EMU_STOPPED = 2 };

/* called before every instruction; nonzero makes vecx_emu() return before
   running it, with VECX_EMU_STOPPED set */
extern int (*vecx_instruction_hook) (unsigned pc, int bank);

/* called when the 64K/256K cart bank changes (not by vecx_reset) */
extern void (*vecx_bank_hook) (int old_bank, int new_bank);

int vecx_get_bank (void);
#endif

#endif
