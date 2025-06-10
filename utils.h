#ifndef UTILS_H
#define UTILS_H

#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "tagTermiosMu/tag_termios_mu.h"

#define MAX_INPUT 32768
#define MAX_LINES 2048
#define MAX_QUERY 256

/* Reading and validating .mu file, converting to wide string */
int read_mu_file(const char *filename, wchar_t **wbuffer, size_t *wlen);

/* Splitting wide string buffer into lines */
void split_lines(const wchar_t *buffer, wchar_t **lines, int *total_lines);

/* Finding tag positions in lines */
void find_tags(wchar_t **lines, int total_lines, int *tag_indices, int *total_tags);

/* Setting up terminal for non-canonical input and hiding cursor */
void setup_terminal(struct termios *orig_termios);

/* Restoring terminal to original state and showing cursor */
void restore_terminal(struct termios *orig_termios);

/* Freeing memory allocated for lines */
void free_lines(wchar_t **lines, int total_lines);

/* Printing error message to stderr */
void print_error(const wchar_t *message, const char *arg);

#endif /* UTILS_H */
