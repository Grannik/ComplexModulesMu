#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "utils.h"
#include "tagTermiosMu/tag_termios_mu.h"

#define MAX_INPUT 32768
#define MAX_LINES 2048
#define MAX_QUERY 256

/* Reading and validating .mu file, converting to wide string */
int read_mu_file(const char *filename, wchar_t **wbuffer, size_t *wlen) {
    if (strlen(filename) < 3 || strcmp(filename + strlen(filename) - 3, ".mu") != 0) {
        fwprintf(stderr, L" \033[1;31mError:\033[0;31m Invalid file extension. Expected a .mu file.\033[0m\n");
        return 1;
    }

    setlocale(LC_ALL, "");

    FILE *file = fopen(filename, "r");
    if (!file) {
        fwprintf(stderr, L" \033[1;31mError:\033[0;31m Failed to open file: %s\033[0m\n", filename);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    if (file_size > MAX_INPUT) {
        fclose(file);
        fwprintf(stderr, L" \033[1;31mError:\033[0;31m File is too large\033[0m\n");
        return 1;
    }
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        fwprintf(stderr, L" \033[1;31mError:\033[0;31m Failed to allocate memory\033[0m\n");
        return 1;
    }
    size_t read_size = fread(buffer, 1, file_size, file);
    buffer[read_size] = '\0';
    fclose(file);

    if (read_size != (size_t)file_size) {
        free(buffer);
        fwprintf(stderr, L" \033[1;31mError:\033[0;31m Failed to read file\033[0m\n");
        return 1;
    }

    /* Handle UTF-8 BOM */
    if (read_size >= 3 && 
        (unsigned char)buffer[0] == 0xEF &&
        (unsigned char)buffer[1] == 0xBB &&
        (unsigned char)buffer[2] == 0xBF) {
        memmove(buffer, buffer + 3, read_size - 3);
        read_size -= 3;
        buffer[read_size] = '\0';
    }

    *wlen = mbstowcs(NULL, buffer, 0);
    if (*wlen == (size_t)-1) {
        free(buffer);
        fwprintf(stderr, L" \033[1;31mError:\033[0;31m Invalid file encoding\033[0m\n");
        return 1;
    }

    *wbuffer = malloc((*wlen + 1) * sizeof(wchar_t));
    if (!*wbuffer) {
        free(buffer);
        fwprintf(stderr, L" \033[1;31mError:\033[0;31m Failed to allocate memory for wbuffer\033[0m\n");
        return 1;
    }
    mbstowcs(*wbuffer, buffer, *wlen + 1);
    free(buffer);
    return 0;
}

/* Splitting wide string buffer into lines */
void split_lines(const wchar_t *buffer, wchar_t **lines, int *total_lines) {
    if (!buffer) return;

    int i = 0;
    int line_index = 0;

    while (buffer[i] != L'\0' && line_index < MAX_LINES) {
        const wchar_t *line_start = &buffer[i];
        int total_len = 0;

        while (buffer[i] != L'\n' && buffer[i] != L'\0' && i < MAX_INPUT) {
            total_len++;
            i++;
        }

        if (total_len >= MAX_INPUT) {
            fwprintf(stderr, L" \033[1;31mError:\033[0;31m Line %d is too long\033[0m\n", line_index);
            free_lines(lines, line_index);
            exit(1);
        }

        lines[line_index] = malloc((total_len + 1) * sizeof(wchar_t));
        if (!lines[line_index]) {
            fwprintf(stderr, L" \033[1;31mError:\033[0;31m Failed to allocate memory for line %d\033[0m\n", line_index);
            free_lines(lines, line_index);
            exit(1);
        }

        wcsncpy(lines[line_index], line_start, total_len);
        lines[line_index][total_len] = L'\0';

        line_index++;
        if (buffer[i] == L'\n') i++;
    }
    *total_lines = line_index;
}

/* Finding tag positions in lines */
void find_tags(wchar_t **lines, int total_lines, int *tag_indices, int *total_tags) {
    *total_tags = 0;
    tag_indices[*total_tags] = 0;
    for (int i = 0; i < total_lines; ++i) {
        if (lines[i][0] == L'<') {
            tag_indices[++(*total_tags)] = i;
        }
        if (i == total_lines - 1) {
            tag_indices[++(*total_tags)] = total_lines;
        }
    }
}

/* Setting up terminal for non-canonical input and hiding cursor */
void setup_terminal(struct termios *orig_termios) {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, orig_termios);
    new_termios = *orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    new_termios.c_cc[VMIN] = 1;
    new_termios.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    wprintf(L"\033[?25l");
}

/* Restoring terminal to original state and showing cursor */
void restore_terminal(struct termios *orig_termios) {
    wprintf(L"\033[?25h");
    wprintf(L"\033[?1049l");
    wprintf(L"\033[0m");
    wprintf(L"\033[H\033[J");
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSANOW, orig_termios);
}

/* Freeing memory allocated for lines */
void free_lines(wchar_t **lines, int total_lines) {
    for (int i = 0; i < total_lines; ++i) {
        if (lines[i]) free(lines[i]);
    }
}

/* Printing error message to stderr */
void print_error(const wchar_t *message, const char *arg) {
    if (arg) {
        fwprintf(stderr, L" \033[1;31m%ls:\033[0;31m %s\033[0m\n", message, arg);
    } else {
        fwprintf(stderr, L" \033[1;31m%ls\033[0m\n", message);
    }
}
