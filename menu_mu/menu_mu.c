#define _XOPEN_SOURCE_EXTENDED 1
#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <locale.h>
#include <wchar.h>
#include <stdio.h>
#include "tags_mu_ncurses/tags_mu_ncurses.h"
// #include "menu_mu.h"

void exit_program(void) {
    clear();
    refresh();
    endwin();
    exit(0);
}

void draw_menu(void) {

    for (int i = 0; i < 27; i++) {
    mvprintw(i, 0, "%*s", 64, "");
}
    attron(COLOR_PAIR(4));
//--- Цикл заглавной части рамки
    int menu_text_length = 11;
    int total_width = 64; // 64
    int left_length = (total_width - menu_text_length - 2) / 2;
        mvprintw(0, 0, "╔");
    for (int j = 1; j <= left_length; j++) mvprintw(0, j, "═");
    mvprintw(0, left_length + 1, "╡ Menu mu ╞");
    for (int j = left_length + menu_text_length + 1; j < total_width - 1; j++) mvprintw(0, j, "═");
    mvprintw(0, total_width - 1, "╗");
// Цикл боковых рамок.
    for (int i = 1; i <= 29; i++) {
        mvprintw(i, 0, "║");
        mvprintw(i, 63, "║");
    }
// ----- Color tags
    int lower_menu_text_length = 14;
    int lower_left_length = (62 - lower_menu_text_length) / 2;
    int lower_menu_start_pos = lower_left_length + 1;
    mvprintw(2, 0, "╟");
    for (int j = 1; j <= lower_left_length; j++) {
    mvprintw(2, j, "─");
}
    mvprintw(2, lower_menu_start_pos, "┤ Color tags ├");
    for (int j = lower_menu_start_pos + lower_menu_text_length; j < 64; j++) {
    mvprintw(2, j, "─");
}
    mvprintw(2, 63, "╢");
//--- Special tags
    int special_menu_text_length = 16;
    int special_left_length = (61 - special_menu_text_length) / 2;
    int special_menu_start_pos = special_left_length + 1;
    mvprintw(24, 0, "╟");
    for (int j = 1; j <= special_left_length; j++) {
    mvprintw(24, j, "─");
}
    mvprintw(24, special_menu_start_pos, "┤ Special tags ├");
    for (int j = special_menu_start_pos + special_menu_text_length; j < 63; j++) {
    mvprintw(24, j, "─");
}
    mvprintw(24, 63, "╢");
//--- Перемычка после специальных тегов ------------
    mvprintw(28, 0, "╟");          // Левая часть.
    for (int j = 1; j < 63; j++) {
    mvprintw(28, j, "─");          // Сама перемычка
}
    mvprintw(28, 63, "╢");         // Правая часть.
//--- Нижняя часть рамки ---------------------------
    mvprintw(30, 0, "╚");          // Левая часть.
    for (int j = 1; j < 63; j++) {
    mvprintw(30, j, "═");          // Сама перемычка
}
   mvprintw(30, 63, "╝");         // Правая часть.
//--- Определение текстовых полей с тегами ---------
    attroff(COLOR_PAIR(4));
    const wchar_t *title = L"          <r>*** Tag information in the TTY menu ***";
    const wchar_t *header = L"+-----+----------+--------------------+------------------+<>| Tag | Code     | Name               | Function         |<>+-----+----------+--------------------+------------------+";
    const wchar_t *body = L"| <</>>  | \\n       | Transfer line      | Перенос строки   |<>| <</>/> | \\033[0m  | Reset              | Сброс атрибутов  |<>|     | <</>#><</>/#>  | Hide text          | Скрыть текст     |";
    const wchar_t *footer = L"<i>|↑ Up | ↓ Down   | ↵ Select           | q Quit           | ";
// Парсинг и вывод title
    {
    wchar_t title_buf[1025];
    attr_t attributes[1024];
        int attr_count = 0;
        parse_tags(title, title_buf, 1024, attributes, &attr_count);
        print_tagged_text(title_buf, 2, 1, attributes, attr_count, 62);
    }
// Парсинг и вывод header
    {
    wchar_t header_buf[1025];
    attr_t attributes[1024];
        int attr_count = 0;
        parse_tags(header, header_buf, 1024, attributes, &attr_count);
        print_tagged_text(header_buf, 3, 3, attributes, attr_count, 62);
    }
// Парсинг и вывод body
    {
    wchar_t body_buf[1025];
    attr_t attributes[1024];
        int attr_count = 0;
        parse_tags(body, body_buf, 1024, attributes, &attr_count);
        print_tagged_text(body_buf, 3, 25, attributes, attr_count, 62);
    }
    // Парсинг и вывод footer
    {
    wchar_t footer_buf[1025];
    attr_t attributes[1024];
        int attr_count = 0;
        parse_tags(footer, footer_buf, 1024, attributes, &attr_count);
        print_tagged_text(footer_buf, 3, 29, attributes, attr_count, 64);
    }
    refresh();
}
/**/
void cleanup(int sig) {
    reset_shell_mode();
    system("tput reset");
    if (sig != 0) {
        fprintf(stderr, "\033[31;1m Error:\033[0m\033[31m Program terminated by signal %d\033[0m\n", sig);
        exit(1);
    }
    exit(0);
}
// ---
typedef struct {
    wchar_t *description;
    wchar_t *details[30];
    int detail_lines;
} MenuItem;
// --- init_menu_items ---------------
void init_menu_items(MenuItem items[]) {
    items[0] = (MenuItem){
        .description = L"<a>| <<a>a> | \\033[30m | Black text         | Текст: черный    |",
        .details = {L" <a>| <<a>a> | \\033[30m | Black text | Текст: черный |<></a> <</>/a> закрывающий тег"},
        .detail_lines = 2
    };
    items[1] = (MenuItem){
        .description = L"<b>| <<b>b> | \\033[31m | Red text           | Текст: красный   |",
        .details = {L" <b>| <<b>b> | \\033[31m | Red text | Текст: красный |<></b> <</>/b> закрывающий тег"},
        .detail_lines = 2
    };
    items[2] = (MenuItem){
        .description = L"<c>| <<c>c> | \\033[32m | Green text         | Текст: зеленый   |",
        .details = {L" <c>| <<c>c> | \\033[32m | Green text | Текст: зеленый |<></c> <</>/c> закрывающий тег"},
        .detail_lines = 2
    };
    items[3] = (MenuItem){
        .description = L"<d>| <<d>d> | \\033[33m | Yellow text        | Текст: желтый    |",
        .details = {L" <d>| <<d>d> | \\033[33m | Yellow text | Текст: желтый |<></d> <</>/d> закрывающий тег"},
        .detail_lines = 2
    };
    items[4] = (MenuItem){
        .description = L"<e>| <<e>e> | \\033[34m | Blue text          | Текст: синий     |",
        .details = {L" <e>| <<e>e> | \\033[34m | Blue text | Текст: синий |<></e> <</>/e> закрывающий тег"},
        .detail_lines = 2
    };
    items[5] = (MenuItem){
        .description = L"<f>| <<f>f> | \\033[35m | Magenta text       | Текст: пурпурный |",
        .details = {L" <f>| <<f>f> | \\033[35m | Magenta text | Текст: пурпурный |<></f> <</>/f> закрывающий тег"},
        .detail_lines = 2
    };
    items[6] = (MenuItem){
        .description = L"<g>| <<g>g> | \\033[36m | Cyan text          | Текст: голубой   |",
        .details = {L" <g>| <<g>g> | \\033[36m | Cyan text | Текст: голубой |<></g> <</>/g> закрывающий тег"},
        .detail_lines = 2
    };
    items[7] = (MenuItem){
        .description = L"<h>| <<h>h> | \\033[37m | White text         | Текст: белый     |",
        .details = {L" <h>| <<h>h> | \\033[37m | White text | Текст: белый |<></h> <</>/h> закрывающий тег"},
        .detail_lines = 2
    };
    items[8] = (MenuItem){
        .description = L"<i>| <<i>i> | \\033[37m | Gray text          | Текст: серый     |",
        .details = {L" <i>| <<i>i> | \\033[37m | Gray text | Текст: серый |<></i> <</>/i> закрывающий тег"},
        .detail_lines = 2
    };
    items[9] = (MenuItem){
        .description = L"<j>| <<j>j> | \\033[40m | Black background   | Фон: черный      |",
        .details = {L" <j>| <<j>j> | \\033[41m | Black background | Фон: черный |<></j> <</j>/j> закрывающий тег<>+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+<>|                      * Двойные теги *                       |<>+---------+-------------+------------------+------------------+<><ja>| <<ja>ja>    | \\033[30;41m | Black text       | Текст: черный    |<><jb>| <<jb>jb>    | \\033[31;41m | Red text         | Текст: красный   |<><jc>| <<jc>jc>    | \\033[32;41m | Green text       | Текст: зеленый   |<><jd>| <<jd>jd>    | \\033[33;41m | Yellow text      | Текст: желтый    |<><je>| <<je>je>    | \\033[34;41m | Blue text        | Текст: синий     |<><jf>| <<jf>jf>    | \\033[35;41m | Magenta text     | Текст: пурпурный |<><jg>| <<jg>jg>    | \\033[36;41m | Cyan text        | Текст: голубой   |<><jh>| <<jh>jh>    | \\033[37;41m | White text       | Текст: белый     |<><ji>| <<ji>ji>    | \\033[37;41m | Gray text        | Текст: серый     |<></>+~~~~~~~~~+~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+"},
        .detail_lines = 15
    };
    items[10] = (MenuItem){
        .description = L"<k>| <<k>k> | \\033[41m | Red background     | Фон: красный     |",
        .details = {L" <k>| <<k>k> | \\033[41m | Red background | Фон: красный |<></k> <</k>/k> закрывающий тег<>+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+<>|                      * Двойные теги *                       |<>+---------+-------------+------------------+------------------+<><ka>| <<ka>ka>    | \\033[30;41m | Black text       | Текст: черный    |<><kb>| <<kb>kb>    | \\033[31;41m | Red text         | Текст: красный   |<><kc>| <<kc>kc>    | \\033[32;41m | Green text       | Текст: зеленый   |<><kd>| <<kd>kd>    | \\033[33;41m | Yellow text      | Текст: желтый    |<><ke>| <<ke>ke>    | \\033[34;41m | Blue text        | Текст: синий     |<><kf>| <<kf>kf>    | \\033[35;41m | Magenta text     | Текст: пурпурный |<><kg>| <<kg>kg>    | \\033[36;41m | Cyan text        | Текст: голубой   |<><kh>| <<kh>kh>    | \\033[37;41m | White text       | Текст: белый     |<><ki>| <<ki>ki>    | \\033[37;41m | Gray text        | Текст: серый     |<></>+~~~~~~~~~+~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+"},
        .detail_lines = 15
    };
    items[11] = (MenuItem){
        .description = L"<l>| <<l>l> | \\033[42m | <r>Green background   | Фон: зеленый     |",
        .details = {L" <l>| <<l>l> | \\033[42m | Green background | Фон: зеленый |<></l> <</l>/l> закрывающий тег<>+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+<>|                      * Двойные теги *                       |<>+---------+-------------+------------------+------------------+<><la>| <<la>la>    | \\033[30;42m | Black text       | Текст: черный    |<><lb>| <<lb>lb>    | \\033[31;42m | Red text         | Текст: красный   |<><lc>| <<lc>lc>    | \\033[32;42m | Green text       | Текст: зеленый   |<><ld>| <<ld>ld>    | \\033[33;42m | Yellow text      | Текст: желтый    |<><le>| <<le>le>    | \\033[34;42m | Blue text        | Текст: синий     |<><lf>| <<lf>lf>    | \\033[35;42m | Magenta text     | Текст: пурпурный |<><lg>| <<lg>lg>    | \\033[36;42m | Cyan text        | Текст: голубой   |<><lh>| <<lh>lh>    | \\033[37;42m | White text       | Текст: белый     |<><li>| <<li>li>    | \\033[37;42m | Gray text        | Текст: серый     |<></>+~~~~~~~~~+~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+"},
        .detail_lines = 15
    };
    items[12] = (MenuItem){
        .description = L"<m>| <<m>m> | \\033[43m | Yellow background  | Фон: желтый      |",
        .details = {L" <m>| <<m>m> | \\033[42m | Yellow background | Фон: желтый |<></m> <</m>/m> закрывающий тег<>+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+<>|                      * Двойные теги *                       |<>+---------+-------------+------------------+------------------+<><ma>| <<ma>ma>    | \\033[30;42m | Black text       | Текст: черный    |<><mb>| <<mb>mb>    | \\033[31;42m | Red text         | Текст: красный   |<><mc>| <<mc>mc>    | \\033[32;42m | Green text       | Текст: зеленый   |<><md>| <<md>md>    | \\033[33;42m | Yellow text      | Текст: желтый    |<><me>| <<me>me>    | \\033[34;42m | Blue text        | Текст: синий     |<><mf>| <<mf>lf>    | \\033[35;42m | Magenta text     | Текст: пурпурный |<><mg>| <<mg>mg>    | \\033[36;42m | Cyan text        | Текст: голубой   |<><mh>| <<mh>mh>    | \\033[37;42m | White text       | Текст: белый     |<><mi>| <<mi>mi>    | \\033[37;42m | Gray text        | Текст: серый     |<></>+~~~~~~~~~+~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+"},
        .detail_lines = 15
    };
    items[13] = (MenuItem){
        .description = L"<n>| <<n>n> | \\033[44m |<r> Blue background    | Фон: синий       |",
        .details = {L" <n>| <<n>n> | \\033[44m | Blue background | Фон: синий |<></n> <</n>/n> закрывающий тег<>+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+<>|                      * Двойные теги *                       |<>+---------+-------------+------------------+------------------+<><na>| <<na>na>    | \\033[30;44m | Black text       | Текст: черный    |<><nb>| <<nb>nb>    | \\033[31;44m | Red text         | Текст: красный   |<><nc>| <<nc>nc>    | \\033[32;44m | Green text       | Текст: зеленый   |<><nd>| <<nd>nd>    | \\033[33;44m | Yellow text      | Текст: желтый    |<><ne>| <<ne>ne>    | \\033[34;44m | Blue text        | Текст: синий     |<><nf>| <<nf>nf>    | \\033[35;44m | Magenta text     | Текст: пурпурный |<><ng>| <<ng>ng>    | \\033[36;44m | Cyan text        | Текст: голубой   |<><nh>| <<nh>nh>    | \\033[37;44m | White text       | Текст: белый     |<><ni>| <<ni>ni>    | \\033[37;44m | Gray text        | Текст: серый     |<></>+~~~~~~~~~+~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+"},
        .detail_lines = 15
    };
    items[14] = (MenuItem){
        .description = L"<o>| <<o>o> | \\033[45m | Magenta background | Фон: пурпурный   |",
        .details = {L" <o>| <<o>o> | \\033[45m | Magenta background | Фон: пурпурный |<></o> <</o>/o> закрывающий тег<>+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+<>|                      * Двойные теги *                       |<>+---------+-------------+------------------+------------------+<><oa>| <<oa>oa>    | \\033[30;45m | Black text       | Текст: черный    |<><ob>| <<ob>ob>    | \\033[31;45m | Red text         | Текст: красный   |<><oc>| <<oc>oc>    | \\033[32;45m | Green text       | Текст: зеленый   |<><od>| <<od>od>    | \\033[33;45m | Yellow text      | Текст: желтый    |<><oe>| <<oe>oe>    | \\033[34;45m | Blue text        | Текст: синий     |<><of>| <<of>of>    | \\033[35;45m | Magenta text     | Текст: пурпурный |<><og>| <<og>og>    | \\033[36;45m | Cyan text        | Текст: голубой   |<><oh>| <<oh>oh>    | \\033[37;45m | White text       | Текст: белый     |<><oi>| <<oi>oi>    | \\033[37;45m | Gray text        | Текст: серый     |<></>+~~~~~~~~~+~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+"},
        .detail_lines = 15
    };
    items[15] = (MenuItem){
        .description = L"<p>| <<p>p> | \\033[46m | Cyan background    | Фон: голубой     |",
        .details = {L"<p>| <<p>p> | \\033[46m | Cyan background | Фон: голубой |<></p> <</p>/p> закрывающий тег<>+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+<>|                      * Двойные теги *                       |<>+---------+-------------+------------------+------------------+<><pa>| <<pa>pa>    | \\033[30;46m | Black text       | Текст: черный    |<><pb>| <<pb>pb>    | \\033[31;46m | Red text         | Текст: красный   |<><pc>| <<pc>pc>    | \\033[32;46m | Green text       | Текст: зеленый   |<><pd>| <<pd>pd>    | \\033[33;46m | Yellow text      | Текст: желтый    |<><pe>| <<pe>pe>    | \\033[34;46m | Blue text        | Текст: синий     |<><pf>| <<pf>pf>    | \\033[35;46m | Magenta text     | Текст: пурпурный |<><pg>| <<pg>pg>    | \\033[36;46m | Cyan text        | Текст: голубой   |<><ph>| <<ph>ph>    | \\033[37;46m | White text       | Текст: белый     |<><pi>| <<pi>pi>    | \\033[37;46m | Gray text        | Текст: серый     |<></>+~~~~~~~~~+~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+"},
        .detail_lines = 15
    };
    items[16] = (MenuItem){
        .description = L"<q>| <<q>q> | \\033[47m | White background   | Фон: белый       |",
        .details = {L"<q>| <<q>q> | \\033[47m | White background | Фон: белый       |<></q> <</q>/q> закрывающий тег<>+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+<>|                      * Двойные теги *                       |<>+---------+-------------+------------------+------------------+<><qa>| <<qa>qa>    | \\033[30;47m | Black text       | Текст: черный    |<><qb>| <<qb>qb>    | \\033[31;47m | Red text         | Текст: красный   |<><qc>| <<qc>qc>    | \\033[32;47m | Green text       | Текст: зеленый   |<><qd>| <<qd>qd>    | \\033[33;47m | Yellow text      | Текст: желтый    |<><qe>| <<qe>qe>    | \\033[34;47m | Blue text        | Текст: синий     |<><qf>| <<qf>qf>    | \\033[35;47m | Magenta text     | Текст: пурпурный |<><qg>| <<qg>qg>    | \\033[36;47m | Cyan text        | Текст: голубой   |<><qh>| <<qh>qh>    | \\033[37;47m | White text       | Текст: белый     |<><qi>| <<qi>qi>    | \\033[37;47m | Gray text        | Текст: серый     |<></>+~~~~~~~~~+~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+~~~~~~~~~~~~~~~~~~+"},
        .detail_lines = 15
    };
//  items[17]
    items[17] = (MenuItem){
        .description = L"           * Applying bold formatting to text *           ",
        .details = {NULL},
        .detail_lines = 0
    };
//  items[18]
    items[18] = (MenuItem){
        .description = L"<r>| <<r>r> | \\033[1m  | <r><<r>r> Bold <</r>/r> text | Выделение жирным |",
        .details = {L" <r>| <<r>r> | \\033[1m | Bold text | Выделение жирным |",
                    L"  </r> <</r>/r> закрывающий тег",
                    L" <b>| <<b>b> <r><<r>r> Bold</r> <</r>/r> text |",
                    L" <c>| <<c>c> <r><<r>r> Bold</r> <</r>/r> text |",
                    L" <d>| <<d>d> <r><<r>r> Bold</r> <</r>/r> text |",
                    L" <e>| <<e>e> <r><<r>r> Bold</r> <</r>/r> text |",
                    L" <f>| <<f>f> <r><<r>r> Bold</r> <</r>/r> text |",
                    L" <g>| <<g>g> <r><<r>r> Bold</r> <</r>/r> text |",
                    L" <h>| <<h>h> <r><<r>r> Bold</r> <</r>/r> text |"
},
        .detail_lines = 9
    };
//  items[19]
    items[19] = (MenuItem){
        .description = L"<k>| <<k>k> | \\033[41m | <r><<r>r> Bold</r> <</r>/r> text | Выделение жирным |",
        .details = {L"<k>| <<k>k>  <r><<r>r> Bold</r> <</r>/r> text                           |",
                    L" <ka><r></r></> <</>ka><</>r><</>/r><</>/> <</>/ka> Invalid tag combination",
                    L"<kb>| <<kb>kb> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<kc>| <<kc>kc> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<kd>| <<kd>kd> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<ke>| <<ke>ke> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<kf>| <<kf>kf> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<kg>| <<kg>kg> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<kh>| <<kh>kh> <r><<r>r> Bold</r> <</r>/r> text |",
                    L" <ki><r></r></> <</>ki><</>r><</>/r><</>/> <</>/ki> Invalid tag combination",
},
        .detail_lines = 10
    };
//  items[20]
    items[20] = (MenuItem){
        .description = L"<l>| <<l>l> | \\033[42m | <r><<r>r> Bold</r> <</r>/r> text | Выделение жирным |",
        .details = {L"<l>| <<l>l>  <r><<r>r> Bold</r> <</r>/r> text                           |",
                    L" <la><r></r></> <</>la><</>r><</>/r><</>/> <</>/la> Invalid tag combination",
                    L"<lb>| <<lb>lb> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<lc>| <<lc>lc> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<ld>| <<ld>ld> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<le>| <<le>le> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<lf>| <<lf>lf> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<lg>| <<lg>lg> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<lh>| <<lh>lh> <r><<r>r> Bold</r> <</r>/r> text |",
                    L" <li><r></r></> <</>li><</>r><</>/r><</>/> <</>/li> Invalid tag combination",
},
        .detail_lines = 10
    };
//  items[21]
    items[21] = (MenuItem){
        .description = L"<m>| <<m>m> | \\033[43m | <r><<r>r> Bold</r> <</r>/r> text | Выделение жирным |",
        .details = {L"<m>| <<m>m>  <r><<r>r> Bold</r> <</r>/r> text                           |",
                    L" <ma><r></r></> <</>ma><</>r><</>/r><</>/> <</>/ma> Invalid tag combination",
                    L"<mb>| <<mb>mb> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<mc>| <<mc>mc> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<md>| <<md>md> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<me>| <<me>me> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<mf>| <<mf>mf> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<mg>| <<mg>mg> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<mh>| <<mh>mh> <r><<r>r> Bold</r> <</r>/r> text |",
                    L" <mi><r></r></> <</>mi><</>r><</>/r><</>/> </</>mi> Invalid tag combination",
},
        .detail_lines = 10
    };
//  items[22]
    items[22] = (MenuItem){
        .description = L"<n>| <<n>n> | \\033[44m | <r><<r>r> Bold</r> <</r>/r> text | Выделение жирным |",
        .details = {L"<n>| <<n>n>  <r><<r>r> Bold</r> <</r>/r> text                           |",
                    L" <na><r></r></> <</>na><</>r><</>/r><</>/> <</>/na> Invalid tag combination",
                    L"<nb>| <<nb>nb> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<nc>| <<nc>nc> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<nd>| <<nd>nd> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<ne>| <<ne>ne> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<nf>| <<nf>nf> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<ng>| <<ng>ng> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<nh>| <<nh>nh> <r><<r>r> Bold</r> <</r>/r> text |",
                    L" <ni><r></r></> <</>ni><</>r><</>/r><</>/> <</>/ni> Invalid tag combination",
},
        .detail_lines = 10
    };
//  items[23]
    items[23] = (MenuItem){
        .description = L"<o>| <<o>o> | \\033[45m | <r><<r>r> Bold</r> <</r>/r> text | Выделение жирным |",
        .details = {L"<o>| <<o>o>  <r><<r>r> Bold</r> <</r>/r> text                           |",
                    L" <oa><r></r></> <</>oa><</>r><</>/r><</>/> <</>/oa> Invalid tag combination",
                    L"<ob>| <<ob>ob> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<oc>| <<oc>oc> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<od>| <<od>od> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<oe>| <<oe>oe> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<of>| <<of>of> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<og>| <<og>og> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<oh>| <<oh>oh> <r><<r>r> Bold</r> <</r>/r> text |",
                    L" <oi><r></r></> <</>oi><</>r><</>/r><</>/> <</>/oi> Invalid tag combination",
},
        .detail_lines = 10
    };
//  items[24]
    items[24] = (MenuItem){
        .description = L"<p>| <<p>p> | \\033[45m | <r><<r>r> Bold</r> <</r>/r> text | Выделение жирным |",
        .details = {L"<p>| <<p>p>  <r><<r>r> Bold</r> <</r>/r> text                           |",
                    L" <pa><r></r></> <</>pa><</>r><</>/r><</>/> <</>/pa> Invalid tag combination",
                    L"<pb>| <<pb>pb> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<pc>| <<pc>pc> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<pd>| <<pd>pd> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<pe>| <<pe>pe> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<pf>| <<pf>pf> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<pg>| <<pg>pg> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<ph>| <<ph>ph> <r><<r>r> Bold</r> <</r>/r> text |",
                    L" <pi><r></r></pi></> <</>pi><</>r><</>/r><</>/> <</>/pi> Invalid tag combination",
},
        .detail_lines = 10
    };
//  items[25]
    items[25] = (MenuItem){
        .description = L"<q>| <<q>q> | \\033[45m | <r><<r>r> Bold</r> <</r>/r> text | Выделение жирным |",
        .details = {L"<q>| <<q>q>  <r><<r>r> Bold</r> <</r>/r> text                           |",
                    L" <qa><r></r></> <</>qa><</>r><</>/r><</>/> <</>/qa> Invalid tag combination",
                    L"<qb>| <<qb>qb> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<qc>| <<qc>qc> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<qd>| <<qd>qd> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<qe>| <<qe>qe> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<qf>| <<qf>qf> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<qg>| <<qg>qg> <r><<r>r> Bold</r> <</r>/r> text |",
                    L"<qh>| <<qh>qh> <r><<r>r> Bold</r> <</r>/r> text |",
                    L" <qi><r></r></> <</>qi><</>r><</>/r><</>/> <</>/qi> Invalid tag combination",
},
        .detail_lines = 10
    };
// items[26]
   items[26] = (MenuItem){
        .description = L"                * Invalid tag combination *               ",
        .details = {NULL},
        .detail_lines = 0
   };
// items[27]
   items[27] = (MenuItem){
        .description = L"| <</>a> | \\033[30m | <</>r>Bold<</r>/r>text<</>/a> | <b><error></b>         |",
        .details = {L" Для корректной работы выделения текста жирным,",
                    L" в последующей строке.",
                    L" Необходимо применить двойное закрытие тега:",
                    L" <</>a>text<</>/a><</>/>",
},
        .detail_lines = 4
    };
// items[28]
   items[28] = (MenuItem){
        .description = L"| <</>i> | \\033[37m | <</>r>Bold<</r>/r>text<</>/i> | <b><error></b>         |",
        .details = {L" Для корректной работы выделения текста жирным,",
                    L" в последующей строке.",
                    L" Необходимо применить двойное закрытие тега:",
                    L" <</>i>text<</>/i><</>/>",
},
        .detail_lines = 4
    };
// end items
}
// === Бегунок ==================================================================================================================================
void print_menu_items(MenuItem items[], int offset, int selected) {
    int start_y = 6;
    // Очищаем область меню (только между рамками)
    for (int i = 0; i < 17; i++) {
        mvprintw(start_y + i, 2, "%*s", 61, ""); // Очистка от x=2 до x=61
    }
    for (int i = 0; i < 17; i++) {
        int item_index = offset + i;
        if (item_index >= 29) break; // if (item_index >= 29) break; (+) Менять при изменении количества пунктов меню. Должно быть равно items[] количеству пунктов меню плюс один.
        wchar_t desc_buf[1025];
        attr_t attributes[1024];
        int attr_count = 0;
        parse_tags(items[item_index].description, desc_buf, 1024, attributes, &attr_count);
        int y = start_y + i;
        int x = 3;
        const wchar_t *ptr = desc_buf;
        int attr_idx = 0;

        if (item_index == selected) {
            attr_t highlight_attr = (wcsstr(items[item_index].description, L"<a>"))
                ? COLOR_PAIR(10) | A_BOLD
                : attributes[0] ? attributes[0] : 0;
            attron(highlight_attr);
            for (int j = 0; j < 60; j++) { // Исчезает неокрашеный пробел.
                wchar_t wch = L'▒';
                cchar_t cch;
                setcchar(&cch, &wch, highlight_attr, 0, NULL);
                mvadd_wch(y, 2 + j, &cch);
            }
            attroff(highlight_attr);
            int in_column = 0;
            int in_star = 0;
            while (*ptr && x < 61 && y < start_y + 17) { // Ограничиваем до x=61
                if (*ptr == L'\n') {
                    y++;
                    x = 3;
                    ptr++;
                    attr_idx++;
                    continue;
                }
                attr_t current_attr = (attr_idx < attr_count) ? attributes[attr_idx] : 0;
                if (wcsstr(items[item_index].description, L"<a>")) {
                    current_attr = COLOR_PAIR(10) | A_BOLD;
                }
                wchar_t wch = *ptr;
                if (wch == L'|') {
                    in_column = !in_column;
                } else if (wch == L'*') {
                    in_star = !in_star;
                } else if ((in_column || in_star) && wch == L' ') {
                    wch = L'▒';
                }
                if (!in_column && !in_star && wch == L' ' && x < 61) { // ------ ?
                    wch = L'▒';
                }
                if (current_attr != 0) {
                    attron(current_attr | A_BOLD);
                }
                cchar_t cch;
                setcchar(&cch, &wch, current_attr | A_BOLD, 0, NULL);
                mvadd_wch(y, x, &cch);
                if (current_attr != 0) {
                    attroff(current_attr | A_BOLD);
                }
                ptr++;
                x++;
                if (attr_idx < attr_count) attr_idx++;
            }
            // Заполняем оставшиеся пробелы до конца строки
            while (x < 61) { // Исчес неокрашеный пробел.
                wchar_t wch = L'▒';
                cchar_t cch;
                setcchar(&cch, &wch, highlight_attr, 0, NULL);
                mvadd_wch(y, x, &cch);
                x++;
            }
        } else {
            while (*ptr && x < 61 && y < start_y + 17) {
                if (*ptr == L'\n') {
                    y++;
                    x = 3;
                    ptr++;
                    attr_idx++;
                    continue;
                }
                attr_t current_attr = (attr_idx < attr_count) ? attributes[attr_idx] : 0;
                if (current_attr != 0) attron(current_attr);
                wchar_t wch = *ptr;
                cchar_t cch;
                setcchar(&cch, &wch, current_attr, 0, NULL);
                mvadd_wch(y, x, &cch);
                if (current_attr != 0) attroff(current_attr);
                ptr++;
                x++;
                if (attr_idx < attr_count) attr_idx++;
            }
        }
        wredrawln(stdscr, y, 1);
    }
    wnoutrefresh(stdscr);
    doupdate();
}
//----------------------------------------------------------------------------------------------------------------------------
void handle_input(int ch, int *selected, int *offset, MenuItem items[]) {
    switch (ch) {
        case KEY_UP:
            (*selected)--;
            if (*selected < 0) *selected = 28; // Менять при изменении количества пунктов меню. Должно быть равно items[] количеству пунктов меню.
            *offset = (*selected < *offset) ? *selected : *offset;
            *offset = (*selected >= *offset + 17) ? *selected - 16 : *offset; // этот параметр должен быть -1 для плавной работы.
            print_menu_items(items, *offset, *selected);
            break;
        case KEY_DOWN:
            (*selected)++;
            if (*selected >= 29) *selected = 0; // Менять при изменении количества пунктов меню. Должно быть равно items[] количеству пунктов меню плюс один.
            *offset = (*selected < *offset) ? *selected : *offset;
            *offset = (*selected >= *offset + 17) ? *selected - 16 : *offset; // этот параметр должен быть -1 для плавной работы.
            print_menu_items(items, *offset, *selected);
            break;
        case 10:
            if (*selected != 17 && *selected != 26) { // Менять при изменении количества пунктов меню. Обработчик блокироваки энтер.
                clear();
                refresh();
                int y = 1;
                for (int i = 0; i < items[*selected].detail_lines && i < 30; i++) { // < 31 Не помогает.
                    wchar_t detail_buf[1025];
                    attr_t attributes[1024];
                    int attr_count = 0;
                    parse_tags(items[*selected].details[i] ? items[*selected].details[i] : L"", detail_buf, 1024, attributes, &attr_count);
                    print_tagged_text(detail_buf, 0, y++, attributes, attr_count, 64);
                }
                attron(COLOR_PAIR(4));
                mvprintw(y + 1, 0, " ENTER = Main Menu ");
                attroff(COLOR_PAIR(4));
                refresh();
                int ch;
                int start_line = 0;
                int max_y = 30; // 30
                int total_lines = items[*selected].detail_lines;
                while ((ch = getch()) != 10) {
                    switch (ch) {
                        case 'j':
                            if (start_line + max_y < total_lines)
                                ++start_line;
                            break;
                        case 'k':
                            if (start_line > 0)
                                --start_line;
                            break;
                    }
                }
               draw_menu();
               print_menu_items(items, *offset, *selected);
            }
            break;
        case 'q':
//        case 'Q':
            exit_program();
            break;
    }
}
//---
int main(void) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    if (!setlocale(LC_ALL, "")) {
        printf("\033[1;31mError:\033[0m \033[31mFailed to set locale\n\033[0m");
        return 1;
    }
    initscr();
    if (has_colors() == FALSE) {
        endwin();
        printf("\033[1;31mError:\033[0m \033[31mTerminal does not support colors\n\033[0m");
        return 1;
    }
    use_default_colors();
    if (LINES < 27 || COLS < 68) {
    endwin();
    printf("\033[1;31mError:\033[0m \033[31mTerminal is too small. Requires at least %dx%d.\n\033[0m", 68, 27);
    return 1;
}
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    signal(SIGINT, cleanup);
    init_tag_colors();
    MenuItem items[22];
    init_menu_items(items);
    int selected = 0;
    int offset = 0;
    draw_menu();
    print_menu_items(items, offset, selected);
    while (1) {
        int ch = getch();
        if (ch == 'q' || ch == 'Q') exit_program();
        handle_input(ch, &selected, &offset, items);
    }
    return 0;
}
