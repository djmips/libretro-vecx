#ifndef __E6809_H
#define __E6809_H

/* user defined read and write functions */

extern unsigned char (*e6809_read8) (unsigned address);
extern void (*e6809_write8) (unsigned address, unsigned char data);

void e6809_reset(void);
unsigned e6809_sstep(unsigned irq_i, unsigned irq_f);

int e6809_statesz(void);
void e6809_serialize(char* ary);
void e6809_deserialize(char * ary);


#ifdef VECX_HOOKS
/* vecx-run debugger hooks. The hook pointers stay NULL unless a host installs one. */
enum {
	E6809_REG_A, E6809_REG_B, E6809_REG_DP, E6809_REG_CC,
	E6809_REG_X, E6809_REG_Y, E6809_REG_U, E6809_REG_S, E6809_REG_PC,
	E6809_REG_COUNT
};
unsigned e6809_get_reg(int reg);
void e6809_set_reg(int reg, unsigned value);

/* address of the instruction being executed, set before its opcode is fetched */
extern unsigned e6809_op_pc;

/* every CPU read (fetch = 1 for opcode and operand bytes read at PC) and write */
extern void (*e6809_read_hook) (unsigned address, unsigned data, int fetch);
extern void (*e6809_write_hook) (unsigned address, unsigned data);

/* undefined encodings, which the core otherwise ignores */
enum {
	E6809_ILLEGAL_OPCODE,  /* code = opcode */
	E6809_ILLEGAL_PAGE2,   /* code = byte after $10 */
	E6809_ILLEGAL_PAGE3,   /* code = byte after $11 */
	E6809_ILLEGAL_INDEXED, /* code = indexed postbyte */
	E6809_ILLEGAL_EXGTFR   /* code = register number */
};
extern void (*e6809_illegal_hook) (unsigned pc, unsigned code, int kind);
#endif

#endif
