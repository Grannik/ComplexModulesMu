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
#define MAX_QUERY 256

wchar_t *lines[MAX_LINES];
int total_lines = 0;
int start_line = 0;
int current_tag = 0;
int tag_indices[MAX_LINES];
int total_tags = 0;
wchar_t *screen_buffer[MAX_LINES]; // Buffer to store current screen content
int screen_buffer_size = 0; // Number of lines in screen_buffer
wchar_t last_query[MAX_QUERY] = {0}; // Buffer to store last displayed query

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

int navigate_to_tag_or_word(const wchar_t *input, int max_y) {
    // Check if input is a tag (e.g., <00>)
    if (input[0] == L'<') {
        for (int i = 0; i < total_tags; ++i) {
            if (wcscmp(lines[tag_indices[i]], input) == 0) {
                current_tag = i;
                start_line = tag_indices[i] + 1;
                while (start_line < total_lines && lines[start_line][0] == L'<') {
                    start_line++;
                }
                return 0; // Success
            }
        }
    }
    // If no match, display error in red
    wprintf(L"\033[%d;1H\033[K", max_y); // Move to last line, clear it
    wprintf(L"\033[31m Tag not found\033[0m");
    wcsncpy(screen_buffer[max_y - 1], L"Tag not found", MAX_INPUT);
    screen_buffer[max_y - 1][MAX_INPUT] = L'\0';
    fflush(stdout);
    return 1; // Error
}

void draw_screen(int max_y, int max_x) {
    static int first_draw = 1;
    if (first_draw) {
        wprintf(L"\033[?1049h"); // Switch to alternate buffer
        wprintf(L"\033[?25l");   // Hide cursor
        init_screen_buffer(max_y); // Initialize screen_buffer
        first_draw = 0;
    }

    int tag_start = tag_indices[current_tag];
    int tag_end = (current_tag + 1 < total_tags) ? tag_indices[current_tag + 1] : total_lines;

    if (start_line <= tag_start) {
        start_line = tag_start + 1;
        while (start_line < tag_end && lines[start_line][0] == L'<') {
            start_line++;
        }
    }
    if (start_line >= tag_end) {
        start_line = tag_end - 1 > tag_start ? tag_end - 1 : tag_start + 1;
        while (start_line > tag_start && lines[start_line][0] == L'<') {
            start_line--;
        }
    }

    int row = 0;
    // Display only content lines between tag_start and tag_end
    for (int i = start_line; i < tag_end && row < max_y - 1; ++i) {
        if (lines[i][0] != L'<') {
            const wchar_t *new_line = lines[i];
            // Check if the line has changed
            if (row >= screen_buffer_size || wcscmp(screen_buffer[row], new_line) != 0) {
                wprintf(L"\033[%d;1H\033[K", row + 1); // Move to line and clear it
                print_tagged_text(new_line, 0, row, max_x);
                wprintf(L"\033[0m"); // Reset attributes
                // Update screen_buffer
                wcsncpy(screen_buffer[row], new_line, MAX_INPUT);
                screen_buffer[row][MAX_INPUT] = L'\0'; // Ensure null termination
            }
            row++;
        }
    }

    // Clear remaining lines if necessary
    for (; row < max_y - 1 && row < screen_buffer_size; ++row) {
        if (screen_buffer[row][0] != L'\0') { // Only clear if not already empty
            wprintf(L"\033[%d;1H\033[K", row + 1);
            screen_buffer[row][0] = L'\0'; // Update buffer to empty
        }
    }

    // Draw input prompt on last line if query has changed or error is not displayed
    wchar_t prompt[MAX_INPUT];
    swprintf(prompt, MAX_INPUT, L"Enter tag: %ls", last_query);
    if (wcscmp(screen_buffer[max_y - 1], prompt) != 0) {
        wprintf(L"\033[%d;2H\033[K Enter tag: %ls", max_y, last_query);
        wcsncpy(screen_buffer[max_y - 1], prompt, MAX_INPUT);
        screen_buffer[max_y - 1][MAX_INPUT] = L'\0';
    }

    fflush(stdout); // Single flush after all updates
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fwprintf(stderr, L" \033[1;31mError:\033[0;31m Usage: %s <file.mu>\033[0m\n", argv[0]);
        return 1;
    }

    setlocale(LC_ALL, "");

    wchar_t *wbuffer = NULL;
    size_t wlen = 0;

    if (read_mu_file(argv[1], &wbuffer, &wlen) != 0) {
        return 1;
    }

    wchar_t output[MAX_INPUT];
    parse_tags(wbuffer, output, MAX_INPUT);
    free(wbuffer);

    split_lines(output, lines, &total_lines);
    find_tags(lines, total_lines, tag_indices, &total_tags);

    struct termios orig_termios;
    setup_terminal(&orig_termios);

    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    int max_y = ws.ws_row - 1;
    int max_x = ws.ws_col;

    if (total_tags > 0) {
        start_line = tag_indices[0] + 1;
        while (start_line < total_lines && lines[start_line][0] == L'<') {
            start_line++;
        }
    }
    draw_screen(max_y, max_x);

    wchar_t query[MAX_QUERY] = {0};
    int query_pos = 0;
    int error_displayed = 0; // Flag to track if error message is shown

    while (1) {
        char ch = getchar();
        if (ch == 'q') break;
        if (ch == '\033') {
            if (getchar() == '[') {
                char arrow = getchar();
                if (arrow == 'A') {
                    int tag_start = tag_indices[current_tag];
                    int prev_line = start_line - 1;
                    while (prev_line > tag_start && lines[prev_line][0] == L'<') {
                        prev_line--;
                    }
                    if (prev_line > tag_start) {
                        start_line = prev_line;
                        error_displayed = 0; // Clear error on navigation
                        draw_screen(max_y, max_x);
                    }
                }
                if (arrow == 'B') {
                    int tag_end = (current_tag + 1 < total_tags) ? tag_indices[current_tag + 1] : tag_indices[total_tags];
                    int next_line = start_line + 1;
                    while (next_line < tag_end && lines[next_line][0] == L'<') {
                        next_line++;
                    }
                    if (next_line < tag_end) {
                        start_line = next_line;
                        error_displayed = 0; // Clear error on navigation
                        draw_screen(max_y, max_x);
                    }
                }
                if (arrow == 'D' && current_tag > 0) {
                    current_tag--;
                    start_line = tag_indices[current_tag] + 1;
                    while (start_line < total_lines && lines[start_line][0] == L'<') {
                        start_line++;
                    }
                    error_displayed = 0; // Clear error on navigation
                    draw_screen(max_y, max_x);
                }
                if (arrow == 'C' && current_tag < total_tags - 1) {
                    current_tag++;
                    start_line = tag_indices[current_tag] + 1;
                    while (start_line < total_lines && lines[start_line][0] == L'<') {
                        start_line++;
                    }
                    error_displayed = 0; // Clear error on navigation
                    draw_screen(max_y, max_x);
                }
            }
        } else if (ch == '\n' && query_pos > 0) {
            query[query_pos] = L'\0';
            wcsncpy(last_query, query, MAX_QUERY); // Update last_query
            error_displayed = navigate_to_tag_or_word(query, max_y);
            query_pos = 0;
            wmemset(query, 0, MAX_QUERY);
            if (!error_displayed) {
                wmemset(last_query, 0, MAX_QUERY); // Clear last_query on successful navigation
                draw_screen(max_y, max_x);
            }
        } else if (ch == 127 && query_pos > 0) { // Backspace
            query_pos--;
            query[query_pos] = L'\0';
            wcsncpy(last_query, query, MAX_QUERY); // Update last_query
            error_displayed = 0; // Clear error on input
            draw_screen(max_y, max_x); // Redraw to update prompt
        } else if (ch >= 32 && ch <= 126 && query_pos < MAX_QUERY - 1) {
            query[query_pos++] = (wchar_t)ch;
            wcsncpy(last_query, query, MAX_QUERY); // Update last_query
            error_displayed = 0; // Clear error on input
            draw_screen(max_y, max_x); // Redraw to update prompt
        } else if (error_displayed && ch >= 32 && ch <= 126) {
            // Clear error on any other key press and start new input
            query_pos = 0;
            wmemset(query, 0, MAX_QUERY);
            query[query_pos++] = (wchar_t)ch;
            wcsncpy(last_query, query, MAX_QUERY); // Update last_query
            error_displayed = 0;
            draw_screen(max_y, max_x); // Redraw to update prompt
        }
    }

    restore_terminal(&orig_termios);
    free_screen_buffer(); // Free screen_buffer
    free_lines(lines, total_lines);

    return 0;
}
