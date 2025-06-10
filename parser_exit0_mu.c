#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdio.h>
#include "tagTermiosMu/tag_termios_mu.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    wchar_t *wbuffer = NULL;
    size_t wlen = 0;

    if (argc != 2) {
        fwprintf(stderr, L" \033[1;31mError:\033[0;31m Usage: %s <file.mu>\033[0m\n", argv[0]);
        return 1;
    }

    if (read_mu_file(argv[1], &wbuffer, &wlen) != 0) {
        return 1;
    }

    wchar_t output[MAX_INPUT + 1];
    parse_tags(wbuffer, output, MAX_INPUT);
    print_tagged_text(output, 0, 0, 80);  // max_x = 80 — ширина терминала

    free(wbuffer);
    return 0;
}
