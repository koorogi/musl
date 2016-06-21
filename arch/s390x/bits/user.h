#undef __WORDSIZE
#define __WORDSIZE 64

typedef struct {
	unsigned long mask, addr;
} psw_t;

typedef struct {
	psw_t psw;
	unsigned long gprs[16];
	unsigned acrs[16];
	unsigned long orig_gpr2;
} s390_regs;

typedef union {
	float  f;
	double d;
	unsigned long ui;
	struct { unsigned hi, lo; } fp;
} freg_t;

typedef struct {
	unsigned fpc;
	unsigned pad;
	freg_t fprs[16];
} s390_fp_regs;

typedef s390_fp_regs elf_fpregset_t;
typedef s390_regs elf_gregset_t;

typedef struct {
	unsigned long cr[3];
} per_cr_words;

typedef struct {
	unsigned                       : 32;
	unsigned em_branching          : 1;
	unsigned em_instruction_fetch  : 1;
	unsigned em_storage_alteration : 1;
	unsigned em_gpr_alt_unused     : 1;
	unsigned em_store_real_address : 1;
	unsigned                       : 3;
	unsigned branch_addr_ctl       : 1;
	unsigned                       : 1;
	unsigned storage_alt_space_ctl : 1;
	unsigned                       : 21;
	unsigned long starting_addr, ending_addr;
} per_cr_bits;

typedef struct {
	unsigned short perc_atmid;
	unsigned long address;
	unsigned char access_id;
} per_lowcore_words;

typedef struct {
	unsigned perc_branching          : 1;
	unsigned perc_instruction_fetch  : 1;
	unsigned perc_storage_alteration : 1;
	unsigned perc_gpr_alt_unused     : 1;
	unsigned perc_store_real_address : 1;
	unsigned                         : 3;
	unsigned atmid_psw_bit_31        : 1;
	unsigned atmid_validity_bit      : 1;
	unsigned atmid_psw_bit_32        : 1;
	unsigned atmid_psw_bit_5         : 1;
	unsigned atmid_psw_bit_16        : 1;
	unsigned atmid_psw_bit_17        : 1;
	unsigned si                      : 2;
	unsigned long address;
	unsigned                         : 4;
	unsigned access_id               : 4;
} per_lowcore_bits;

typedef struct {
	union {
		per_cr_words words;
		per_cr_bits bits;
	} control_regs;

	unsigned single_step             : 1;
	unsigned instruction_fetch       : 1;
	unsigned                         : 30;

	unsigned long starting_addr, ending_addr;
	union {
		per_lowcore_words words;
		per_lowcore_bits bits;
	} lowcore;
} per_struct;

struct user_regs_struct {
	psw_t psw;
	unsigned long gprs[16];
	unsigned acrs[16];
	unsigned long orig_gpr2;
	s390_fp_regs fp_regs;
	per_struct per_info;
	unsigned long ieee_instruction_pointer;
};

struct user {
	struct user_regs_struct regs;
	unsigned long u_tsize, u_dsize, u_ssize;
	unsigned long start_code, start_stack;
	long signal;
	unsigned long u_ar0, magic;
	char u_comm[32];
};

#define PAGE_MASK            (~(PAGE_SIZE-1))
#define NBPG                 PAGE_SIZE
#define UPAGES               1
#define HOST_TEXT_START_ADDR (u.start_code)
#define HOST_STACK_END_ADDR  (u.start_stack + u.u_ssize * NBPG)

