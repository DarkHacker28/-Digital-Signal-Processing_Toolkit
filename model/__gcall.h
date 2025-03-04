/**
 * @file __gcall.h
 * @details Short (thus contained in a header file) gnuplot calling interface
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define __GCALL_SIMPLE_CALL_CMD_BUF_SIZE 50
#define __GCALL_ADVANCED_CALL_CMD_BUF_SIZE 300

void __gnuplot_simple_call(const char* script_path) {
    char cmd_buf[__GCALL_SIMPLE_CALL_CMD_BUF_SIZE];
    snprintf(cmd_buf, __GCALL_SIMPLE_CALL_CMD_BUF_SIZE, "gnuplot %s", script_path);
    int status = system(cmd_buf);
    if (status != 0) {
        fprintf(stderr, "Error: gnuplot call failed (__gnuplot_simple_call)\n");
    }
}

typedef struct {
    char* argname;
    char* argval;
} gnuplot_arg_t;

typedef struct {
    gnuplot_arg_t* args;
    uint8_t num_args;
} gnuplot_arglist_t;

void __gnuplot_arglist_aloc_args (gnuplot_arglist_t* pArglist) { pArglist->args = (gnuplot_arg_t*) malloc (pArglist->num_args * sizeof(gnuplot_arg_t)); }
void __gnuplot_arglist_free_args (gnuplot_arglist_t* pArglist) { free (pArglist->args); }

/**
 * @note Arguments passed int the arglist can't contain quotes and apostrophes
 * @todo Handle the buffer limit gracefully [TODO]
*/
void __gnuplot_advanced_call(const char* script_path, gnuplot_arglist_t* pGnuplotArglist) {
    char cmd_buf[__GCALL_ADVANCED_CALL_CMD_BUF_SIZE];
    char* hBuf = cmd_buf;
    uint32_t rem = __GCALL_ADVANCED_CALL_CMD_BUF_SIZE;
    int n = 0;
    n = snprintf(hBuf, rem, "gnuplot "); rem -=n; hBuf +=n;
    
    for (uint32_t i = 0; i < pGnuplotArglist->num_args; i++) {
        gnuplot_arg_t* pArg = pGnuplotArglist->args + i;
        n = snprintf(hBuf, rem, "-e \"%s='%s'\" ", pArg->argname, pArg->argval); rem -=n; hBuf +=n;
    }
    n = snprintf(hBuf, rem, " %s", script_path); rem -=n; hBuf +=n;

    // fprintf(stdout, "[dbg] Gnuplot command (advanced call): %s\n", cmd_buf);
    int status = system(cmd_buf);
    if (status != 0) {
        fprintf(stderr, "Error: gnuplot call failed (__gnuplot_advanced_call)\n");
    }
}