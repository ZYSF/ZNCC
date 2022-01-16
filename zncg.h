#ifndef CCBGENERIC_H
#define CCBGENERIC_H

#include "ccb.h"

#ifdef CCBGENERIC_IMPLEMENTATION

//#define CCB_TARGET_ENVPREFIX "CCB_"

static int ccb_target_fam;// = -1;
static int ccb_target_wordsize_val;// = -1;
static int ccb_target_callconv_val;// = -1;

void ccb_target_init(ccb_t* ccb) {
	const char* x;

    //fprintf(stderr, "XXX: INITIALISING TARGET\n");
    ccb_target_fam = -1;
    ccb_target_wordsize_val = -1;
    ccb_target_callconv_val = -1;

    x = getenv(/*CCB_TARGET_ENVPREFIX*/ "CCB_FAMILY");

    //fprintf(stderr, "XXX: INITIALISING TARGET ...\n");

    //fprintf(stderr, "XXX: INITIALISING TARGET GOT '%s'\n", x);
    if (x != NULL) {

    //fprintf(stderr, "XXX: INITIALISING TARGET A\n");
        if (strcmp(x, "x86") == 0 || strcmp(x, "X86") == 0) {
            ccb_target_fam = CCB_ARCH_FAMILY_X86;
        }
        else if (strcmp(x, "arm") == 0 || strcmp(x, "ARM") == 0) {
            ccb_target_fam = CCB_ARCH_FAMILY_ARM;
        }
        else if (strcmp(x, "risc-v") == 0 || strcmp(x, "RISC-V") == 0 || strcmp(x, "riscv") == 0 || strcmp(x, "RISCV") == 0) {
            ccb_target_fam = CCB_ARCH_FAMILY_RISCV;
        }
        else if (strcmp(x, "generic") == 0 || strcmp(x, "GENERIC") == 0) {
            ccb_target_fam = CCB_ARCH_FAMILY_GENERIC;
        }
        else if (strcmp(x, "gen1") == 0 || strcmp(x, "GEN1") == 0) {
            ccb_target_fam = CCB_ARCH_FAMILY_GEN1;
        }
        else {
            ccb_compile_error(ccb, "Invalid value for environment variable (%s=\"%s\")", /* CCB_TARGET_ENVPREFIX */ "CCB_FAMILY", x);
        }
    }
    else {

    //fprintf(stderr, "XXX: INITIALISING TARGET B\n");
        ccb_target_fam = CCB_ARCH_FAMILY_X86; // GENERIC; // X86; // TODO: Guess based on build environment or use compiler flag? (Maybe use GENERIC by default in the future?)
    }

    //fprintf(stderr, "XXX: HALF INITIALISING TARGET\n");

    x = getenv(/* CCB_TARGET_ENVPREFIX */ "CCB_CALLCONV");
    if (x != NULL) {
        if (strcmp(x, "standard") == 0 || strcmp(x, "STANDARD") == 0) {
            ccb_target_callconv_val = CCB_TARGET_CALLCONV_STANDARD;
        }
        else if (strcmp(x, "windows") == 0 || strcmp(x, "WINDOWS") == 0) {
            ccb_target_callconv_val = CCB_TARGET_CALLCONV_WINDOWS;
        }
        else {
            ccb_compile_error(ccb, "Invalid value for environment variable (%s=\"%s\")", /* CCB_TARGET_ENVPREFIX */ "CCB_CALLCONV", x);
        }
    }
    else {
        ccb_target_callconv_val = CCB_TARGET_CALLCONV_STANDARD;
    }

	x = getenv(/* CCB_TARGET_ENVPREFIX */ "CCB_WORDSIZE");
	if (x != NULL) {
		if (strcmp(x, "16") == 0) {
			ccb_target_wordsize_val = 16;
		}
		else if (strcmp(x, "32") == 0) {
			ccb_target_wordsize_val = 32;
		}
		else if (strcmp(x, "64") == 0) {
			ccb_target_wordsize_val = 64;
		}
		else {
			ccb_compile_error(ccb, "Invalid value for environment variable (%s=\"%s\"): Only 16, 32 and 64-bit word sizes are available", /* CCB_TARGET_ENVPREFIX */ "CCB_ARCH_WORDSIZE", x);
		}
	}
	switch (ccb_target_fam) {
	case CCB_ARCH_FAMILY_X86:
		if (ccb_target_wordsize_val == -1) {
			ccb_target_wordsize_val = 64;
		}
		break;
	case CCB_ARCH_FAMILY_ARM:
	case CCB_ARCH_FAMILY_RISCV:
		if (ccb_target_wordsize_val == -1) {
			ccb_target_wordsize_val = 64;
		}
		if (ccb_target_wordsize_val < 32) {
			ccb_compile_error(ccb, "Invalid value for environment variable (%s=\"%s\"): Not available in the ARM or RISC-V targets", /* CCB_TARGET_ENVPREFIX */ "CCB_ARCH_WORDSIZE", x);
		}
		break;
	case CCB_ARCH_FAMILY_GEN1:
		if (ccb_target_wordsize_val == -1) {
			ccb_target_wordsize_val = 64;
		}
		if (ccb_target_wordsize_val != 64) {
			ccb_compile_error(ccb, "Invalid value for environment variable (%s=\"%s\"): Not available in the GEN1 target", /* CCB_TARGET_ENVPREFIX */ "CCB_WORDSIZE", x);
		}
		break;
	case CCB_ARCH_FAMILY_GENERIC:
		if (ccb_target_wordsize_val == -1) {
			ccb_target_wordsize_val = 64;
		}
		break;
	default:
		ccb_compile_error(ccb, "Internal Error (missing case?)");
	}

    //fprintf(stderr, "XXX: DONE INITIALISING TARGET\n");
}

int ccb_target_family(ccb_t* ccb) {
	switch (ccb_target_fam) {
	case CCB_ARCH_FAMILY_X86:
	case CCB_ARCH_FAMILY_ARM:
	case CCB_ARCH_FAMILY_RISCV:
	case CCB_ARCH_FAMILY_GENERIC:
	case CCB_ARCH_FAMILY_GEN1:
		return ccb_target_fam;
	default:
		ccb_compile_error(ccb, "Internal Error: arch_init() hasn't completed yet");
	}
}

int ccb_target_wordsize(ccb_t* ccb) {
    return ccb_target_wordsize_val;
}

int ccb_target_callconv(ccb_t* ccb) {
    return ccb_target_callconv_val;
}

size_t ccb_target_type_size_char(ccb_t* ccb) {
	return 1;
}

size_t ccb_target_type_size_short(ccb_t* ccb) {
	return 2;
}

size_t ccb_target_type_size_int(ccb_t* ccb) {
	if (ccb_target_wordsize(ccb) >= 32) {
		return 4;
	}
	else {
		return 2;
	}
}

size_t ccb_target_type_size_long(ccb_t* ccb) {
	if (ccb_target_wordsize(ccb) >= 64) {
		return 8;
	}
	else {
		return 4;
	}
}

size_t ccb_target_type_size_llong(ccb_t* ccb) {
	return 8;
}

size_t ccb_target_type_size_float(ccb_t* ccb) {
	return 4;
}

size_t ccb_target_type_size_double(ccb_t* ccb) {
	return 8;
}

size_t ccb_target_type_size_ldouble(ccb_t* ccb) {
	return 8;
}

size_t ccb_target_type_size_pointer(ccb_t* ccb) {
	return ccb_target_wordsize(ccb) / 8;
}

size_t ccb_target_alignment(ccb_t* ccb) {
	return 16;
}

int ccb_target_callregisters(ccb_t* ccb) {
	switch (ccb_target_family(ccb)) {
	case CCB_ARCH_FAMILY_X86:
		return (ccb_target_callconv(ccb) == CCB_TARGET_CALLCONV_WINDOWS) ? 4 : 6; // TODO: This only applies to 64-bit mode
	case CCB_ARCH_FAMILY_ARM:
		return 4; // TODO: This might only apply to 32-bit mode
	case CCB_ARCH_FAMILY_RISCV:
		return 8; // Pretty sure this is the same for all RISC-V ISAs
	case CCB_ARCH_FAMILY_GENERIC:
		return 6; // Currently...
	case CCB_ARCH_FAMILY_GEN1:
		return 4; // With standard-ish ABI, might use about 8-ish in the future
	default:
		ccb_compile_error(ccb, "Target Error: arch_callregisters() got unknown arch");
		return -1; // Unreachable
	}
}

const char* ccb_target_callregister(ccb_t* ccb, int idx) {
	if (idx < 0 || idx >= ccb_target_callregisters(ccb)) {
		ccb_compile_error(ccb, "Target Error: arch_callregister(cc,%d) received bad argument", idx);
	}

	switch (ccb_target_family(ccb)) {
	case CCB_ARCH_FAMILY_X86:
        if (ccb_target_callconv(ccb) == CCB_TARGET_CALLCONV_WINDOWS) {
            switch (idx) {
            case 0:
                return "rcx";
            case 1:
                return "rdx";
            case 2:
                return "r8";
            case 3:
                return "r9";
            }
        }
        else {
            switch (idx) {
            case 0:
                return "rdi";
            case 1:
                return "rsi";
            case 2:
                return "rdx";
            case 3:
                return "rcx";
            case 4:
                return "r8";
            case 5:
                return "r9";
            }
        }
        ccb_compile_error(ccb, "Target Error: Register lookup failed");
	case CCB_ARCH_FAMILY_ARM:
		switch (idx) {
		case 0:
			return "r0";
		case 1:
			return "r1";
		case 2:
			return "r2";
		case 3:
			return "r3";
		}
        ccb_compile_error(ccb, "Target Error: Register lookup failed");
	case CCB_ARCH_FAMILY_RISCV:
		switch (idx) {
		case 0:
			return "a0"; // Also known as 'x10'
		case 1:
			return "a1";
		case 2:
			return "a2";
		case 3:
			return "a3";
		case 4:
			return "a4";
		case 5:
			return "a5";
		case 6:
			return "a6";
		case 7:
			return "a7"; // .. 'x17'
		}
        ccb_compile_error(ccb, "Target Error: Register lookup failed");
	case CCB_ARCH_FAMILY_GENERIC:
		switch (idx) {
		case 0:
			return "r7";
		case 1:
			return "r6";
		case 2:
			return "r2";
		case 3:
			return "r1";
		case 4:
			return "r8";
		case 5:
			return "r9";
		}
        ccb_compile_error(ccb, "Target Error: Register lookup failed");
	case CCB_ARCH_FAMILY_GEN1:
		switch (idx) {
		case 0:
			return "$r0";
		case 1:
			return "$r1";
		case 2:
			return "$r2";
		case 3:
			return "$r3";
		}
		ccb_compile_error(ccb, "Target Error: Register lookup failed");
	default:
        ccb_compile_error(ccb, "Target Error: Register lookup failed");
		return "ERROR?"; // Unreachable
	}
}


const char* ccb_target_r0(ccb_t* ccb) {
	switch (ccb_target_family(ccb)) {
	case CCB_ARCH_FAMILY_X86:
		return "rax";
    case CCB_ARCH_FAMILY_GEN1:
        return "$r0";
    case CCB_ARCH_FAMILY_GENERIC:
        return "r0";
    case CCB_ARCH_FAMILY_RISCV:
        return "a0";
	default:
		return "todo";
	}
}

const char* ccb_target_r1(ccb_t* ccb) {
	switch (ccb_target_family(ccb)) {
	case CCB_ARCH_FAMILY_X86:
		return "rcx";
	case CCB_ARCH_FAMILY_GEN1:
		return "$r1";
    case CCB_ARCH_FAMILY_GENERIC:
        return "r1";
    case CCB_ARCH_FAMILY_RISCV:
        return "a1";
	default:
		return "todo";
	}
}

const char* ccb_target_r15(ccb_t* ccb) {
	switch (ccb_target_family(ccb)) {
	case CCB_ARCH_FAMILY_X86:
		return "r15";
	case CCB_ARCH_FAMILY_GEN1:
		return "$rf";
    case CCB_ARCH_FAMILY_GENERIC:
        return "r15";
    case CCB_ARCH_FAMILY_RISCV:
        return "t0";
	default:
		return "todo";
	}
}

const char* ccb_target_sp(ccb_t* ccb) {
	switch (ccb_target_family(ccb)) {
	case CCB_ARCH_FAMILY_X86:
		return "rsp";
	case CCB_ARCH_FAMILY_GEN1:
		return "$rstack";
    case CCB_ARCH_FAMILY_RISCV:
        return "sp"; // Also known as 'x2'
	default:
		return "r4";
	}
}

const char* ccb_target_bp(ccb_t* ccb) {
	switch (ccb_target_family(ccb)) {
	case CCB_ARCH_FAMILY_X86:
		return "rbp";
	case CCB_ARCH_FAMILY_GEN1:
		return "$rbase";
    case CCB_ARCH_FAMILY_RISCV:
        return "x8";//"fp"; // Also known as 's0' (or `x8`)
	default:
		return "r5";
	}
}

static int ccb_target_gen_asmfmt_cached = 0;

int ccb_target_asmfmt(ccb_t* ccb) {
    if (ccb_target_gen_asmfmt_cached == 0) {
        const char* val = getenv(/* CCB_TARGET_ENVPREFIX */ "CCB_ASMFMT");
        if (val == NULL) {
            ccb_target_gen_asmfmt_cached = CCB_TARGET_ASMFMT_GAS;
        } else if (strcmp(val, "") == 0 || strcmp(val, "gas") == 0) {
            ccb_target_gen_asmfmt_cached = CCB_TARGET_ASMFMT_GAS;
        }
        else if (strcmp(val, "fasm") == 0) {
            ccb_target_gen_asmfmt_cached = CCB_TARGET_ASMFMT_FASM;
        }
        else if (strcmp(val, "raw") == 0) {
            ccb_target_gen_asmfmt_cached = CCB_TARGET_ASMFMT_RAW;
        }
        else {
            fprintf(stderr, "Invalid value for CC_ASMFMT (\"%s\")\n", val);
            return -1;
        }
    }

    return ccb_target_gen_asmfmt_cached;
}

static int ccb_target_binfmt_cached = 0;

int ccb_target_binfmt(ccb_t* ccb) {
    if (ccb_target_binfmt_cached == 0) {
        const char* val = getenv(/* CCB_TARGET_ENVPREFIX */ "CCB_BINFMT");
        if (val == NULL || strcmp(val, "") == 0 || strcmp(val, "elf") == 0) {
            ccb_target_binfmt_cached = CCB_TARGET_BINFMT_ELF;
        }
        else if (strcmp(val, "flat") == 0) {
            ccb_target_binfmt_cached = CCB_TARGET_BINFMT_FLAT;
        }
        else {
            fprintf(stderr, "Invalid value for CC_BINFMT (\"%s\")\n", val);
            return -1;
        }
    }

    return ccb_target_binfmt_cached;
}

/*
static const char *registers[] = {
    "rdi", "rsi", "rdx",
    "rcx", "r8",  "r9"
};
*/

static void ccb_target_gen_expression(ccb_t* ccb, ccb_ast_t*);
static void ccb_target_gen_declaration_initialization(ccb_t* ccb, ccb_list_t*, int);
#ifdef _ZCC
/* TODO: Allow enough arguments for these! */
#define ccb_target_gen_emit(...)        do{fprintf(ccb->output, __VA_ARGS__); fprintf(ccb->output, "\n"); fflush(ccb->output);}while(0)
#define ccb_target_gen_emit_inline(...)        do{fprintf(ccb->output, __VA_ARGS__); fprintf(ccb->output, "\n"); fflush(ccb->output);}while(0)
/*
TODO: Better string handling... #define ccb_target_gen_emit(...)        ccb_target_gen_emit_impl(ccb, __LINE__, "\t" __VA_ARGS__)
*/
#else
#define ccb_target_gen_emit(...)        ccb_target_gen_emit_impl(ccb, __LINE__,  __VA_ARGS__)
#define ccb_target_gen_emit_inline(...) ccb_target_gen_emit_impl(ccb, __LINE__,      __VA_ARGS__)
#endif
#define ccb_target_gen_push(X)          ccb_target_gen_push_    (ccb, X, __LINE__)
#define ccb_target_gen_pop(X)           ccb_target_gen_pop_     (ccb, X, __LINE__)
#define ccb_target_gen_push_xmm(X)      ccb_target_gen_push_xmm_(ccb, X, __LINE__)
#define ccb_target_gen_pop_xmm(X)       ccb_target_gen_pop_xmm_ (ccb, X, __LINE__)

static int   ccb_target_gen_stack = 0;

static char* ccb_target_gen_label_break = NULL;
static char* ccb_target_gen_label_continue = NULL;
//static char* ccb_target_gen_label_break_store = NULL;
//static char* ccb_target_gen_label_continue_store = NULL;
static char* ccb_target_gen_label_switch = NULL;
//static char* ccb_target_gen_label_switch_store = NULL;

#ifdef _ZCC
#define ccb_target_gen_emit_impl(ccb,ln,...) fprintf(ccb->output, __VA_ARGS__); fprintf(ccb->output, "\n")
#else
void ccb_target_gen_emit_impl(ccb_t* ccb, int line, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int col = vfprintf(ccb->output, fmt, args);
    va_end(args);

    for (const char* p = fmt; *p; p++)
        if (*p == '\t')
            col += 8 - 1;

    col = (40 - col) > 0 ? (40 - col) : 2;

    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC || ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        fprintf(ccb->output, "%*c % 4d %d\n", col, ';', line, ccb_target_gen_stack);
    }
    else {
        fprintf(ccb->output, "%*c % 4d %d\n", col, '#', line, ccb_target_gen_stack);
    }
}
#endif

//static void ccb_target_gen_jump_save(ccb_t* ccb, char* lbreak, char* lcontinue) {
#define ccb_target_gen_jump_save(ccb,lbreak,lcontinue) \
    char* ccb_target_gen_label_break_store = ccb_target_gen_label_break; \
    char* ccb_target_gen_label_continue_store = ccb_target_gen_label_continue; \
    ccb_target_gen_label_break = lbreak; \
    ccb_target_gen_label_continue = lcontinue

//static void ccb_target_gen_jump_restore(ccb_t* ccb) {
#define ccb_target_gen_jump_restore(ccb) \
do { \
    ccb_target_gen_label_break = ccb_target_gen_label_break_store; \
    ccb_target_gen_label_continue = ccb_target_gen_label_continue_store; \
} while(0);

static int ccb_target_regcode(ccb_t* ccb, const char* reg) {
    if (!strcmp(reg, "rax")) {
        return 0;
    }
    else if (!strcmp(reg, "rcx")) {
        return 1;
    }
    else if (!strcmp(reg, "rdx")) {
        return 2;
    }
    else if (!strcmp(reg, "rbx")) {
        return 3;
    }
    else if (!strcmp(reg, "rsi")) {
        return 6;
    }
    else if (!strcmp(reg, "rdi")) {
        return 7;
    }
    else {
        //fprintf(stderr, "WARNING: Unimplemented regcode '%s'\n", reg);
        return -1;
    }
}

static void ccb_target_gen_push_(ccb_t* ccb, const char* reg, int line) {
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
        ccb_target_gen_emit_impl(ccb, line, "\twrite32 $rsp, %s, -8", reg); // TODO: Fix this mess.
        ccb_target_gen_emit_impl(ccb, line, "\txor $rscratch, $rscratch, $rscratch");
        ccb_target_gen_emit_impl(ccb, line, "\taddimm $rscratch, $rscratch, 32");
        ccb_target_gen_emit_impl(ccb, line, "\tshrz $rscratch, %s, $rscratch", reg);
        ccb_target_gen_emit_impl(ccb, line, "\twrite32 $rsp, $rscratch, -4");
        ccb_target_gen_emit_impl(ccb, line, "\taddimm $rsp, -8");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        if (ccb_target_wordsize(ccb) == 32) {
            ccb_target_gen_emit_impl(ccb, line, "\taddi sp, sp, -4");
            ccb_target_gen_emit_impl(ccb, line, "\tsw %s, 0(sp)", reg);
        } else  {
            ccb_target_gen_emit_impl(ccb, line, "\taddi sp, sp, -8");
            ccb_target_gen_emit_impl(ccb, line, "\tsd %s, 0(sp)", reg);
        }
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit_impl(ccb, line, "\tpushr %s", reg);
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit_impl(ccb, line, "\tpush %s", reg);
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW) {
        int regcode = ccb_target_regcode(ccb, reg);
        if (regcode < 0) {
            ccb_target_gen_emit_impl(ccb, line, "\tdata8 0x?? ; TODO: pop %s", reg);
        }
        else {
            ccb_target_gen_emit_impl(ccb, line, "\tdata8 0x%x ; push %s", 0x50 + regcode, reg);
        }
    }
    else {
        ccb_target_gen_emit_impl(ccb, line, "\tpush %%%s", reg);
    }
    ccb_target_gen_stack += (ccb_target_wordsize(ccb)/8);
}
static void ccb_target_gen_pop_(ccb_t* ccb, const char* reg, int line) {
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
        ccb_target_gen_emit_impl(ccb, line, "\tread32 %s, $rsp, 4", reg);
        ccb_target_gen_emit_impl(ccb, line, "\txor $rscratch, $rscratch, $rscratch");
        ccb_target_gen_emit_impl(ccb, line, "\taddimm $rscratch, $rscratch, 32");
        ccb_target_gen_emit_impl(ccb, line, "\tshl %s, %s, $rscratch", reg, reg);
        ccb_target_gen_emit_impl(ccb, line, "\tread32 %rscratch, $rsp, 0");
        ccb_target_gen_emit_impl(ccb, line, "\tor %s, %s, %rscratch", reg, reg);
        ccb_target_gen_emit_impl(ccb, line, "\taddimm $rsp, $rsp, 8");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        if (ccb_target_wordsize(ccb) == 32) {
            ccb_target_gen_emit_impl(ccb, line, "\tlw %s, 0(sp)", reg);
            ccb_target_gen_emit_impl(ccb, line, "\taddi sp, sp, 4");
        }
        else {
            ccb_target_gen_emit_impl(ccb, line, "\tld %s, 0(sp)", reg);
            ccb_target_gen_emit_impl(ccb, line, "\taddi sp, sp, 8");
        }
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit_impl(ccb, line, "\tpopr %s", reg);
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit_impl(ccb, line, "\tpop %s", reg);
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW) {
        int regcode = ccb_target_regcode(ccb, reg);
        if (regcode < 0) {
            ccb_target_gen_emit_impl(ccb, line, "\tdata8 0x?? ; TODO: pop %s", reg);
        }
        else {
            ccb_target_gen_emit_impl(ccb, line, "\tdata8 0x%x ; pop %s", 0x58 + regcode, reg);
        }
    }
    else {
        ccb_target_gen_emit_impl(ccb, line, "\tpop %%%s", reg);
    }
    ccb_target_gen_stack -= (ccb_target_wordsize(ccb)/8);
}
static void ccb_target_gen_push_xmm_(ccb_t* ccb, int r, int line) {
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit_impl(ccb, line, "\tsubrc r4, 8");
        ccb_target_gen_emit_impl(ccb, line, "\tsetrmf r4, f%d", r);
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        ccb_target_gen_emit_impl(ccb, line, "\taddi sp, sp, -8");
        ccb_target_gen_emit_impl(ccb, line, "\tfsd fa%d, 0(sp)", r);
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit_impl(ccb, line, "\tsub rsp, 8");
        ccb_target_gen_emit_impl(ccb, line, "\tmovsd [rsp], xmm%d", r);
    }
    else {
        ccb_target_gen_emit_impl(ccb, line, "\tsub $8, %%rsp");
        ccb_target_gen_emit_impl(ccb, line, "\tmovsd %%xmm%d, (%%rsp)", r);
    }
    ccb_target_gen_stack += 8;
}
static void ccb_target_gen_pop_xmm_(ccb_t* ccb, int r, int line) {
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit_impl(ccb, line, "\tsetfrm f%d, r4", r);
        ccb_target_gen_emit_impl(ccb, line, "\taddrc r4, 8");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        ccb_target_gen_emit_impl(ccb, line, "\tfld fa%d, 0(sp)", r);
        ccb_target_gen_emit_impl(ccb, line, "\taddi sp, sp, 8");
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit_impl(ccb, line, "\tmovsd xmm%d, [rsp]", r);
        ccb_target_gen_emit_impl(ccb, line, "\tadd rsp, 8");
    }
    else {
        ccb_target_gen_emit_impl(ccb, line, "\tmovsd (%%rsp), %%xmm%d", r);
        ccb_target_gen_emit_impl(ccb, line, "\tadd $8, %%rsp");
    }
    ccb_target_gen_stack -= 8;
}

static const char* ccb_target_gen_register_integer(ccb_t* ccb, ccb_data_type_t* type, char r) {
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) { // TODO...
        switch (type->size) {
        case 1: return (r == 'a') ? "r0x8" : "r1x8";
        case 2: return (r == 'a') ? "r0x16" : "r1x16";
        case 4: return (r == 'a') ? "v0" : "v1";
        case 8: return (r == 'a') ? "v0x64" : "v1x64";
        }
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        switch (type->size) {
        case 1: return (r == 'a') ? "r0x8" : "r1x8";
        case 2: return (r == 'a') ? "r0x16" : "r1x16";
        case 4: return (r == 'a') ? "r0x32" : "r1x32";
        case 8: return (r == 'a') ? "r0" : "r1";
        }
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        return (r == 'a') ? "a0" : "a1";
    }
    else {
        switch (type->size) {
        case 1: return (r == 'a') ? "al" : "cl";
        case 2: return (r == 'a') ? "ax" : "cx";
        case 4: return (r == 'a') ? "eax" : "ecx";
        case 8: return (r == 'a') ? "rax" : "rcx";
        }
    }
    ccb_compile_error(ccb, "Unexpected operand to ccb_target_gen_register_integer");
    return ""; // Unreachable?
}

static const char* ccb_target_gen_loadstorespec(ccb_t* ccb, ccb_data_type_t* type, bool isstore) {
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        switch (type->size) {
        case 1: return isstore ? "b" : (type->sign ? "b" : "bu");
        case 2: return isstore ? "h" : (type->sign ? "h" : "hu");
        case 4: return isstore ? "w" : (type->sign ? "w" : "wu");
        case 8: return "d"; // isstore ? "d" : (type->sign ? "b" : "bu");
        default:
            ccb_compile_error(ccb, "Unexpected operand to ccb_target_gen_loadstorespec");
            return ""; // Unreachable?
        }
    }
    else {
        return "";
    }
}

static void ccb_target_gen_load_global(ccb_t* ccb, ccb_data_type_t* type, char* label, int offset) {
    if (type->type == CCB_TYPE_ARRAY) {
        if (offset) {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
                ccb_target_gen_emit("xor $r0, $r0, $r0");
                ccb_target_gen_emit("addimm $r0, (%s + %d)", label, offset);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                //ccb_target_gen_emit("addi a0, zero, %s", label); // TODO: Use lui here?
                ccb_target_gen_emit("la a0, %s", label);
                ccb_target_gen_emit("addi a0, a0, %d", offset);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("setrcpc r0, %s, %d", label, offset);
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("lea rax, [%s + %d]", label, offset);
            }
            else {
                ccb_target_gen_emit("lea %s+%d(%%rip), %%rax", label, offset);
            }
        }
        else {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
                ccb_target_gen_emit("xor $r0, $r0, $r0");
                ccb_target_gen_emit("addimm $r0, (%s + %d)", label);
                //ccb_target_gen_emit("read32 $r0, $r1, 0");
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("setrc r0, %s", label);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                //ccb_target_gen_emit("addi a0, zero, %s", label); // TODO: Use lui here?
                ccb_target_gen_emit("la a0, %s", label); // TODO: Use lui here?
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("lea rax, [%s]", label);
            }
            else {
                ccb_target_gen_emit("lea %s(%%rip), %%rax", label);
            }
        }
        return;
    }

    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
        ccb_target_gen_emit("xor $r1, $r1, $r1");
        ccb_target_gen_emit("addimm $r1, $r1, (%s + %d)", label, offset);
        if (type->size <= 4) {
            ccb_target_gen_emit("read32 $r0, $r1, 0");
            if (type->size < 4) {
                int32_t mask = ((type->size == 1) ? 0xFF : 0xFFFF);
                ccb_target_gen_emit("xor $r1, $r1, $r1");
                ccb_target_gen_emit("addimm $r1, %d", mask); // TODO: This might not work out so well for 16-bit, must check.
                ccb_target_gen_emit("and $r0, $r0, $r1");
            }
        }
        else {
            ccb_target_gen_emit("read32 $r0, $r1, 4");
            ccb_target_gen_emit("shlz $r0, $r0, 32");
            ccb_target_gen_emit("read32 $r1, $r1, 0");
            ccb_target_gen_emit("or $r0, $r0, $r1"); // TODO: Sign bit may also interfere here.
        }
    }
    else {

        if (type->size < 4) {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("setrc r0, 0");
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                ccb_target_gen_emit("addi a0, zero, 0"); // Probably unneeded?
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("mov eax, 0");
            }
            else {
                ccb_target_gen_emit("mov $0, %%eax");
            }
        }

        const char* reg = ccb_target_gen_register_integer(ccb, type, 'a');

        if (offset) {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("setrcpcm %s, %s, %d", reg, label, offset);
            }
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                //ccb_target_gen_emit("addi t0, zero, %s", label); // TODO: Use lui? Better temporary register?
                ccb_target_gen_emit("la t0, %s", label); // TODO: Use lui? Better temporary register?
                //ccb_target_gen_emit("ld %s, t0, %d", reg, offset);
                ccb_target_gen_emit("ld %s, %d(t0)", reg, offset);
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("mov %s, [%s + %d]", reg, label, offset);
            }
            else {
                ccb_target_gen_emit("mov %s+%d(%%rip), %%%s", label, offset, reg);
            }
        }
        else {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("setrcm %s, %s", reg, label);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                //ccb_target_gen_emit("addi t0, zero, %s", label); // TODO: Use lui? Better temporary register?
                ccb_target_gen_emit("la t0, %s", label); // TODO: Use lui? Better temporary register?
                //ccb_target_gen_emit("ld %s, t0, 0", reg);
                ccb_target_gen_emit("ld %s, 0(t0)", reg);
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("mov %s, [%s]", reg, label);
            }
            else {
                ccb_target_gen_emit("mov %s(%%rip), %%%s", label, reg);
            }
        }
    }
}

static void ccb_target_gen_cast_int(ccb_t* ccb, ccb_data_type_t* type) {
    if (!ccb_ast_type_floating(ccb, type)) {
        if (type->type == CCB_TYPE_INT) {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86) {
                ccb_target_gen_emit("cltq"); // TODO: This is test only for now, needs to handle unsigned/other
            } /* TODO: Fix this elsewhere? May not be an issue on RISC-V since I think it auto-extends (bonus TODO: Check that!) */
        }
        return;
    }
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("intf r0, f0");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        ccb_target_gen_emit("fcvt.l.d a0, fa0, rtz");
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("cvttsd2si eax, xmm0");
    }
    else {
        ccb_target_gen_emit("cvttsd2si %%xmm0, %%eax");
    }
}

static void ccb_target_gen_cast_float(ccb_t* ccb, ccb_data_type_t* type) {
    if (ccb_ast_type_floating(ccb, type)) {
        return;
    }

    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("floati f0, r0");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        ccb_target_gen_emit("fcvt.d.l fa0, a0");
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("cvtsi2sd xmm0, eax");
    }
    else {
        ccb_target_gen_emit("cvtsi2sd %%eax, %%xmm0");
    }
}

static void ccb_target_gen_load_local(ccb_t* ccb, ccb_data_type_t* var, const char* base, int offset) {
    if (var->type == CCB_TYPE_ARRAY) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setrrpc r0, %s, %d", base, offset); // Set-register-to-register-plus-constant (we only want the address of array variable)
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("addi a0, %s, %d", base, offset); // Set-register-to-register-plus-constant (we only want the address of array variable)
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
            ccb_target_gen_emit("addimm $r0, %s, %d", base, offset);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("lea rax, [%s + %d]", base, offset);
        }
        else {
            ccb_target_gen_emit("lea %d(%%%s), %%rax", offset, base);
        }
    }
    else if (var->type == CCB_TYPE_FLOAT) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setfrpcmx32 f0, %s, %d", base, offset); // Set-float-to-register-plus-constant's-memory-x32bit
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("flw ft0, %d(%s)", offset, base);
            ccb_target_gen_emit("fcvt.d.s fa0, ft0");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
            ccb_target_gen_emit("todo");
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("cvtps2pd xmm0, [%s + %d]", base, offset);
        }
        else {
             ccb_target_gen_emit("cvtps2pd %d(%%%s), %%xmm0", offset, base);
        }
    }
    else if (var->type == CCB_TYPE_DOUBLE || var->type == CCB_TYPE_LDOUBLE) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setfrpcm f0, %s, %d", base, offset); // Set-float-to-register-plus-constant's-memory
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("fld fa0, %d(%s)", offset, base);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
            ccb_target_gen_emit("todo");
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("movsd xmm0, [%s + %d]", base, offset);
        }
        else {
            ccb_target_gen_emit("movsd %d(%%%s), %%xmm0", offset, base);
        }
    }
    else {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
            ccb_target_gen_emit("xor $r1, $r1, $r1");
            ccb_target_gen_emit("addimm $r1, %s, %d", base, offset);
            if (var->size <= 4) {
                ccb_target_gen_emit("read32 $r0, $r1, 0");
                if (var->size < 4) {
                    int32_t mask = ((var->size == 1) ? 0xFF : 0xFFFF);
                    ccb_target_gen_emit("xor $r1, $r1, $r1");
                    ccb_target_gen_emit("addimm $r1, %d", mask); // TODO: This might not work out so well for 16-bit, must check.
                    ccb_target_gen_emit("and $r0, $r0, $r1");
                }
            }
            else {
                ccb_target_gen_emit("read32 $r0, $r1, 4");
                ccb_target_gen_emit("shlz $r0, $r0, 32");
                ccb_target_gen_emit("read32 $r1, $r1, 0");
                ccb_target_gen_emit("or $r0, $r0, $r1"); // TODO: Sign bit may also interfere here.
            }
        }
        else {
            const char* reg = ccb_target_gen_register_integer(ccb, var, 'c');
            const char* spec = ccb_target_gen_loadstorespec(ccb, var, false);
            if (var->size < 4) { // TODO: Should this be 8 (or normal int/pointer size) ??
                if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                    ccb_target_gen_emit("setrc r0, 0");
                }
                else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                    // No need to clear top bits, handled in load instruction (??)
                }
                else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                    ccb_target_gen_emit("mov ecx, 0");
                }
                else {
                    ccb_target_gen_emit("mov $0, %%ecx");
                }
            }

            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
                ccb_target_gen_emit("peek32 %s, %s, %d", reg, base, offset);
                ccb_target_gen_emit("move v0, v1");
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("setrrpcm %s, %s, %d", reg, base, offset);
                ccb_target_gen_emit("setrr r0, r1");
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                ccb_target_gen_emit("l%s %s, %d(%s)", spec, reg, offset, base);
                ccb_target_gen_emit("addi a0, a1, 0");
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("mov %s, [%s + %d]", reg, base, offset);
                ccb_target_gen_emit("mov rax, rcx");
            }
            else {
                ccb_target_gen_emit("mov %d(%%%s), %%%s", offset, base, reg);
                ccb_target_gen_emit("mov %%rcx, %%rax");
            }
        }
    }
}

static void ccb_target_gen_save_global(ccb_t* ccb, char* name, ccb_data_type_t* type, int offset) {
    const char* reg = ccb_target_gen_register_integer(ccb, type, 'a');
    if (offset) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setrpcmr %s, %d, %s", name, offset, reg);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            //ccb_target_gen_emit("addi t0, zero, %s", name); // TODO: lui?
            ccb_target_gen_emit("la t0, %s", name);
            ccb_target_gen_emit("sd %s, %d(t0)", reg, offset);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("mov [%s + %d], %s", name, offset, reg);
        }
        else {
            ccb_target_gen_emit("mov %%%s, %s+%d(%%rip)", reg, name, offset);
        }
    }
    else {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setcmr %s, %s", name, reg);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            //ccb_target_gen_emit("addi t0, zero, %s", name); // TODO: lui?
            ccb_target_gen_emit("la t0, %s", name); // TODO: lui?
            ccb_target_gen_emit("sd %s, 0(t0)", reg);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("mov [%s], %s", name, reg);
        }
        else {
            ccb_target_gen_emit("mov %%%s, %s(%%rip)", reg, name);
        }
    }
}

static void ccb_target_gen_save_local(ccb_t* ccb, ccb_data_type_t* type, int offset) {
    if (type->type == CCB_TYPE_FLOAT) {
        ccb_target_gen_push_xmm(0);
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setrpcmfx32 r5, %d, f0", offset);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("fcvt.s.d ft0, fa0");
            //ccb_target_gen_emit("fsw ft0, %d(fp)", offset);
            ccb_target_gen_emit("fsw ft0, %d(x8)", offset);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("unpcklpd xmm0, xmm0");
            ccb_target_gen_emit("cvtpd2ps xmm0, xmm0");
            ccb_target_gen_emit("movss [rbp + %d], xmm0", offset);
        }
        else {
            ccb_target_gen_emit("unpcklpd %%xmm0, %%xmm0");
            ccb_target_gen_emit("cvtpd2ps %%xmm0, %%xmm0");
            ccb_target_gen_emit("movss %%xmm0, %d(%%rbp)", offset);
        }
        ccb_target_gen_pop_xmm(0);
    }
    else if (type->type == CCB_TYPE_DOUBLE || type->type == CCB_TYPE_LDOUBLE) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setrpcmf r5, %d, f0", offset);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            //ccb_target_gen_emit("fsd fa0, %d(fp)", offset);
            ccb_target_gen_emit("fsd fa0, %d(x8)", offset);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("movsd [rbp + %d], xmm0", offset);
        }
        else {
            ccb_target_gen_emit("movsd %%xmm0, %d(%%rbp)", offset);
        }
    }
    else {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setrpcmr r5, %d, %s", offset, ccb_target_gen_register_integer(ccb, type, 'a'));
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            //ccb_target_gen_emit("sd %s, %d(fp)", ccb_target_gen_register_integer(ccb, type, 'a'), offset);
            ccb_target_gen_emit("sd %s, %d(x8)", ccb_target_gen_register_integer(ccb, type, 'a'), offset);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("mov [rbp + %d], %s", offset, ccb_target_gen_register_integer(ccb, type, 'a'));
        }
        else {
            ccb_target_gen_emit("mov %%%s, %d(%%rbp)", ccb_target_gen_register_integer(ccb, type, 'a'), offset);
        }
    }
}

static void ccb_target_gen_assignment_dereference_intermediate(ccb_t* ccb, ccb_data_type_t* type, int offset) {
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("setrrm r1, r4");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        ccb_target_gen_emit("ld a1, 0(sp)");
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("mov rcx, [rsp]");
    }
    else {
        ccb_target_gen_emit("mov (%%rsp), %%rcx");
    }

    const char* reg = ccb_target_gen_register_integer(ccb, type, 'c');

    if (offset) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setrpcmr r0, %d, %s", offset, reg);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("sd %s, %d(a0)", reg, offset);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("mov [rax + %d], %s", offset, reg);
        }
        else {
            ccb_target_gen_emit("mov %%%s, %d(%%rax)", reg, offset);
        }
    }
    else {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setrmr r0, %s", reg);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("sd %s, 0(a0)", reg);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("mov [rax], %s", reg);
        }
        else {
            ccb_target_gen_emit("mov %%%s, (%%rax)", reg);
        }
    }

    ccb_target_gen_pop(ccb_target_r0(ccb));
}

static void ccb_target_gen_assignment_dereference(ccb_t* ccb, ccb_ast_t* var) {
    ccb_target_gen_push(ccb_target_r0(ccb));
    ccb_target_gen_expression(ccb, var->unary.operand);
    ccb_target_gen_assignment_dereference_intermediate(ccb, var->unary.operand->ctype->pointer, 0);
}

static void ccb_target_gen_ensure_lva(ccb_t* ccb, ccb_ast_t* ast) {
    if (ast->variable.init)
        ccb_target_gen_declaration_initialization(ccb, ast->variable.init, ast->variable.off);
    ast->variable.init = NULL;
}

static void ccb_target_gen_pointer_arithmetic(ccb_t* ccb, char op, ccb_ast_t* left, ccb_ast_t* right) {
    ccb_target_gen_expression(ccb, left);
    ccb_target_gen_push(ccb_target_r0(ccb));
    ccb_target_gen_expression(ccb, right);

    int size = left->ctype->pointer->size;
    if (size > 1) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("mulrc r0, %d", size);
        }
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            if (size == 2 || size == 4 || size == 8) {
                ccb_target_gen_emit("slli a0, a0, %d", (size == 2) ? 1 : ((size == 4) ? 2 : 3));
            }
            else {
                //ccb_target_gen_emit("addi a1, zero, %d", size);
                ccb_target_gen_emit("li a1, %d", size);
                ccb_target_gen_emit("mul a0, a0, a1");
            }
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("imul rax, %d", size);
        }
        else {
            ccb_target_gen_emit("imul $%d, %%rax", size);
        }
    }

    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("setrr r1, r0");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        ccb_target_gen_emit("addi a1, a0, 0");
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("mov rcx, rax");
    }
    else {
        ccb_target_gen_emit("mov %%rax, %%rcx");
    }

    ccb_target_gen_pop(ccb_target_r0(ccb));

    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("addrr r0, r1");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        ccb_target_gen_emit("add a0, a0, a1");
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("add rax, rcx");
    }
    else {
        ccb_target_gen_emit("add %%rcx, %%rax");
    }
}

static void ccb_target_gen_assignment_structure(ccb_t* ccb, ccb_ast_t* structure, ccb_data_type_t* field, int offset) {
    switch (structure->type) {
    case CCB_AST_TYPE_VAR_LOCAL:
        ccb_target_gen_ensure_lva(ccb, structure);
        ccb_target_gen_save_local(ccb, field, structure->variable.off + field->offset + offset);
        break;

    case CCB_AST_TYPE_VAR_GLOBAL:
        ccb_target_gen_save_global(ccb, structure->variable.name, field, field->offset + offset);
        break;

    case CCB_AST_TYPE_STRUCT:
        ccb_target_gen_assignment_structure(ccb, structure->structure, field, offset + structure->ctype->offset);
        break;

    case CCB_AST_TYPE_DEREFERENCE:
        ccb_target_gen_push(ccb_target_r0(ccb));
        ccb_target_gen_expression(ccb, structure->unary.operand);
        ccb_target_gen_assignment_dereference_intermediate(ccb, field, field->offset + offset);
        break;

    default:
        ccb_compile_error(ccb, "Internal error: gen_assignment_structure");
        break;
    }
}

static void ccb_target_gen_load_structure(ccb_t* ccb, ccb_ast_t* structure, ccb_data_type_t* field, int offset) {
    switch (structure->type) {
    case CCB_AST_TYPE_VAR_LOCAL:
        ccb_target_gen_ensure_lva(ccb, structure);
        ccb_target_gen_load_local(ccb, field, ccb_target_bp(ccb), structure->variable.off + field->offset + offset);
        break;
    case CCB_AST_TYPE_VAR_GLOBAL:
        ccb_target_gen_load_global(ccb, field, structure->variable.name, field->offset + offset);
        break;
    case CCB_AST_TYPE_STRUCT:
        ccb_target_gen_load_structure(ccb, structure->structure, field, structure->ctype->offset + offset);
        break;
    case CCB_AST_TYPE_DEREFERENCE:
        ccb_target_gen_expression(ccb, structure->unary.operand);
        ccb_target_gen_load_local(ccb, field, ccb_target_r0(ccb), field->offset + offset);
        break;
    default:
        ccb_compile_error(ccb, "Internal error: gen_assignment_structure");
        break;
    }
}

static void ccb_target_gen_assignment(ccb_t* ccb, ccb_ast_t* var) {
    switch (var->type) {
    case CCB_AST_TYPE_DEREFERENCE:
        ccb_target_gen_assignment_dereference(ccb, var);
        break;
    case CCB_AST_TYPE_STRUCT:
        ccb_target_gen_assignment_structure(ccb, var->structure, var->ctype, 0);
        break;
    case CCB_AST_TYPE_VAR_LOCAL:
        ccb_target_gen_ensure_lva(ccb, var);
        ccb_target_gen_save_local(ccb, var->ctype, var->variable.off);
        break;
    case CCB_AST_TYPE_VAR_GLOBAL:
        ccb_target_gen_save_global(ccb, var->variable.name, var->ctype, 0);
        break;
    default:
        ccb_compile_error(ccb, "Internal error: gen_assignment");
    }
}

static void ccb_target_gen_comparision(ccb_t* ccb, char* operation, ccb_ast_t* ast) {
    if (ccb_ast_type_floating(ccb, ast->left->ctype) || ccb_ast_type_floating(ccb, ast->right->ctype)) {
        ccb_target_gen_expression(ccb, ast->left);
        ccb_target_gen_cast_float(ccb, ast->left->ctype);
        ccb_target_gen_push_xmm(0);
        ccb_target_gen_expression(ccb, ast->right);
        ccb_target_gen_cast_float(ccb, ast->right->ctype);
        ccb_target_gen_pop_xmm(1);
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("compff f1, f0 ; TODO: Right way around?");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            if (ccb_strcasecmp(operation, "setl") == 0) {
                ccb_target_gen_emit("flt.d a0, fa1, fa0");
            }
            else if (ccb_strcasecmp(operation, "setg") == 0) {
                ccb_target_gen_emit("flt.d a0, fa0, fa1");
            }
            else if (ccb_strcasecmp(operation, "setle") == 0) {
                ccb_target_gen_emit("fle.d a0, fa1, fa0");
            }
            else if (ccb_strcasecmp(operation, "setge") == 0) {
                ccb_target_gen_emit("fle.d a0, fa0, fa1");
            }
            else if (ccb_strcasecmp(operation, "sete") == 0) {
                ccb_target_gen_emit("feq.d a0, fa0, fa1");
            }
            else if (ccb_strcasecmp(operation, "setne") == 0) {
                ccb_target_gen_emit("feq.d a0, fa0, fa1");
                ccb_target_gen_emit("xori a0, a0, 1");
            }
            else {
                ccb_compile_error(ccb, "Bad internal comparison operator");
            }
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("ucomisd xmm1, xmm0");
        }
        else {
            ccb_target_gen_emit("ucomisd %%xmm0, %%xmm1");
        }
    }
    else {
        ccb_target_gen_expression(ccb, ast->left);
        ccb_target_gen_cast_int(ccb, ast->left->ctype);
        ccb_target_gen_push(ccb_target_r0(ccb));
        ccb_target_gen_expression(ccb, ast->right);
        ccb_target_gen_cast_int(ccb, ast->right->ctype);
        ccb_target_gen_pop(ccb_target_r1(ccb));
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("comprr r1, r0 ; TODO: Right way around?");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            if (ccb_strcasecmp(operation, "setl") == 0) {
                ccb_target_gen_emit("slt a0, a1, a0");
            } else if (ccb_strcasecmp(operation, "setg") == 0) {
                ccb_target_gen_emit("slt a0, a0, a1");
            } else if (ccb_strcasecmp(operation, "setle") == 0) {
                ccb_target_gen_emit("slt a0, a0, a1");
                ccb_target_gen_emit("xori a0, a0, 1"); // XXX TODO PRObLEMATIC...
            } else if (ccb_strcasecmp(operation, "setge") == 0) {
                ccb_target_gen_emit("slt a0, a1, a0");
                ccb_target_gen_emit("xori a0, a0, 1");
            } else if (ccb_strcasecmp(operation, "sete") == 0) {
                ccb_target_gen_emit("xor a0, a0, a1");
                ccb_target_gen_emit("seqz a0, a0");
            } else if (ccb_strcasecmp(operation, "setne") == 0) {
                ccb_target_gen_emit("xor a0, a0, a1");
                ccb_target_gen_emit("snez a0, a0");
            } else {
                ccb_compile_error(ccb, "Bad internal comparison operator");
            }
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("cmp rcx, rax ; TODO: Right way around?");
        }
        else {
            ccb_target_gen_emit("cmp %%rax, %%rcx");
        }
    }

    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("%s r0", operation);
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        // No cleanup needed
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("%s al", operation);
        ccb_target_gen_emit("movzx eax, al ; TODO: Right instruction?");
    }
    else {
        ccb_target_gen_emit("%s %%al", operation);
        ccb_target_gen_emit("movzb %%al, %%eax");
    }
}

static void ccb_target_gen_binary_arithmetic_integer(ccb_t* ccb, ccb_ast_t* ast) {
    char* op;
    switch (ast->type) {
    case '+':             op = (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "addrr" : "add");  break;
    case '-':             op = (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "subrr" : "sub");  break;
    case '*':             op = (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "mulrr" : (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV ? "mul" : "imul")); break;
    case '^':             op = (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "xorrr" : "xor");  break;
    case CCB_AST_TYPE_LSHIFT: op = (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "shlrr" : (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV ? /*"sla"*/ "sll" : "sal"));  break;
    case CCB_AST_TYPE_RSHIFT: op = (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "shrrr" : (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV ? "sra" :"sar")); break; // TODO: Unsigned variant?
    case '/':
    case '%':
        op = NULL; // Avoid uninitialised warning/error
        break;
    default:
        ccb_compile_error(ccb, "Internal error: gen_binary");
        return; // Seems intended. -Zak
        break;
    }

    ccb_target_gen_expression(ccb, ast->left);
    ccb_target_gen_cast_int(ccb, ast->left->ctype);
    ccb_target_gen_push(ccb_target_r0(ccb));
    ccb_target_gen_expression(ccb, ast->right);
    ccb_target_gen_cast_int(ccb, ast->right->ctype);
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("setrr r1, r0");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
        ccb_target_gen_emit("addimm $r1, $r0, 0");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        ccb_target_gen_emit("addi a1, a0, 0");
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("mov rcx, rax");
    }
    else {
        ccb_target_gen_emit("mov %%rax, %%rcx");
    }
    ccb_target_gen_pop(ccb_target_r0(ccb));

    if (ast->type == '/' || ast->type == '%') {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("signx64x32 r0");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("sext.w a0, a0");
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("cqo");
        }
        else {
            ccb_target_gen_emit("cqto");
        }
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("div r0, r2, r1");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("%s a0, a0, a1", ast->type == '%' ? "rem" : "div");
            //if (ast->type != '%'){
            ccb_target_gen_emit("sext.w a0, a0");
            //}
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("idiv rcx");
        }
        else {
            ccb_target_gen_emit("idiv %%rcx");
        }
        if (ast->type == '%') {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("setrr r0, r2");
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                // Handled above
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("mov eax, edx");
            }
            else {
                ccb_target_gen_emit("mov %%edx, %%eax");
            }
        }
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && (ast->type == CCB_AST_TYPE_LSHIFT || ast->type == CCB_AST_TYPE_RSHIFT)) {
        if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("%s rax, cl", op);
        }
        else {
            ccb_target_gen_emit("%s %%cl, %%rax", op);
        }
    }
    else {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("%s r0, r1", op);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
            ccb_target_gen_emit("%s $r0, $r0, $r1", op);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("%s a0, a0, a1", op);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("%s rax, rcx", op);
        }
        else {
            ccb_target_gen_emit("%s %%rcx, %%rax", op);
        }
    }
}

static void ccb_target_gen_binary_arithmetic_floating(ccb_t* ccb, ccb_ast_t* ast) {
    char* op;
    switch (ast->type) {
    case '+': op = (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "addff" : (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV ? "fadd.d" : "addsd")); break;
    case '-': op = (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "subff" : (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV ? "fsub.d" : "subsd")); break;
    case '*': op = (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "mulff" : (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV ? "fmul.d" : "mulsd")); break;
    case '/': op = (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "divff" : (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV ? "fdiv.d" : "divsd")); break;
    default:
        ccb_compile_error(ccb, "Internal error: gen_binary");
        return; // XXX Seems intended. -Zak
        break;
    }

    ccb_target_gen_expression(ccb, ast->left);
    ccb_target_gen_cast_float(ccb, ast->left->ctype);
    ccb_target_gen_push_xmm(0);
    ccb_target_gen_expression(ccb, ast->right);
    ccb_target_gen_cast_float(ccb, ast->right->ctype);
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("movff fa1, fa0");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        ccb_target_gen_emit("fmv.d f1, f0");
    }
    else {
        ccb_target_gen_emit("movsd %%xmm0, %%xmm1"); // TODO: Is this wrong order for FASM?
    }
    ccb_target_gen_pop_xmm(0);
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("%s f0, f1", op);
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        ccb_target_gen_emit("%s fa0, fa0, fa1", op);
    }
    else {
        ccb_target_gen_emit("%s %%xmm1, %%xmm0", op); // TODO: Is this wrong order for FASM? (Or is it corrected by other reversals?)
    }
}

static void ccb_target_gen_load(ccb_t* ccb, ccb_data_type_t* to, ccb_data_type_t* from) {
    if (ccb_ast_type_floating(ccb, to))
        ccb_target_gen_cast_float(ccb, from);
    else
        ccb_target_gen_cast_int(ccb, from);
}

static void ccb_target_gen_save(ccb_t* ccb, ccb_data_type_t* to, ccb_data_type_t* from) {
    if (ccb_ast_type_integer(ccb, from) && to->type == CCB_TYPE_FLOAT) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("floatrx32 f0, r0"); // Float-of-register-x32bit
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("fcvt.s.i fa0, a0");
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("cvtsi2ss xmm0, eax");
        }
        else {
            ccb_target_gen_emit("cvtsi2ss %%eax, %%xmm0");
        }
    }
    else if (ccb_ast_type_floating(ccb, from) && to->type == CCB_TYPE_FLOAT) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("floatsfd f0, f0"); // Float-single-of-float-double
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("fcvt.s.d fa0, fa0");
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("cvtpd2ps xmm0, xmm0");
        }
        else {
            ccb_target_gen_emit("cvtpd2ps %%xmm0, %%xmm0");
        }
    }
    else if (ccb_ast_type_integer(ccb, from) && (to->type == CCB_TYPE_DOUBLE || to->type == CCB_TYPE_LDOUBLE)) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("floatr f0, r0");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("fcvt.d.l fa0, a0"); // NOTE: .i on 32-bit?
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("cvtsi2sd xmm0, eax");
        }
        else {
            ccb_target_gen_emit("cvtsi2sd %%eax, %%xmm0");
        }
    }
    else if (!(ccb_ast_type_floating(ccb, from) && (to->type == CCB_TYPE_DOUBLE || to->type == CCB_TYPE_LDOUBLE))) {
        ccb_target_gen_load(ccb, to, from); // TODO: I'm hoping this is a reasonable code-path as long as to/from ordering is known?
    }
}

static void ccb_target_gen_binary(ccb_t* ccb, ccb_ast_t* ast) {
    if (ast->ctype->type == CCB_TYPE_POINTER) {
        ccb_target_gen_pointer_arithmetic(ccb, ast->type, ast->left, ast->right);
        return;
    }

    switch (ast->type) { // TODO: Unsigned variants
    case '<':             ccb_target_gen_comparision(ccb, (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "setrflaglt" : "setl"), ast); return;
    case '>':             ccb_target_gen_comparision(ccb, (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "setrflaggt" : "setg"), ast); return;
    case CCB_AST_TYPE_EQUAL:  ccb_target_gen_comparision(ccb, (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "setrflaget" : "sete"), ast); return;
    case CCB_AST_TYPE_GEQUAL: ccb_target_gen_comparision(ccb, (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "setrflagge" : "setge"), ast); return;
    case CCB_AST_TYPE_LEQUAL: ccb_target_gen_comparision(ccb, (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "setrflagle" : "setle"), ast); return;
    case CCB_AST_TYPE_NEQUAL: ccb_target_gen_comparision(ccb, (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC ? "setrflagne" : "setne"), ast); return;
    }

    if (ccb_ast_type_integer(ccb, ast->ctype))
        ccb_target_gen_binary_arithmetic_integer(ccb, ast);
    else if (ccb_ast_type_floating(ccb, ast->ctype))
        ccb_target_gen_binary_arithmetic_floating(ccb, ast);
    else
        ccb_compile_error(ccb, "Internal error in ccb_target_gen_binary");
}

static void ccb_target_gen_literal_save(ccb_t* ccb, ccb_ast_t* ast, ccb_data_type_t* type, int offset) {
    switch (type->type) {
    case CCB_TYPE_CHAR:
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setrpcmcx8 r5, %d, %d", offset, ast->integer);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            //ccb_target_gen_emit("addi t0, zero, %d", ast->integer);
            ccb_target_gen_emit("li t0, %d", ast->integer);
            //ccb_target_gen_emit("sb t0, %d(fp)", offset);
            ccb_target_gen_emit("sb t0, %d(x8)", offset);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("mov byte [rbp + %d], %d", offset, ast->integer);
        }
        else {
            ccb_target_gen_emit("movb $%d, %d(%%rbp)", ast->integer, offset);
        }
        break;

    case CCB_TYPE_SHORT:
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setrpcmcx16 r5, %d, %d", offset, ast->integer);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            //ccb_target_gen_emit("addi t0, zero, %d", ast->integer);
            ccb_target_gen_emit("li t0, %d", ast->integer);
            //ccb_target_gen_emit("sh t0, %d(fp)", offset);
            ccb_target_gen_emit("sh t0, %d(x8)", offset);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("mov word [rbp + %d], %d", offset, ast->integer);
        }
        else {
            ccb_target_gen_emit("movw $%d, %d(%%rbp)", ast->integer, offset);
        }
        break;

    case CCB_TYPE_INT:
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setrpcmcx32 r5, %d, %d", offset, ast->integer);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            //ccb_target_gen_emit("addi t0, zero, %d", ast->integer);
            ccb_target_gen_emit("li t0, %d", ast->integer);
            //ccb_target_gen_emit("sw t0, %d(fp)", offset);
            ccb_target_gen_emit("sw t0, %d(x8)", offset);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("mov dword [rbp + %d], %d", offset, ast->integer);
        }
        else {
            ccb_target_gen_emit("movl $%d, %d(%%rbp)", ast->integer, offset);
        }
        break;

    case CCB_TYPE_LONG:
    case CCB_TYPE_LLONG:
    case CCB_TYPE_POINTER:
        ccb_target_gen_push(ccb_target_r0(ccb));
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            unsigned long long tmp = ast->integer; /* TODO: Casts in function calls? */
            ccb_target_gen_emit("setrc r0, %llu", tmp /*(unsigned long long)ast->integer*/);
            ccb_target_gen_emit("setrpcmr r5, %d, r0", offset);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            unsigned long long tmp = ast->integer; /* TODO: Casts in function calls? */
            //ccb_target_gen_emit("addi t0, zero, %d", tmp); // TODO: Handle larger literals!
            ccb_target_gen_emit("li t0, %d", tmp); // TODO: Handle larger literals!
            //ccb_target_gen_emit("sd t0, %d(fp)", offset);
            ccb_target_gen_emit("sd t0, %d(x8)", offset);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            unsigned long long tmp = ast->integer; /* TODO: Casts in function calls? */
            ccb_target_gen_emit("mov qword rax, %llu", tmp);
            ccb_target_gen_emit("mov qword [rbp + %d], rax", offset);
        }
        else {
            unsigned long long tmp = ast->integer; /* TODO: Casts in function calls? */
            ccb_target_gen_emit("movq $%llu, %%rax", tmp);
            ccb_target_gen_emit("movq %%rax, %d(%%rbp)", offset);
        }
        ccb_target_gen_pop(ccb_target_r0(ccb));
        break;

    case CCB_TYPE_FLOAT:
    case CCB_TYPE_DOUBLE:
        ccb_target_gen_push(ccb_target_r0(ccb));
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            double tmp = ast->floating.value; // TODO: Allow getting address of struct member
            ccb_target_gen_emit("setrc rax, %llu", *((unsigned long long*) &tmp));
            ccb_target_gen_emit("setrpcmr r5, %d, r0", offset);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            double tmp = ast->floating.value; // TODO: Allow getting address of struct member
            ccb_target_gen_emit("li a0, %llu", *((unsigned long long*) & tmp));
            ccb_target_gen_emit("fmv.d.x fa0, a0");
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            double tmp = ast->floating.value; // TODO: Allow getting address of struct member
            ccb_target_gen_emit("movq rax, %llu", *((unsigned long long*) & tmp));
            ccb_target_gen_emit("movq [rbp + %d], rax", offset);
        }
        else {
            double tmp = ast->floating.value; // TODO: Allow getting address of struct member
            ccb_target_gen_emit("movq $%llu, %%rax", *((unsigned long long*) & tmp));
            ccb_target_gen_emit("movq %%rax, %d(%%rbp)", offset);
        }
        ccb_target_gen_pop(ccb_target_r0(ccb));
        break;

    default:
        ccb_compile_error(ccb, "codegen internal error in %s", __func__);
    }
}

static void ccb_target_gen_declaration_initialization(ccb_t* ccb, ccb_list_t* init, int offset) {
    for (ccb_list_iterator_t* it = ccb_list_iterator(init); !ccb_list_iterator_end(it); ) {
        ccb_ast_t* node = ccb_list_iterator_next(it);
        if (node->init.value->type == CCB_AST_TYPE_LITERAL)
            ccb_target_gen_literal_save(ccb, node->init.value, node->init.type, node->init.offset + offset);
        else {
            ccb_target_gen_expression(ccb, node->init.value);
            ccb_target_gen_save_local(ccb, node->init.type, node->init.offset + offset);
        }
    }
}

static void ccb_target_gen_emit_prefix(ccb_t* ccb, ccb_ast_t* ast, const char* op) {
    ccb_target_gen_expression(ccb, ast->unary.operand);
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("%src r0, 1", op);
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        if (ccb_strcasecmp(op, "sub") == 0) {
            ccb_target_gen_emit("addi a0, a0, -1");
        }
        else {
            ccb_target_gen_emit("addi a0, a0, 1");
        }
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("%s rax, 1", op);
    }
    else {
        ccb_target_gen_emit("%s $1, %%rax", op);
    }
    ccb_target_gen_assignment(ccb, ast->unary.operand);
}

static void ccb_target_gen_emit_postfix(ccb_t* ccb, ccb_ast_t* ast, const char* op) {
    ccb_target_gen_expression(ccb, ast->unary.operand);
    ccb_target_gen_push(ccb_target_r0(ccb));
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("%src r0, 1", op);
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        if (ccb_strcasecmp(op, "sub") == 0) {
            ccb_target_gen_emit("addi a0, a0, -1");
        }
        else {
            ccb_target_gen_emit("addi a0, a0, 1");
        }
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("%s rax, 1", op);
    }
    else {
        ccb_target_gen_emit("%s $1, %%rax", op);
    }
    ccb_target_gen_assignment(ccb, ast->unary.operand);
    ccb_target_gen_pop(ccb_target_r0(ccb));
}

static ccb_list_t* ccb_target_gen_function_argument_types(ccb_t* ccb, ccb_ast_t* ast) {
    ccb_list_t* list = ccb_list_create();
    ccb_list_iterator_t* jt = ccb_list_iterator(ast->function.call.paramtypes); // TODO: Compiler doesn't support multiple initialisers in for statements properly!
    for (ccb_list_iterator_t* it = ccb_list_iterator(ast->function.call.args); !ccb_list_iterator_end(it); ) {
        //fprintf(stderr, "Got list iterators @%lx, %lx\n", it, jt);
        ccb_ast_t* value = ccb_list_iterator_next(it);
        //fprintf(stderr, "Got value @%lx\n", value);
        ccb_data_type_t* type = ccb_list_iterator_next(jt);
        //fprintf(stderr, "Got type @%lx\n", type);

        ccb_list_push(list, type ? type : ccb_ast_result_type(ccb, '=', value->ctype, ccb_ast_data_table[CCB_AST_DATA_INT]));
    }
    return list;
}

static void ccb_target_gen_je(ccb_t* ccb, const char* label) {
    // TODO: This should be called ..gen_jz (jump-if-zero)
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("comprc r0, 0");
        ccb_target_gen_emit("jumpcifeq %s", label);
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        //ccb_target_gen_emit("addi t0, zero, 0");
        ccb_target_gen_emit("beq a0, zero, %s", label);
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("test rax, rax");
        ccb_target_gen_emit("je %s", label);
    }
    else {
        ccb_target_gen_emit("test %%rax, %%rax");
        ccb_target_gen_emit("je %s", label);
    }
}

static void ccb_target_gen_label_impl(ccb_t* ccb, int ln,  const char* label) {
    ccb_target_gen_emit("%s: # from ln%d", label, ln);
}

#define ccb_target_gen_label(ccb,lbl) ccb_target_gen_label_impl(ccb,__LINE__,lbl)

static void ccb_target_gen_jmp_impl(ccb_t* ccb, int line, const char* label) {
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("jumpc %s", label);
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        ccb_target_gen_emit("j %s", label);
    }
    else {
        ccb_target_gen_emit("jmp %s # from ln%d", label, line);
    }
}
#define ccb_target_gen_jmp(ccb,lbl) ccb_target_gen_jmp_impl(ccb,__LINE__,lbl)

int tmpintctr;// TODO: Fails to self-compile: = 999;

static void ccb_target_gen_expression(ccb_t* ccb, ccb_ast_t* ast) {
    //printf("Processing expression type 0x100+%d\n", ast == NULL ? -1 : ast->type-0x100);
    if (!ast) return;

    char* begin = NULL;
    char* ne = NULL;
    char* end = NULL;
    char* step = NULL;
    char* skip = NULL;

    int regi = 0, backi;
    int regx = 0, backx;

    int nstackints = 0, istackints;
    int nstackfloats = 0, istackfloats;
    int nregints = 0, iregints;
    int nregfloats = 0, iregfloats;

    ccb_list_t* argtypes;

    switch (ast->type) {
    case CCB_AST_TYPE_LITERAL:
        switch (ast->ctype->type) {
        case CCB_TYPE_CHAR:
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("setrc r0, %d", ast->integer);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
                ccb_target_gen_emit("xor $r0, $r0, $r0");
                ccb_target_gen_emit("addimm $r0, $r0, %d", ast->integer); // TODO: Sizing
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                ccb_target_gen_emit("addi a0, zero, %d", ast->integer); // TODO: Sizing
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("mov rax, %d", ast->integer);
            }
            else {
                ccb_target_gen_emit("mov $%d, %%rax", ast->integer);
            }
            break;

        case CCB_TYPE_INT:
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("setrc r0, %d", ast->integer);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
                ccb_target_gen_emit("xor $r0, $r0, $r0");
                ccb_target_gen_emit("addimm $r0, $r0, %d", ast->integer); // TODO: Sizing
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                //ccb_target_gen_emit("addi a0, zero, %d", ast->integer); // TODO: Sizing
                //ccb_target_gen_emit("lui a0, %%hi(%d)", ast->integer); // TODO: Sizing
                //ccb_target_gen_emit("addi a0, a0, %%lo(%d)", ast->integer); // TODO: Sizing
                ccb_target_gen_emit("li a0, %d", ast->integer); // TODO: Sizing
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("mov rax, %d", ast->integer);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW) {
                tmpintctr++;
                ccb_target_gen_emit("section data");
                ccb_target_gen_emit("tmpint%d: data64 %d", tmpintctr, ast->integer);
                ccb_target_gen_emit("section code");
                ccb_target_gen_emit("data8 0x48, 0x89, 0x04, 0x25 ; mov rax, qword ds:...");
                ccb_target_gen_emit("data32  tmpint%d", tmpintctr);
            }
            else {
                ccb_target_gen_emit("mov $%d, %%rax", ast->integer);
            }
            break;

        case CCB_TYPE_LONG:
        case CCB_TYPE_LLONG:
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                unsigned long long tmp = ast->integer; /* TODO: Casts in function calls? */
                ccb_target_gen_emit("setrc r0, %llu", tmp);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
                unsigned long long tmp = ast->integer; /* TODO: Casts in function calls? */
                ccb_target_gen_emit("xor $r0, $r0, $r0");
                ccb_target_gen_emit("addimm $r0, $r0, %llu", tmp); // TODO: Sizing
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                unsigned long long tmp = ast->integer; /* TODO: Casts in function calls? */
                //ccb_target_gen_emit("addi a0, zero, %lld", tmp); // TODO: Sizing
                ccb_target_gen_emit("li a0, %lld", tmp); // TODO: Sizing
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                long long tmp = ast->integer; /* TODO: Casts in function calls? */
                ccb_target_gen_emit("mov rax, %lld", tmp);
            }
            else {
                long long tmp = ast->integer; /* TODO: Casts in function calls? */
                ccb_target_gen_emit("mov $%lld, %%rax", tmp);
            }
            break;

        case CCB_TYPE_FLOAT:
        case CCB_TYPE_DOUBLE:
        case CCB_TYPE_LDOUBLE:
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("setfcm f0, %s", ast->floating.label);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
                ccb_target_gen_emit("todo");
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                ccb_target_gen_emit("la t0, %s", ast->floating.label);
                //ccb_target_gen_emit("lui t0, %%hi(%s)", ast->floating.label);
                //ccb_target_gen_emit("addi t0, t0, %%lo(%s)", ast->floating.label);
                ccb_target_gen_emit("fld fa0, 0(t0)");
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("movsd xmm0, [%s]", ast->floating.label);
            }
            else {
                ccb_target_gen_emit("movsd %s(%%rip), %%xmm0", ast->floating.label);
            }
            break;

        default:
            ccb_compile_error(ccb, "Internal error: gen_expression");
        }
        break;

    case CCB_AST_TYPE_STRING:
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setrc r0, %s", ast->string.label);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
            ccb_target_gen_emit("todo");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            //ccb_target_gen_emit("lui a0, %%hi(%s)", ast->string.label);
            //ccb_target_gen_emit("addi a0, a0, %%lo(%s)", ast->string.label);
            ccb_target_gen_emit("la a0, %s", ast->string.label);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("lea rax, [%s] ; TODO: Offset from RIP explicitly?", ast->string.label);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW) {
            ccb_target_gen_emit("data8 0x48, 0x8D, 0x04, 0x25 ; lea rax, [...]");
            ccb_target_gen_emit("data32 %s ; Pointer to string", ast->string.label);
        }
        else {
            ccb_target_gen_emit("lea %s(%%rip), %%rax", ast->string.label);
        }
        break;

    case CCB_AST_TYPE_VAR_LOCAL:
        ccb_target_gen_ensure_lva(ccb, ast);
        ccb_target_gen_load_local(ccb, ast->ctype, ccb_target_bp(ccb), ast->variable.off);
        break;
    case CCB_AST_TYPE_VAR_GLOBAL:
        ccb_target_gen_load_global(ccb, ast->ctype, ast->variable.label, 0);
        break;

    case CCB_AST_TYPE_PTRCALL:
        ccb_target_gen_expression(ccb, ast->function.call.callable);
        // TODO: Test this again...
        ccb_target_gen_push(ccb_target_r15(ccb));
        //ccb_target_gen_push("$rF");
        ccb_target_gen_push(ccb_target_r0(ccb));
        ccb_target_gen_pop(ccb_target_r15(ccb));
        //ccb_target_gen_pop("$rF");
    case CCB_AST_TYPE_CALL:
        argtypes = ccb_target_gen_function_argument_types(ccb, ast);
        for (ccb_list_iterator_t* it = ccb_list_iterator(argtypes); !ccb_list_iterator_end(it); ) {
            if (ccb_ast_type_floating(ccb, ccb_list_iterator_next(it))) {
                if (regx > 0) ccb_target_gen_push_xmm(regx);
                regx++;
                nregfloats++;
            }
            else {
                if (regi < ccb_target_callregisters(ccb)) {
                    const char* re = ccb_target_callregister(ccb, regi++);
                    if (ccb_strcasecmp(re, ccb_target_r0(ccb)) == 0) {
                        // Avoid pushing/popping the result register!
                    }
                    else {
                        ccb_target_gen_push(re);
                    }
                    nregints++;
                } else {
                    // This will go on the stack so it doesn't need a register!
                    nstackints++;
                }
            }
        }

        // TODO: Consider nstackints/nstackfloats in stack alignment!


        if (ccb_target_gen_stack % 16) {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("subrc r4, 8");
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
                ccb_target_gen_emit("addimm $rsp, $rsp, -8");
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                ccb_target_gen_emit("addi sp, sp, -8");
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("sub rsp, 8");
            }
            else {
                ccb_target_gen_emit("sub $8, %%rsp");
            }
        }


        istackints = nstackints;
        istackfloats = nstackfloats;

        for (ccb_list_iterator_t* it = ccb_list_iterator(ccb_list_reverse(ast->function.call.args)); !ccb_list_iterator_end(it); ) {
            ccb_ast_t* v = ccb_list_iterator_next(it);
            /*if (ccb_ast_type_floating(ccb, v)) {
                //ccb_target_gen_pop_xmm(--backx);
            }
            else {*/
                if (istackints > 0) {
                    istackints--;
                    ccb_target_gen_expression(ccb, v);
                    //ccb_data_type_t* ptype = ccb_list_iterator_next(jt);
                    //ccb_target_gen_save(ccb, ptype, v->ctype);
                    ccb_target_gen_push(ccb_target_r0(ccb));
                } else {
                    //ccb_target_gen_pop(ccb_target_callregister(ccb, --backi));
                }
            //}
        }

        istackints = nstackints;
        istackfloats = nstackfloats;
        iregints = nregints;
        iregfloats = nregfloats;

        ccb_list_iterator_t* jt = ccb_list_iterator(argtypes);
        for (ccb_list_iterator_t* it = ccb_list_iterator(ast->function.call.args);  !ccb_list_iterator_end(it);)
        {
            ccb_ast_t* v = ccb_list_iterator_next(it);
            ccb_data_type_t* ptype = ccb_list_iterator_next(jt);
            if (ccb_ast_type_floating(ccb, ptype)) {
                ccb_target_gen_expression(ccb, v);
                ccb_target_gen_save(ccb, ptype, v->ctype);
                ccb_target_gen_push_xmm(0);
            } else {
                /*if (istackints > 0) {
                    istackints--;
                } else*/ if (nregints > 0) {
                    ccb_target_gen_expression(ccb, v);
                    ccb_target_gen_save(ccb, ptype, v->ctype);
                    ccb_target_gen_push(ccb_target_r0(ccb));
                    nregints--;
                }
            }
        }

        backi = regi;
        backx = regx;

        istackints = nstackints;

        for (ccb_list_iterator_t* it = ccb_list_iterator(ccb_list_reverse(argtypes)); !ccb_list_iterator_end(it); ) {
            if (ccb_ast_type_floating(ccb, ccb_list_iterator_next(it))) {
                ccb_target_gen_pop_xmm(--backx);
            }
            else {
                if (istackints > 0) {
                    istackints--;
                } else {
                    ccb_target_gen_pop(ccb_target_callregister(ccb, --backi));
                }
            }
        }

        // TODO : I forget wtf was going on here...
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setrc r0, %d", regx);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
            ccb_target_gen_emit("xor $r0, $r0, $r0");
            ccb_target_gen_emit("addimm $r0, $r0, %d", regx);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            //ccb_target_gen_emit("addi a0, zero, %d", regx);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("mov eax, %d", regx); // TODO: Should this  be rax??
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW) {
            ccb_target_gen_emit("data8 0x48, 0xB8 ; mov rax, ...");
            ccb_target_gen_emit("data64 %d", regx);
        }
        else {
            ccb_target_gen_emit("mov $%d, %%eax", regx);
        }

        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_callconv(ccb) == CCB_TARGET_CALLCONV_WINDOWS) {
            ccb_target_gen_emit("push 0");
            ccb_target_gen_emit("push 0");
            ccb_target_gen_emit("push 0");
            ccb_target_gen_emit("push 0");
        }

        if (ast->type == CCB_AST_TYPE_PTRCALL) {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("callr r15");
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
                ccb_target_gen_emit("be $rlink, $r0, $rF");
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                ccb_target_gen_emit("jalr t0");
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("call r15");
            }
            else {
                ccb_target_gen_emit("call *%%r15 # GNU syntax is a fucking disaster");
            }

        }
        else {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("callc %s", ast->function.name);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
                ccb_target_gen_emit("xor $rF, $rF, $rF");
                ccb_target_gen_emit("addimm $rF, $rF, %s", ast->function.name);
                ccb_target_gen_emit("be $rlink, $r0, $rF");
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                ccb_target_gen_emit("call %s", ast->function.name);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW) {
                tmpintctr++;
                ccb_target_gen_emit("data8 0xE8 ; call ...");
                ccb_target_gen_emit("data32 (%s - callinstrend%i) ; instruction-relative address", ast->function.name, tmpintctr);
                ccb_target_gen_emit("callinstrend%i:", tmpintctr);
            }
            else {
                ccb_target_gen_emit("call %s", ast->function.name);
            }
        }

        istackints = nstackints;
        istackfloats = nstackfloats;

        for (ccb_list_iterator_t* it = ccb_list_iterator(ccb_list_reverse(argtypes)); !ccb_list_iterator_end(it); ) {
            if (ccb_ast_type_floating(ccb, ccb_list_iterator_next(it))) {
                /*if (regx != 1)
                    ccb_target_gen_pop_xmm(--regx);*/
            }
            else {
                if (istackints > 0) {
                    istackints--;
                    ccb_target_gen_pop(ccb_target_r15(ccb)); // TODO: Add a "drop" equivalent
                } else {
                    /*const char* re = ccb_target_callregister(ccb, --regi);
                    if (ccb_strcasecmp(re, ccb_target_r0(ccb)) == 0) {
                        // Avoid pushing/popping the result register!
                    }
                    else {
                        ccb_target_gen_pop(re);
                    }*/
                }
            }
        }


        if (ccb_target_gen_stack % 16) {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("addrc r4, 8");
            }
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                ccb_target_gen_emit("addi sp, sp, 8");
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("add rsp, 8");
            }
            else {
                ccb_target_gen_emit("add $8, %%rsp");
            }
        }



        istackints = nstackints;
        istackfloats = nstackfloats;

        for (ccb_list_iterator_t* it = ccb_list_iterator(ccb_list_reverse(argtypes)); !ccb_list_iterator_end(it); ) {
            if (ccb_ast_type_floating(ccb, ccb_list_iterator_next(it))) {
                if (regx != 1)
                    ccb_target_gen_pop_xmm(--regx);
            }
            else {
                if (istackints > 0) {
                    istackints--;
                    //ccb_target_gen_pop("r15"); // TODO: Add a "drop" equivalent
                } else {
                    const char* re = ccb_target_callregister(ccb, --regi);
                    if (ccb_strcasecmp(re, ccb_target_r0(ccb)) == 0) {
                        // Avoid pushing/popping the result register!
                    }
                    else {
                        ccb_target_gen_pop(re);
                    }
                }
            }
        }

        if (ast->ctype->type == CCB_TYPE_FLOAT) {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("addrr r0, r1"); // TODO
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                ccb_target_gen_emit("fcvt.d.s fa0, fa0");
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86) {
                ccb_target_gen_emit("cvtps2pd %%xmm0, %%xmm0"); // TODO: FASM?
            }
            else {
                ccb_target_gen_emit("todo");
            }
        }

        if (ast->type == CCB_AST_TYPE_PTRCALL) {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                ccb_target_gen_pop("t0");
            } else {
                ccb_target_gen_pop("r15");
            }
        }

        break;

    case CCB_AST_TYPE_DECLARATION:
        if (ast->decl.init)
            ccb_target_gen_declaration_initialization(ccb, ast->decl.init, ast->decl.var->variable.off);
        break;

    case CCB_AST_TYPE_ADDRESS:
        switch (ast->unary.operand->type) {
        case CCB_AST_TYPE_FUNCTION:
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("setrc r0, %s", ast->unary.operand->function.name);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                //ccb_target_gen_emit("lui a0, %%hi(%s)", ast->unary.operand->function.name);
                //ccb_target_gen_emit("addi a0, a0, %%lo(%s)", ast->unary.operand->function.name);
                ccb_target_gen_emit("la a0, %s", ast->unary.operand->function.name);
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("lea rax, [%s]", ast->unary.operand->function.name);
            }
            else {
                ccb_target_gen_emit("lea %s(%%rip), %%rax", ast->unary.operand->function.name);
            }
            break;
        case CCB_AST_TYPE_VAR_LOCAL:
            ccb_target_gen_ensure_lva(ccb, ast->unary.operand);
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("setrrpc r0, r5, %d", ast->unary.operand->variable.off);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                //ccb_target_gen_emit("add a0, fp, %d", ast->unary.operand->variable.off);
                ccb_target_gen_emit("add a0, x8, %d", ast->unary.operand->variable.off);
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("lea rax, [rbp + %d]", ast->unary.operand->variable.off);
            }
            else {
                ccb_target_gen_emit("lea %d(%%rbp), %%rax", ast->unary.operand->variable.off);
            }
            break;

        case CCB_AST_TYPE_VAR_GLOBAL:
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
                ccb_target_gen_emit("setrc r0, %s", ast->unary.operand->variable.label);
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("lea rax, [%s]", ast->unary.operand->variable.label);
            }
            else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
                //ccb_target_gen_emit("lui a0, %%hi(%s)", ast->unary.operand->variable.label);
                //ccb_target_gen_emit("addi a0, a0, %%lo(%s)", ast->unary.operand->variable.label);
                ccb_target_gen_emit("la a0, %s", ast->unary.operand->variable.label);
            }
            else {
                ccb_target_gen_emit("lea %s(%%rip), %%rax", ast->unary.operand->variable.label);
            }
            break;

        case CCB_AST_TYPE_DEREFERENCE:
            ccb_target_gen_expression(ccb, ast->unary.operand);
            break;

        default:
            ccb_compile_error(ccb, "Internal error in gen_expression, trying to get address of type 0x100+%d", ast->unary.operand->type-0x100);
            break;
        }
        break;

    case CCB_AST_TYPE_DEREFERENCE:
        ccb_target_gen_expression(ccb, ast->unary.operand);
        ccb_target_gen_load_local(ccb, ast->unary.operand->ctype->pointer, ccb_target_r0(ccb), 0);
        ccb_target_gen_load(ccb, ast->ctype, ast->unary.operand->ctype->pointer);
        break;

    case CCB_AST_TYPE_STATEMENT_IF:
    case CCB_AST_TYPE_EXPRESSION_TERNARY:
        ccb_target_gen_expression(ccb, ast->ifstmt.cond);
        ne = ccb_ast_label(ccb);
        ccb_target_gen_je(ccb, ne);
        ccb_target_gen_expression(ccb, ast->ifstmt.then);
        if (ast->ifstmt.last) {
            end = ccb_ast_label(ccb);
            ccb_target_gen_jmp(ccb, end);
            ccb_target_gen_label(ccb, ne);
            ccb_target_gen_emit("# TEST LINE C 1");
            ccb_target_gen_expression(ccb, ast->ifstmt.last);
            ccb_target_gen_emit("# TEST LINE C 2");
            ccb_target_gen_label(ccb, end);
        }
        else {
            ccb_target_gen_label(ccb, ne);
        }
        break;

    case CCB_AST_TYPE_STATEMENT_FOR: {
        if (ast->forstmt.init)
            ccb_target_gen_expression(ccb, ast->forstmt.init);
        begin = ccb_ast_label(ccb);
        step = ccb_ast_label(ccb);
        end = ccb_ast_label(ccb);
        ccb_target_gen_jump_save(ccb, end, step);
        ccb_target_gen_label(ccb, begin);
        if (ast->forstmt.cond) {
            ccb_target_gen_expression(ccb, ast->forstmt.cond);
            ccb_target_gen_je(ccb, end);
        }
        ccb_target_gen_expression(ccb, ast->forstmt.body);
        ccb_target_gen_label(ccb, step);
        if (ast->forstmt.step)
            ccb_target_gen_expression(ccb, ast->forstmt.step);
        ccb_target_gen_jmp(ccb, begin);
        ccb_target_gen_label(ccb, end);
        ccb_target_gen_jump_restore(ccb);
    } break;

    case CCB_AST_TYPE_STATEMENT_WHILE: {
        begin = ccb_ast_label(ccb);
        end = ccb_ast_label(ccb);
        ccb_target_gen_jump_save(ccb, end, begin);
        ccb_target_gen_label(ccb, begin);
        ccb_target_gen_expression(ccb, ast->forstmt.cond);
        ccb_target_gen_je(ccb, end);
        ccb_target_gen_expression(ccb, ast->forstmt.body);
        ccb_target_gen_jmp(ccb, begin);
        ccb_target_gen_label(ccb, end);
        ccb_target_gen_jump_restore(ccb);
    } break;

    case CCB_AST_TYPE_STATEMENT_DO: {
        begin = ccb_ast_label(ccb);
        end = ccb_ast_label(ccb);
        //fprintf(stderr, "generated begin '%s' end '%s'\n", begin, end);
        ccb_target_gen_jump_save(ccb, end, begin);
        ccb_target_gen_label(ccb, begin);
        ccb_target_gen_expression(ccb, ast->forstmt.body);
        ccb_target_gen_expression(ccb, ast->forstmt.cond);
        ccb_target_gen_emit("#TODO TEST LINE 1");
        ccb_target_gen_je(ccb, end);
        ccb_target_gen_jmp(ccb, begin);
        ccb_target_gen_label(ccb, end);
        ccb_target_gen_jump_restore(ccb);
        ccb_target_gen_emit("#TODO TEST LINE 2");
    } break;

    case CCB_AST_TYPE_STATEMENT_BREAK:
        if (!ccb_target_gen_label_break)
            ccb_compile_error(ccb, "ICE");
        ccb_target_gen_jmp(ccb, ccb_target_gen_label_break);
        break;

    case CCB_AST_TYPE_STATEMENT_CONTINUE:
        if (!ccb_target_gen_label_continue)
            ccb_compile_error(ccb, "ICE");
        ccb_target_gen_jmp(ccb, ccb_target_gen_label_continue);
        break;

    case CCB_AST_TYPE_STATEMENT_SWITCH: {
        char* ccb_target_gen_label_switch_store = ccb_target_gen_label_switch;
        char* ccb_target_gen_label_break_store = ccb_target_gen_label_break;
        ccb_target_gen_expression(ccb, ast->switchstmt.expr);
        ccb_target_gen_label_switch = ccb_ast_label(ccb);
        ccb_target_gen_label_break = ccb_ast_label(ccb);
        //fprintf(stderr, "generated switch '%s' break '%s'\n", ccb_target_gen_label_switch, ccb_target_gen_label_break);
        ccb_target_gen_jmp(ccb, ccb_target_gen_label_switch);
        ccb_target_gen_expression(ccb, ast->switchstmt.body);
        ccb_target_gen_label(ccb, ccb_target_gen_label_switch);
        ccb_target_gen_emit("#TODO TEST LINE B 1");
        ccb_target_gen_label(ccb, ccb_target_gen_label_break);
        ccb_target_gen_emit("# TODO TEST LINE B 2");
        ccb_target_gen_label_switch = ccb_target_gen_label_switch_store;
        ccb_target_gen_label_break = ccb_target_gen_label_break_store;
    } break;

    case CCB_AST_TYPE_STATEMENT_CASE:
        if (!ccb_target_gen_label_switch)
            ccb_compile_error(ccb, "ICE");
        skip = ccb_ast_label(ccb);
        ccb_target_gen_jmp(ccb, skip);
        ccb_target_gen_label(ccb, ccb_target_gen_label_switch);

        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("comprr r0, %d", ast->casevalue); // TODO: Is this the right ordering??
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            //ccb_target_gen_emit("addi a1, zero, %d", ast->casevalue);
            ccb_target_gen_emit("li a1, %d", ast->casevalue);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("cmp eax, %d", ast->casevalue);
        }
        else {
            ccb_target_gen_emit("cmp $%d, %%eax", ast->casevalue);
        }
        ccb_target_gen_label_switch = ccb_ast_label(ccb);
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("jumpcifne %s", ccb_target_gen_label_switch);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("bne a0, a1, %s", ccb_target_gen_label_switch);
        }
        else {
            ccb_target_gen_emit("jne %s", ccb_target_gen_label_switch);
        }
        ccb_target_gen_label(ccb, skip);
        break;

    case CCB_AST_TYPE_STATEMENT_DEFAULT:
        if (!ccb_target_gen_label_switch)
            ccb_compile_error(ccb, "ICE");
        ccb_target_gen_label(ccb, ccb_target_gen_label_switch);
        ccb_target_gen_label_switch = ccb_ast_label(ccb);
        break;

    case CCB_AST_TYPE_STATEMENT_GOTO:
        ccb_target_gen_jmp(ccb, ast->gotostmt.where);
        break;

    case CCB_AST_TYPE_STATEMENT_LABEL:
        if (ast->gotostmt.where)
            ccb_target_gen_label(ccb, ast->gotostmt.where);
        break;
    /*
    case CCB_AST_TYPE_STATEMENT_GOTO:
        fprintf(stderr, "Generating goto '%s'\n", ast->gotostmt.label);
        ccb_target_gen_jmp(ccb, ast->gotostmt.label);
        break;

    case CCB_AST_TYPE_STATEMENT_LABEL:
        fprintf(stderr, "Generating label '%s'\n", ast->gotostmt.label);
        ccb_target_gen_label(ccb, ast->gotostmt.label);
        break;
    */
    case CCB_AST_TYPE_STATEMENT_RETURN:
        if (ast->returnstmt) {
            ccb_target_gen_expression(ccb, ast->returnstmt);
            ccb_target_gen_save(ccb, ast->ctype, ast->returnstmt->ctype);
        }
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("setrr r4, r5");
            ccb_target_gen_emit("popr r5");
            ccb_target_gen_emit("return");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
            ccb_target_gen_pop("$rlink");
            ccb_target_gen_emit("bto $rlink, $rlink, $rlink");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("addi sp, x8, 0");
            ccb_target_gen_pop(ccb_target_bp(ccb));
            ccb_target_gen_pop("ra");
            ccb_target_gen_emit("ret");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW) {
            ccb_target_gen_emit("data8 0xC9 ; leave");
            ccb_target_gen_emit("data8 0xC3 ; ret");
        }
        else {
            ccb_target_gen_emit("leave");
            ccb_target_gen_emit("ret");
        }
        break;

    case CCB_AST_TYPE_STATEMENT_COMPOUND:
        for (ccb_list_iterator_t* it = ccb_list_iterator(ast->compound); !ccb_list_iterator_end(it); )
            ccb_target_gen_expression(ccb, ccb_list_iterator_next(it));
        break;

    case CCB_AST_TYPE_STRUCT:
        ccb_target_gen_load_structure(ccb, ast->structure, ast->ctype, 0);
        break;

    case '!':
        ccb_target_gen_expression(ccb, ast->unary.operand);
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("comprc r0, 0");
            ccb_target_gen_emit("setrflageq r0");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            //ccb_target_gen_emit("not a0, a0");
            //ccb_target_gen_emit("andi a0, a0, 1");
            ccb_target_gen_emit("seqz a0, a0");
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("cmp rax, 0");
            ccb_target_gen_emit("sete al");
            ccb_target_gen_emit("movzx eax, al");
        }
        else {
            ccb_target_gen_emit("cmp $0, %%rax");
            ccb_target_gen_emit("sete %%al");
            ccb_target_gen_emit("movzb %%al, %%eax");
        }
        break;

    case CCB_AST_TYPE_AND:
        end = ccb_ast_label(ccb);
        ccb_target_gen_expression(ccb, ast->left);
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("comprc r0, 0");
            ccb_target_gen_emit("setrc r0, 0");
            ccb_target_gen_emit("jumpcifeq %s", end);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("mv a1, a0");
            ccb_target_gen_emit("addi a2, zero, 0");
            ccb_target_gen_emit("addi a0, zero, 0");
            ccb_target_gen_emit("beq a1, a2, %s", end);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("test rax, rax");
            ccb_target_gen_emit("mov rax, 0");
            ccb_target_gen_emit("je %s", end);
        }
        else {
            ccb_target_gen_emit("test %%rax, %%rax");
            ccb_target_gen_emit("mov $0, %%rax");
            ccb_target_gen_emit("je %s", end);
        }

        ccb_target_gen_expression(ccb, ast->right);

        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("comprc r0, 0");
            ccb_target_gen_emit("setrc r0, 0");
            ccb_target_gen_emit("jumpcifeq %s", end);
            ccb_target_gen_emit("setrc r0, 1");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("mv a1, a0");
            ccb_target_gen_emit("addi a2, zero, 0");
            ccb_target_gen_emit("addi a0, zero, 0");
            ccb_target_gen_emit("beq a1, a2, %s", end);
            ccb_target_gen_emit("addi a0, zero, 1");
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("test rax, rax");
            ccb_target_gen_emit("mov rax, 0");
            ccb_target_gen_emit("je %s", end);
            ccb_target_gen_emit("mov rax, 1");
        }
        else {
            ccb_target_gen_emit("test %%rax, %%rax");
            ccb_target_gen_emit("mov $0, %%rax");
            ccb_target_gen_emit("je %s", end);
            ccb_target_gen_emit("mov $1, %%rax");
        }
        ccb_target_gen_label(ccb, end);
        break;

    case CCB_AST_TYPE_OR:
        end = ccb_ast_label(ccb);
        ccb_target_gen_expression(ccb, ast->left);
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("comprc r0, 0");
            ccb_target_gen_emit("setrc r0, 1");
            ccb_target_gen_emit("jumpcifne %s", end);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("mv a1, a0");
            ccb_target_gen_emit("addi a2, zero, 0");
            ccb_target_gen_emit("addi a0, zero, 1");
            ccb_target_gen_emit("bne a1, a2, %s", end);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("test rax, rax");
            ccb_target_gen_emit("mov rax, 1");
            ccb_target_gen_emit("jne %s", end);
        }
        else {
            ccb_target_gen_emit("test %%rax, %%rax");
            ccb_target_gen_emit("mov $1, %%rax");
            ccb_target_gen_emit("jne %s", end);
        }

        ccb_target_gen_expression(ccb, ast->right);

        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("comprc r0, 0");
            ccb_target_gen_emit("setrc r0, 1");
            ccb_target_gen_emit("jumpcifne %s", end);
            ccb_target_gen_emit("setrc r0, 0");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("mv a1, a0");
            ccb_target_gen_emit("addi a2, zero, 0");
            ccb_target_gen_emit("addi a0, zero, 1");
            ccb_target_gen_emit("bne a1, a2, %s", end);
            ccb_target_gen_emit("addi a0, zero, 0");
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("test rax, rax");
            ccb_target_gen_emit("mov rax, 1");
            ccb_target_gen_emit("jne %s", end);
            ccb_target_gen_emit("mov rax, 0"); // Fix, I think I mistranslated at first..
        }
        else {
            ccb_target_gen_emit("test %%rax, %%rax");
            ccb_target_gen_emit("mov $1, %%rax");
            ccb_target_gen_emit("jne %s", end);
            ccb_target_gen_emit("mov $0, %%rax");
        }
        ccb_target_gen_label(ccb, end);
        break;

    case '&':
    case '|':
        ccb_target_gen_expression(ccb, ast->left);
        ccb_target_gen_push(ccb_target_r0(ccb));
        ccb_target_gen_expression(ccb, ast->right);
        ccb_target_gen_pop(ccb_target_r1(ccb));
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("%s r0, r1", (ast->type == '|') ? "orrr" : "andrr");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("%s a0, a0, a1", (ast->type == '|') ? "or" : "and");
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("%s rax, rcx", (ast->type == '|') ? "or" : "and");
        }
        else {
            ccb_target_gen_emit("%s %%rcx, %%rax", (ast->type == '|') ? "or" : "and");
        }
        break;

    case '~':
        ccb_target_gen_expression(ccb, ast->left);
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("notr r0");
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("not a0, a0");
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("not rax");
        }
        else {
            ccb_target_gen_emit("not %%rax");
        }
        break;

    case CCB_AST_TYPE_POST_INCREMENT: ccb_target_gen_emit_postfix(ccb, ast, "add"); break;
    case CCB_AST_TYPE_POST_DECREMENT: ccb_target_gen_emit_postfix(ccb, ast, "sub"); break;
    case CCB_AST_TYPE_PRE_INCREMENT:  ccb_target_gen_emit_prefix(ccb, ast, "add"); break;
    case CCB_AST_TYPE_PRE_DECREMENT:  ccb_target_gen_emit_prefix(ccb, ast, "sub"); break;

    case CCB_AST_TYPE_EXPRESSION_CAST:
        ccb_target_gen_expression(ccb, ast->unary.operand);
        ccb_target_gen_load(ccb, ast->ctype, ast->unary.operand->ctype);
        break;

    case '=':
        ccb_target_gen_expression(ccb, ast->right);
        ccb_target_gen_load(ccb, ast->ctype, ast->right->ctype);
        ccb_target_gen_assignment(ccb, ast->left);
        break;

    default:
        ccb_target_gen_binary(ccb, ast);
    }
}

static void ccb_target_gen_data_initialization_intermediate(ccb_t* ccb, ccb_table_t* labels, char* data, ccb_table_t* literal, ccb_list_t* init, int offset);
static void ccb_target_gen_data_initialization_intermediate(ccb_t* ccb, ccb_table_t* labels, char* data, ccb_table_t* literal, ccb_list_t* init, int offset) {
    for (ccb_list_iterator_t* it = ccb_list_iterator(init); !ccb_list_iterator_end(it); ) {
        ccb_ast_t* node = ccb_list_iterator_next(it);

        //printf("Iterating over node at 0x%lx (init.value at 0x%lx)\n", node, node->init.value);
        //printf("Unary operand at 0x%lx \n", node->init.value->unary.operand);
        // TODO: Doesn't properly optimise away the "&&" here so we have to triple-if it:
        if ((node->init.value->type == CCB_AST_TYPE_ADDRESS))
            if( (node->init.value->unary.operand->type == CCB_AST_TYPE_VAR_LOCAL))
            if ( (node->init.value->unary.operand->variable.init)) {


            //printf("address\n");
            char* label = ccb_ast_label(ccb);
            ccb_string_t* string = ccb_string_create();

            //ccb_string_catf(string, "%d", node->init.offset + offset);
            ccb_string_catint(string, node->init.offset + offset);

            ccb_table_insert(literal, label, node->init.value->unary.operand);
            ccb_table_insert(labels, ccb_string_buffer(string), label);
            continue;
        }

        //printf("...\n");

        if (node->init.value->type == CCB_AST_TYPE_VAR_LOCAL && node->init.value->variable.init) {
            ccb_target_gen_data_initialization_intermediate(ccb, labels, data, literal, node->init.value->variable.init, node->init.offset + offset);
            continue;
        }

        //printf("...\n");
        //printf("Using %d\n", node->init.type->type);

        switch (node->init.type->type) {
        case CCB_TYPE_FLOAT:
            *(float*)(data + node->init.offset + offset) = node->init.value->floating.value;
            break;

        case CCB_TYPE_DOUBLE:
            *(double*)(data + node->init.offset + offset) = node->init.value->floating.value;
            break;

        case CCB_TYPE_CHAR:
            *(char*)(data + node->init.offset + offset) = ccb_parse_evaluate(ccb, node->init.value);
            break;

        case CCB_TYPE_SHORT:
            *(short*)(data + node->init.offset + offset) = ccb_parse_evaluate(ccb, node->init.value);
            break;

        case CCB_TYPE_INT:
            *(int*)(data + node->init.offset + offset) = ccb_parse_evaluate(ccb, node->init.value);
            break;

        case CCB_TYPE_LONG:
            *(long*)(data + node->init.offset + offset) = ccb_parse_evaluate(ccb, node->init.value);
            break;

        case CCB_TYPE_LLONG:
            *(long long*)(data + node->init.offset + offset) = ccb_parse_evaluate(ccb, node->init.value);
            break;

        case CCB_TYPE_POINTER:
            *(long*)(data + node->init.offset + offset) = ccb_parse_evaluate(ccb, node->init.value);

        default:
            break;
        }
    }
}

static void ccb_target_gen_data_initialization(ccb_t* ccb, ccb_table_t* table, ccb_list_t* list, int size) {
    char* data = ccb_memory_allocate(size);
    memset(data, 0, size);

    ccb_table_t* labels = ccb_table_create(NULL);
    ccb_target_gen_data_initialization_intermediate(ccb, labels, data, table, list, 0);

    int i = 0;
    for (; i <= size - 4; i += 4) {
        ccb_string_t* string = ccb_string_create();
        //ccb_string_catf(string, "%d", i);
        ccb_string_catint(string, i);
        char* label = ccb_table_find(labels, ccb_string_buffer(string));
        if (label) {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC
                || (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW)) {
                ccb_target_gen_emit("data64 %s", label);
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("dq %s", label);
            }
            else {
                ccb_target_gen_emit(".quad %s", label);
            }
            i += 4;
        }
        else {
            if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC
                || (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW)) {
                ccb_target_gen_emit("data32 %d", data[i]);
            }
            else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
                ccb_target_gen_emit("dd %d", data[i]);
            }
            else {
                ccb_target_gen_emit(".long %d", data[i]);
            }
        }
    }
    for (; i < size; i++) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC
            || (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW)) {
            ccb_target_gen_emit("data8 %d", data[i]);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("db %d", data[i]);
        }
        else {
            ccb_target_gen_emit(".byte %d", data[i]);
        }
    }

    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC
        || (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW)) {
        ccb_target_gen_emit("align 8");
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("align 8");
    }
    else {
        ccb_target_gen_emit(".align 8");
    }
}

/*
 * Recursive compliteral generation, emits data initialization
 * until there is nothing to initialize left.
 */
static void ccb_target_gen_data_literal(ccb_t* ccb, char* label, ccb_ast_t* ast) {
    ccb_table_t* table = ccb_table_create(NULL);
    ccb_target_gen_emit_inline("%s:", label);
    ccb_target_gen_data_initialization(ccb, table, ast->variable.init, ast->ctype->size);

    for (ccb_list_iterator_t* it = ccb_list_iterator(ccb_table_keys(table)); !ccb_list_iterator_end(it); ) {
        char* label = ccb_list_iterator_next(it);
        ccb_ast_t* node = ccb_table_find(table, label);

        ccb_target_gen_data_literal(ccb, label, node);
    }
}

static void ccb_target_gen_data(ccb_t* ccb, ccb_ast_t* ast) {
    ccb_table_t* table = ccb_table_create(NULL);

    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit_inline("section data");
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM || ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW) {
        if (ccb_target_binfmt(ccb) == CCB_TARGET_BINFMT_FLAT) {
            // ignored for flat binaries for now
        }
        else {
            ccb_target_gen_emit_inline("section data");
        }
    }
    else {
        ccb_target_gen_emit_inline(".data");
    }

    if (!ast->decl.var->ctype->isstatic) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit_inline("symbol %s, public", ast->decl.var->variable.name);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            if (ccb_target_binfmt(ccb) == CCB_TARGET_BINFMT_FLAT) {
                ccb_target_gen_emit_inline("; [public %s]", ast->decl.var->variable.name);
            }
            else {
                ccb_target_gen_emit_inline("public %s", ast->decl.var->variable.name);
            }
        }
        else {
            ccb_target_gen_emit_inline(".global %s", ast->decl.var->variable.name);
        }
    }

    ccb_target_gen_emit_inline("%s:", ast->decl.var->variable.name);
    ccb_target_gen_data_initialization(ccb, table, ast->decl.init, ast->decl.var->ctype->size);

    for (ccb_list_iterator_t* it = ccb_list_iterator(ccb_table_keys(table)); !ccb_list_iterator_end(it); ) {
        char* label = ccb_list_iterator_next(it);
        ccb_ast_t* node = ccb_table_find(table, label);

        ccb_target_gen_data_literal(ccb, label, node);
    }
}

static void ccb_target_gen_bss(ccb_t* ccb, ccb_ast_t* ast) {
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit_inline("section zerodata");
        ccb_target_gen_emit("align %d", ast->decl.var->ctype->size < 8 ? ast->decl.var->ctype->size : 8);
        ccb_target_gen_emit_inline("%s:", ast->decl.var->variable.name);
        ccb_target_gen_emit("reserve %d", ast->decl.var->ctype->size);
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        if (ccb_target_binfmt(ccb) == CCB_TARGET_BINFMT_FLAT) {
            ccb_target_gen_emit_inline("; [bss section]");
        }
        else {
            ccb_target_gen_emit_inline("section '.bss' writeable");
        }
        ccb_target_gen_emit("align %d", ast->decl.var->ctype->size < 8 ? ast->decl.var->ctype->size : 8);

        /* I thought some errors in the FASM output were due to reserving rather than setting bytes explicitly,
         * turns out that wasn't the case (I was missing the "section" directive above!). However, the
         * explicitly-setting code is preserved here in case it's useful in the future (e.g. for targetting
         * assemblers which can't reserve bytes like FASM).

        switch (ast->decl.var->ctype->size) {
            case 1:
                ccb_target_gen_emit("%s: db 0", ast->decl.var->variable.name);
                break;

            case 2:
                ccb_target_gen_emit("%s: dw 0", ast->decl.var->variable.name);
                break;

            case 4:
                ccb_target_gen_emit("%s: dd 0", ast->decl.var->variable.name);
                break;

            case 8:
                ccb_target_gen_emit("%s: dq 0", ast->decl.var->variable.name);
                break;

            default:
                ccb_target_gen_emit("%s:", ast->decl.var->variable.name);
                int i;
                for (i = 0; i < ast->decl.var->ctype->size; i++) {
                    ccb_target_gen_emit(" db 0");
                }
        }
        */
        // Simply reserve the required number of bytes
        ccb_target_gen_emit("%s: rb %d", ast->decl.var->variable.name, ast->decl.var->ctype->size);
    }
    else {
        ccb_target_gen_emit(".lcomm %s, %d", ast->decl.var->variable.name, ast->decl.var->ctype->size);
    }
}

static void ccb_target_gen_global(ccb_t* ccb, ccb_ast_t* var) {
    if (var->decl.init) {
        ccb_target_gen_data(ccb, var);
    }
    else {
        ccb_target_gen_bss(ccb, var);
    }
}

void ccb_target_gen_data_section(ccb_t* ccb) {
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC
        || (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW)) {
        ccb_target_gen_emit_inline("section data");
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        if (ccb_target_binfmt(ccb) == CCB_TARGET_BINFMT_FLAT) {
            ccb_target_gen_emit_inline("; [data section]");
        }
        else {
            ccb_target_gen_emit_inline("format elf64");
            ccb_target_gen_emit("extrn printf ; XXX TODO Total hack");
            ccb_target_gen_emit("extrn exit ; XXX TODO Total hack");
            ccb_target_gen_emit("extrn strcmp ; XXX TODO Total hack");
            ccb_target_gen_emit("extrn strlen ; XXX TODO Total hack");
            ccb_target_gen_emit("extrn strcat ; XXX TODO Total hack");
            ccb_target_gen_emit("extrn calloc ; XXX TODO Total hack");
            ccb_target_gen_emit_inline("section '.data' writeable");
        }
    }
    else {
        ccb_target_gen_emit_inline(".data");
    }

    for (ccb_list_iterator_t* it = ccb_list_iterator(ccb_ast_strings); !ccb_list_iterator_end(it); ) {
        ccb_ast_t* ast = ccb_list_iterator_next(it);
        ccb_target_gen_emit_inline("%s: ", ast->string.label);
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC
            || (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW)) {
            ccb_target_gen_emit("data8 \"%s\", 0", ccb_string_quote_fasm(ast->string.data, '\"'));
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("db '%s', 0", ccb_string_quote_fasm(ast->string.data, '\''));
        }
        else {
            ccb_target_gen_emit(".string \"%s\"", ccb_string_quote(ast->string.data));
        }
    }

    for (ccb_list_iterator_t* it = ccb_list_iterator(ccb_ast_floats); !ccb_list_iterator_end(it); ) {
        ccb_ast_t* ast = ccb_list_iterator_next(it);
        char* label = ccb_ast_label(ccb);

        ast->floating.label = label;
        ccb_target_gen_emit_inline("%s:", label);
        double tmp = ast->floating.value;
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC
            || ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW) {
            ccb_target_gen_emit("data32 %d", ((int*)&tmp)[0]);
            ccb_target_gen_emit("data32 %d", ((int*)&tmp)[1]);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("dd %d", ((int*)&tmp)[0]);
            ccb_target_gen_emit("dd %d", ((int*)&tmp)[1]);
        }
        else {
            ccb_target_gen_emit(".long %d", ((int*)&tmp)[0]);
            ccb_target_gen_emit(".long %d", ((int*)&tmp)[1]);
        }
    }
}

static int ccb_target_gen_alignment(ccb_t* ccb, int n, int align) {
    int remainder = n % align;
    return (remainder == 0)
        ? n
        : n - remainder + align;
}

static void ccb_target_gen_function_prologue(ccb_t* ccb, ccb_ast_t* ast) {
    if (ccb_list_length(ast->function.params) > ccb_target_callregisters(ccb)) {
        // TODO: Track callconv if (ast->function.callconv != 0) {
        //    fprintf(stderr, "NOTE: This function has calling convention %d\n", ast->function.callconv);
        //}
        
        //ccb_compile_error(ccb, "Too many params for function");
        ccb_compile_warn(ccb, "This function will use stack arguments, which are only partly tested");
    }

    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC
        || (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW)) {
        ccb_target_gen_emit_inline("section code");
        ccb_target_gen_emit_inline("symbol %s, public", ast->function.name);
    }
    else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        if (ccb_target_binfmt(ccb) == CCB_TARGET_BINFMT_FLAT) {
            ccb_target_gen_emit_inline("; [text section]");
            ccb_target_gen_emit_inline("; [public %s]", ast->function.name);
        }
        else {
            ccb_target_gen_emit_inline("section '.text' executable");
            ccb_target_gen_emit_inline("public %s", ast->function.name);
        }
    }
    else {
        ccb_target_gen_emit_inline(".text");
        ccb_target_gen_emit_inline(".global %s", ast->function.name);
    }

    ccb_target_gen_emit_inline("%s:", ast->function.name);
    ccb_target_gen_stack -= (ccb_target_wordsize(ccb)/8); // Remember to account for return instruction pointer as well as rbp

    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
        ccb_target_gen_push("$rlink"); // On this architecture we need to store the return address explicitly
    } else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        ccb_target_gen_push("ra"); // On this architecture we need to store the return address explicitly
    }

    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW) {
        ccb_target_gen_emit("data8 0x55 ; push rbp");
    }
    else {
        ccb_target_gen_push(ccb_target_bp(ccb));
    }

    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        //ccb_target_gen_emit("addi fp, sp, 0");
        ccb_target_gen_emit("addi x8, sp, 0");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
        ccb_target_gen_emit("addimm $rbase, $rstack, 0");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("setrr r5, r4");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("mov rbp, rsp");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW) {
        ccb_target_gen_emit("data8 0x48, 0x89, 0xEC ; mov rbp, rsp");
    }
    else {
        ccb_target_gen_emit("mov %%rsp, %%rbp");
    }

    int offset = 0;
    int spareoffset = ccb_target_type_size_pointer(ccb)*2;
    int regi = 0;
    int regx = 0;

    for (ccb_list_iterator_t* it = ccb_list_iterator(ast->function.params); !ccb_list_iterator_end(it); ) {
        ccb_ast_t* value = ccb_list_iterator_next(it);
        bool spare = false;

        if (value->ctype->type == CCB_TYPE_FLOAT) {
            ccb_target_gen_push_xmm(regx++);
        }
        else if (value->ctype->type == CCB_TYPE_DOUBLE || value->ctype->type == CCB_TYPE_LDOUBLE) {
            ccb_target_gen_push_xmm(regx++);
        }
        else { // TODO: Support for 64-bit values on 32-bit architectures
            if (regi < ccb_target_callregisters(ccb)) {
                ccb_target_gen_push(ccb_target_callregister(ccb, regi++));
            } else {
                spare = true;
            }
        }
        if (spare){
            value->variable.off = spareoffset;
            spareoffset += ccb_target_gen_alignment(ccb, value->ctype->size, 8);
        } else {
            offset -= ccb_target_gen_alignment(ccb, value->ctype->size, 8);
            value->variable.off = offset;
        }
    }

    int localdata = 0;
    for (ccb_list_iterator_t* it = ccb_list_iterator(ast->function.locals); !ccb_list_iterator_end(it); ) {
        ccb_ast_t* value = ccb_list_iterator_next(it);
        int inneroffset = ccb_target_gen_alignment(ccb, value->ctype->size, 8);
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC || ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("; var offset %d size %d <- offset %d ldata %d", inneroffset, value->ctype->size, offset, localdata);
        }
        else {
            ccb_target_gen_emit("# var offset %d size %d <- offset %d ldata %d", inneroffset, value->ctype->size, offset, localdata);
        }
        offset -= inneroffset;
        value->variable.off = offset;
        localdata -= inneroffset;
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC || ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("; var offset %d size %d -> offset %d ldata %d", inneroffset, value->ctype->size, offset, localdata);
        }
        else {
            ccb_target_gen_emit("# var offset %d size %d -> offset %d ldata %d", inneroffset, value->ctype->size, offset, localdata);
        }
    }

    if (localdata) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
            ccb_target_gen_emit("addiu sp, %d", localdata);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
            ccb_target_gen_emit("subrc r4, %d", -localdata);
        }
        else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
            ccb_target_gen_emit("addi sp, sp, %d", localdata);
        }
        else if (ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            ccb_target_gen_emit("sub rsp, %d", -localdata);
        }
        else {
            ccb_target_gen_emit("sub $%d, %%rsp", -localdata);
        }
        //ccb_target_gen_stack += -localdata; // XXX TODO: I don't know *why* this fixes most of the test cases, it just does. -Zak.
    }
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC || ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("; stack before: %d", ccb_target_gen_stack);
    }
    else {
        ccb_target_gen_emit("# stack before: %d", ccb_target_gen_stack);
    }
    ccb_target_gen_stack += -localdata;//-(offset - 8);
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1 || ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC || ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
        ccb_target_gen_emit("; stack after: %d", ccb_target_gen_stack);
    }
    else {
        ccb_target_gen_emit("# stack after: %d", ccb_target_gen_stack);
    }
}

static void ccb_target_gen_function_epilogue(ccb_t* ccb) {
    if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GEN1) {
        ccb_target_gen_pop("$rbase");
        ccb_target_gen_pop("$rlink");
        ccb_target_gen_emit("bto $rlink, $rlink, $rlink");
        /*ccb_target_gen_emit("addimm $sp, $fp, 8");
        ccb_target_gen_emit("move sp, fp");
        ccb_target_gen_emit("loadw fp, sp, 4");
        ccb_target_gen_emit("addiu sp, sp, 8");
        ccb_target_gen_emit("jr ra");
        ccb_target_gen_emit("nop");*/
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC) {
        ccb_target_gen_emit("setrr r4, r5");
        ccb_target_gen_emit("popr r5");
        ccb_target_gen_emit("return");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_RISCV) {
        ccb_target_gen_emit("addi sp, x8, 0");
        ccb_target_gen_pop(ccb_target_bp(ccb));
        ccb_target_gen_pop("ra");
        ccb_target_gen_emit("ret");
    }
    else if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_X86 && ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_RAW) {
        ccb_target_gen_emit("data8 0xC9 ; leave");
        ccb_target_gen_emit("data8 0xC3 ; ret");
    }
    else {
        ccb_target_gen_emit("leave");
        ccb_target_gen_emit("ret");
    }
}

void ccb_target_gen_function(ccb_t* ccb, ccb_ast_t* ast) {
    ccb_target_gen_stack = 0;
    if (ast->type == CCB_AST_TYPE_FUNCTION) {
        ccb_target_gen_function_prologue(ccb, ast);
        ccb_target_gen_expression(ccb, ast->function.body);
        ccb_target_gen_function_epilogue(ccb);
    }
    else if (ast->type == CCB_AST_TYPE_DECLARATION) {
        ccb_target_gen_global(ccb, ast);
    }
    else {
        ccb_compile_error(ccb, "ICE");
    }
    if (ccb_target_gen_stack > 8) {
        if (ccb_target_family(ccb) == CCB_ARCH_FAMILY_GENERIC || ccb_target_asmfmt(ccb) == CCB_TARGET_ASMFMT_FASM) {
            fprintf(ccb->output, ";; stack is misaligned by %d bytes [this warning may be outdated]\n", ccb_target_gen_stack);
        }
        else {
            fprintf(ccb->output, "## stack is misaligned by %d bytes [this warning may be outdated]\n", ccb_target_gen_stack);
        }
    }
}


/* From ifdef of CCBGENERIC_IMPLEMENTATION: */
#endif

/* From ifndef at top of file: */
#endif
