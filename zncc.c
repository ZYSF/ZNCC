#define _CRT_SECURE_NO_WARNINGS

#ifdef _WIN32
#include <process.h>
#endif
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

//#include "compile.h"
#define CCB_IMPLEMENTATION
#include "ccb.h"
#define CCBGENERIC_IMPLEMENTATION
#include "ccbgeneric.h"
#ifdef TOOL_CPP
#define CPP_IMPLEMENTATION
#include "cpp.h"
#endif
#ifdef TOOL_MK
#define MK_IMPLEMENTATION
#include "mk.h"
#endif

int sh_main(int argc, char** argv);

void ccb_compile_error_impl(ccb_t* ccb/*, const char* fmt, ...*/) {
    fprintf(stderr, "^ Around line %d, column %d of input.\n", ccb->line, ccb->col);
    /*#ifdef _ZCC
    fprintf(stderr, "ERROR '%s'\n(TODO: Better formatting!)\n", fmt);
    ((char*)NULL)[0] = 0; // Trigger debugger 
    #else
    va_list  a;
    va_start(a, fmt);
    vfprintf(stderr, fmt, a);
    fprintf(stderr, "\n");
    va_end(a);
    #endif*/

    exit(EXIT_FAILURE);
}

void ccb_compile_warn_impl(ccb_t* ccb/*, const char* fmt, ...*/) {
    fprintf(stderr, "^ Around line %d, column %d of input.\n", ccb->line, ccb->col);
    /*#ifdef _ZCC
    fprintf(stderr, "WARNING '%s'\n(TODO: Better formatting!)\n", fmt);
    #else
    va_list  a;
    va_start(a, fmt);
    vfprintf(stderr, fmt, a);
    fprintf(stderr, "\n");
    va_end(a);
    #endif*/
}

static int startcompile(ccb_t* ccb) {
    ccb_util_init(ccb);
    ccb_target_init(ccb);
    ccb_ast_init(ccb);

    ccb_list_t* block = ccb_parse_run(ccb);
    if (!ccb->dump_ast) {
        if (ccb->include_data) {
            ccb_target_gen_data_section(ccb);
        }
    }
    for (ccb_list_iterator_t* it = ccb_list_iterator(block); !ccb_list_iterator_end(it); ) {
        if (!ccb->dump_ast) {
            if (ccb->include_code) {
                ccb_target_gen_function(ccb, ccb_list_iterator_next(it));
            }
        }
        else {
            printf("%s", ccb_ast_string(ccb, ccb_list_iterator_next(it)));
        }
    }
    return true;
}

/* Original main program from LICE:
int main(int argc, char **argv) {
    argc--;
    argv++;
    return startcompile(!!(argc && !strcmp(*argv, "--dump-ast")))
            ? EXIT_SUCCESS
            : EXIT_FAILURE;
}
*/

static void usage(int argc, char** argv, int arge) {
    //fprintf(stderr, "TODO: Usage!\n");
    char* n = argv[0];

    fprintf(stderr, "USAGE:\n\n");
    fprintf(stderr, "    %s [--silent] [--ast-only|--code-only|--data-only|--usage] [TODO --asmfmt fasm|gas] [TODO --binfmt elf|flat] [--input <fname>] [--output|--append <fname>]\n\n", n);
    fprintf(stderr, "(This is a simple core compiler designed to be invoked from a more user-friendly frontend.\nArguments must be provided in the above order, defaults to using stdin/stdout.)");
}

int preprocessormain(int argc, char** argv); // Extra definition of the C preprocessor main program
//void mk_main(int argc, char** argv); // Extra definition of the "make" tool main program
/* Zak's new main program: */
int main(int argc, char** argv) {
    //argc = 3;
    //argv = (char*[]){"test",/*"--ast-only",*/"--input","C:\\Users\\Zak\\source\\repos\\ZCC\\Debug\\test2.c"};
    ccb_t ccb;
    ccb.dump_ast = false;
    ccb.silent = false;
    ccb.input = stdin;
    ccb.output = stdout;
    ccb.include_code = true;
    ccb.include_data = true;
    ccb.line = 1;
    ccb.col = 1;

    int argi = 1;
    if (argc > argi && (!strcmp(argv[argi], "--silent") || !strcmp(argv[argi], "--usage"))) { // Also skip added notices if using --usage
        if (!strcmp(argv[argi], "--silent")) {
            argi++;
        }
        ccb.silent = true;
    }
    else {
        fprintf(stderr, "CCb C Compiler backend (early version)\n");
        fprintf(stderr, "NOTE: This program generally reads program code from standard input (until EOF) and writes assembly code to standard output.\n");
        fprintf(stderr, "      It would usually be used from a compiler frontend (preprocessing, assembling and linking must be done separately).\n");
        fprintf(stderr, "      Use --silent as the first argument to disable these notices or --usage to learn more.\n\n");
    }

    if (argc > argi && !strcmp(argv[argi], "--ast-only")) {
        if (!ccb.silent) {
            fprintf(stderr, "NOTE: Will dump AST to output instead of assembly code.\n");
        }
        ccb.dump_ast = true;
        argi++;
    }
    else if (argc > argi && !strcmp(argv[argi], "--data-only")) {
        if (!ccb.silent) {
            fprintf(stderr, "NOTE: Will dump data section only.\n");
        }
        ccb.include_code = false;
        ccb.include_data = true;
        argi++;
    }
    else if (argc > argi && !strcmp(argv[argi], "--code-only")) {
        if (!ccb.silent) {
            fprintf(stderr, "NOTE: Will dump code section only.\n");
        }
        ccb.include_code = true;
        ccb.include_data = false;
        argi++;
    }
#ifdef TOOL_CPP
    else if (argc > argi && !strcmp(argv[argi], "-P")) { /* Invoke the built-in preprocessor instead of the backend. */
        if (!ccb.silent) {
            fprintf(stderr, "NOTE: Invoking the experimental preprocessor.\n");
        }
        // NOTE: This will only work for argv == 1, and because cppmain ignores the -P flag!
        return preprocessormain(argc, argv);
    }
#endif
#ifdef TOOL_MK
    else if (argc > argi && !strcmp(argv[argi], "-M")) { /* Invoke the built-in make tool instead of the backend. */
        if (!ccb.silent) {
            fprintf(stderr, "NOTE: Invoking the experimental make tool.\n");
        }
        // NOTE: This will only work for argv == 1, and because cppmain ignores the -P flag!
        void* tmp = NULL;
        mk_main(argc, argv, &tmp);
        return -1; // TODO: mk_main returns void but also has some exit calls, this behaviour needs to be checked to return error codes properly
    }
#endif
#ifdef TOOL_SH
    else if (argc > argi && !strcmp(argv[argi], "-S")) { /* Invoke the built-in make tool instead of the backend. */
        if (!ccb.silent) {
            fprintf(stderr, "NOTE: Invoking the experimental shell tool.\n");
        }
        // NOTE: This will only work for argv == 1, and because cppmain ignores the -P flag!
        void* tmp = NULL;
        return sh_main(argc, argv);
    }
#endif

    if (argc > argi && !strcmp(argv[argi], "--usage")) {
        usage(argc, argv, -1);
        return EXIT_SUCCESS;
    }

    if (argc > argi + 1 && !strcmp(argv[argi], "--input")) {
        if (!ccb.silent) {
            fprintf(stderr, "NOTE: Using input from '%s'.\n", argv[argi + 1]);
        }
        ccb.input = fopen(argv[argi + 1], "r");
        argi += 2;
    }

    if (argc > argi + 1 && !strcmp(argv[argi], "--output")) {
        if (!ccb.silent) {
            fprintf(stderr, "NOTE: Writing output to '%s'.\n", argv[argi + 1]);
        }
        ccb.output = fopen(argv[argi + 1], "w");
        argi += 2;
    }
    else if (argc > argi + 1 && !strcmp(argv[argi], "--append")) {
        if (!ccb.silent) {
            fprintf(stderr, "NOTE: Appending output from '%s'.\n", argv[argi]);
        }
        ccb.output = fopen(argv[argi + 1], "a");
        argi += 2;
    }

    if (ccb.input == NULL) {
        fprintf(stderr, "ERROR: Failed to open input file.\n");
    }

    if (ccb.output == NULL) {
        fprintf(stderr, "ERROR: Failed to open output file.\n");
    }

    if (argi != argc) {
        usage(argc, argv, argi);
        return EXIT_FAILURE;
    }

    int result = startcompile(&ccb);

    if (ccb.input != stdin) {
        fclose(ccb.input);
    }
    if (ccb.output != stdout) {
        fclose(ccb.output);
    }

    if (result) {
        return EXIT_SUCCESS;
    }
    else {
        return EXIT_FAILURE;
    }
}
