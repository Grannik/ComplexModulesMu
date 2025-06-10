#define _XOPEN_SOURCE_EXTENDED 1
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "tagTermiosMu/tag_termios_mu.h"
#include "utils.h"

#define MAX_INPUT 32768
#define MAX_LINES 2048

wchar_t *lines[MAX_LINES];
int total_lines = 0;
int start_line = 0;
wchar_t *screen_buffer[MAX_LINES]; // Buffer to store current screen content
int screen_buffer_size = 0; // Number of lines in screen_buffer

void init_screen_buffer(int max_y) {
    // Initialize screen_buffer with empty strings
    for (int i = 0; i < max_y && i < MAX_LINES; ++i) {
        screen_buffer[i] = malloc((MAX_INPUT + 1) * sizeof(wchar_t));
        if (!screen_buffer[i]) {
            fwprintf(stderr, L" \033[1;31mError:\033[0;31m Failed to allocate memory for screen_buffer\033[0m\n");
            free_lines(lines, total_lines);
            for (int j = 0; j < i; ++j) free(screen_buffer[j]);
            exit(1);
        }
        screen_buffer[i][0] = L'\0'; // Empty string
    }
    screen_buffer_size = max_y < MAX_LINES ? max_y : MAX_LINES;
}

void free_screen_buffer(void) {
    for (int i = 0; i < screen_buffer_size; ++i) {
        if (screen_buffer[i]) free(screen_buffer[i]);
    }
}

void draw_screen(int max_y, int max_x) {
    static int first_draw = 1;
    if (first_draw) {
        wprintf(L"\033[?1049h"); // Switch to alternate buffer
        wprintf(L"\033[?25l");   // Hide cursor
        init_screen_buffer(max_y); // Initialize screen_buffer
        first_draw = 0;
    }

    // Update only changed lines
    for (int row = 0; row < max_y && start_line + row < total_lines; ++row) {
        const wchar_t *new_line = lines[start_line + row];
        // Check if the line has changed
        if (row >= screen_buffer_size || wcscmp(screen_buffer[row], new_line) != 0) {
            wprintf(L"\033[%d;1H\033[K", row + 1); // Move to line and clear it
            print_tagged_text(new_line, 0, row, max_x);
            wprintf(L"\033[0m"); // Reset attributes
            // Update screen_buffer
            wcsncpy(screen_buffer[row], new_line, MAX_INPUT);
            screen_buffer[row][MAX_INPUT] = L'\0'; // Ensure null termination
        }
    }

    // Clear remaining lines if necessary
    for (int row = total_lines - start_line; row < max_y && row < screen_buffer_size; ++row) {
        if (screen_buffer[row][0] != L'\0') { // Only clear if not already empty
            wprintf(L"\033[%d;1H\033[K", row + 1);
            screen_buffer[row][0] = L'\0'; // Update buffer to empty
        }
    }

    fflush(stdout); // Single flush after all updates
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fwprintf(stderr, L" \033[1;31mError:\033[0;31m Usage: %s <file.mu>\033[0m\n", argv[0]);
        return 1;
    }

    wchar_t *wbuffer = NULL;
    size_t wlen = 0;

    if (read_mu_file(argv[1], &wbuffer, &wlen) != 0) {
        return 1;
    }

    wchar_t output[MAX_INPUT];
    parse_tags(wbuffer, output, MAX_INPUT);
    free(wbuffer);

    split_lines(output, lines, &total_lines);

    struct termios orig_termios;
    setup_terminal(&orig_termios);

    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    int max_y = ws.ws_row - 1;
    int max_x = ws.ws_col;

    draw_screen(max_y, max_x);

    while (1) {
        char ch = getchar();
        if (ch == 'q') break;
        if (ch == '\033') {
            if (getchar() == '[') {
                char arrow = getchar();
                if (arrow == 'A' && start_line > 0) {
                    start_line--;
                    draw_screen(max_y, max_x);
                }
                if (arrow == 'B' && start_line + max_y < total_lines) {
                    start_line++;
                    draw_screen(max_y, max_x);
                }
            }
        }
    }

    restore_terminal(&orig_termios);
    wprintf(L"\033[?25h"); // Show cursor
    wprintf(L"\033[?1049l\033[H"); // Return to normal buffer and move cursor to (0,0)
    fflush(stdout);

    free_screen_buffer(); // Free screen_buffer
    free_lines(lines, total_lines);
    return 0;
}
