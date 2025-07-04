#define _XOPEN_SOURCE_EXTENDED 1
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include <ncursesw/ncurses.h>
#include "tags_mu_ncurses.h"

// Макрос для серого цвета (использует COLOR_WHITE с A_DIM)
#define COLOR_GRAY COLOR_WHITE

// Массив соответствий тегов и атрибутов
const struct TagMapping tag_mappings[] = {
    { TAG_NEWLINE, 0, 0 },                  // Перенос строки
    { TAG_RESET, 0, 1 },                    // Сброс всех атрибутов
    { TAG_BLACK, COLOR_PAIR(6), 0 },        // Черный текст
    { TAG_RED, COLOR_PAIR(3), 0 },          // Красный текст
    { TAG_GREEN, COLOR_PAIR(4), 0 },        // Зеленый текст
    { TAG_YELLOW, COLOR_PAIR(5), 0 },       // Желтый текст
    { TAG_BLUE, COLOR_PAIR(7), 0 },         // Синий текст
    { TAG_MAGENTA, COLOR_PAIR(8), 0 },      // Пурпурный текст
    { TAG_CYAN, COLOR_PAIR(9), 0 },         // Голубой текст
    { TAG_WHITE, COLOR_PAIR(10), 0 },       // Белый текст
    { TAG_GRAY, COLOR_PAIR(11), 0 },        // Серый текст
    { TAG_BG_BLACK, COLOR_PAIR(12), 0 },    // Черный фон
    { TAG_BG_RED, COLOR_PAIR(13), 0 },      // Красный фон
    { TAG_BG_GREEN, COLOR_PAIR(14), 0 },    // Зеленый фон
    { TAG_BG_YELLOW, COLOR_PAIR(15), 0 },   // Желтый фон
    { TAG_BG_BLUE, COLOR_PAIR(16), 0 },     // Синий фон
    { TAG_BG_MAGENTA, COLOR_PAIR(17), 0 },  // Пурпурный фон
    { TAG_BG_CYAN, COLOR_PAIR(18), 0 },     // Голубой фон
    { TAG_BG_WHITE, COLOR_PAIR(19), 0 },    // Белый фон
    { TAG_BOLD, A_BOLD, 0 },               // Жирный текст
    { TAG_QA, COLOR_PAIR(21), 0 },         // Белый фон, черный текст
    { TAG_QB, COLOR_PAIR(22), 0 },         // Белый фон, красный текст
    { TAG_QC, COLOR_PAIR(23), 0 },         // Белый фон, зеленый текст
    { TAG_QD, COLOR_PAIR(24), 0 },         // Белый фон, желтый текст
    { TAG_QE, COLOR_PAIR(25), 0 },         // Белый фон, синий текст
    { TAG_QF, COLOR_PAIR(26), 0 },         // Белый фон, пурпурный текст
    { TAG_QG, COLOR_PAIR(27), 0 },         // Белый фон, голубой текст
    { TAG_QH, COLOR_PAIR(28), 0 },         // Белый фон, белый текст
    { TAG_QI, COLOR_PAIR(29), 0 },         // Белый фон, серый текст
    { TAG_JA, COLOR_PAIR(30), 0 },         // Черный текст на черном фоне
    { TAG_JB, COLOR_PAIR(31), 0 },         // Красный текст на черном фоне
    { TAG_JC, COLOR_PAIR(32), 0 },         // Зеленый текст на черном фоне
    { TAG_JD, COLOR_PAIR(33), 0 },         // Желтый текст на черном фоне
    { TAG_JE, COLOR_PAIR(34), 0 },         // Синий текст на черном фоне
    { TAG_JF, COLOR_PAIR(35), 0 },         // Пурпурный текст на черном фоне
    { TAG_JG, COLOR_PAIR(36), 0 },         // Голубой текст на черном фоне
    { TAG_JH, COLOR_PAIR(37), 0 },         // Белый текст на черном фоне
    { TAG_JI, COLOR_PAIR(38), 0 },         // Серый текст на черном фоне
    { TAG_KA, COLOR_PAIR(39), 0 },         // Черный текст на красном фоне
    { TAG_KB, COLOR_PAIR(40), 0 },         // Красный текст на красном фоне
    { TAG_KC, COLOR_PAIR(41), 0 },         // Зеленый текст на красном фоне
    { TAG_KD, COLOR_PAIR(42), 0 },         // Желтый текст на красном фоне
    { TAG_KE, COLOR_PAIR(43), 0 },         // Синий текст на красном фоне
    { TAG_KF, COLOR_PAIR(44), 0 },         // Пурпурный текст на красном фоне
    { TAG_KG, COLOR_PAIR(45), 0 },         // Голубой текст на красном фоне
    { TAG_KH, COLOR_PAIR(46), 0 },         // Белый текст на красном фоне
    { TAG_KI, COLOR_PAIR(47), 0 },         // Серый текст на красном фоне
    { TAG_LA, COLOR_PAIR(48), 0 },         // Черный текст на зелёном фоне
    { TAG_LB, COLOR_PAIR(49), 0 },         // Красный текст на зелёном фоне
    { TAG_LC, COLOR_PAIR(50), 0 },         // Зелёный текст на зелёном фоне
    { TAG_LD, COLOR_PAIR(51), 0 },         // Жёлтый текст на зелёном фоне
    { TAG_LE, COLOR_PAIR(52), 0 },         // Синий текст на зелёном фоне
    { TAG_LF, COLOR_PAIR(53), 0 },         // Пурпурный текст на зелёном фоне
    { TAG_LG, COLOR_PAIR(54), 0 },         // Голубой текст на зелёном фоне
    { TAG_LH, COLOR_PAIR(55), 0 },         // Белый текст на зелёном фоне
    { TAG_LI, COLOR_PAIR(56), 0 },         // Серый текст на зелёном фоне
    { TAG_MA, COLOR_PAIR(57), 0 },         // Черный текст на жёлтом фоне
    { TAG_MB, COLOR_PAIR(58), 0 },         // Красный текст на жёлтом фоне
    { TAG_MC, COLOR_PAIR(59), 0 },         // Зелёный текст на жёлтом фоне
    { TAG_MD, COLOR_PAIR(60), 0 },         // Жёлтый текст на жёлтом фоне
    { TAG_ME, COLOR_PAIR(61), 0 },         // Синий текст на жёлтом фоне
    { TAG_MF, COLOR_PAIR(62), 0 },         // Пурпурный текст на жёлтом фоне
    { TAG_MG, COLOR_PAIR(63), 0 },         // Голубой текст на жёлтом фоне
    { TAG_MH, COLOR_PAIR(64), 0 },         // Белый текст на жёлтом фоне
    { TAG_MI, COLOR_PAIR(65), 0 },         // Серый текст на жёлтом фоне
    { TAG_NA, COLOR_PAIR(67), 0 },         // Черный текст на синем фоне
    { TAG_NB, COLOR_PAIR(68), 0 },         // Красный текст на синем фоне
    { TAG_NC, COLOR_PAIR(69), 0 },         // Зелёный текст на синем фоне
    { TAG_ND, COLOR_PAIR(70), 0 },         // Жёлтый текст на синем фоне
    { TAG_NE, COLOR_PAIR(71), 0 },         // Синий текст на синем фоне
    { TAG_NF, COLOR_PAIR(72), 0 },         // Пурпурный текст на синем фоне
    { TAG_NG, COLOR_PAIR(73), 0 },         // Голубой текст на синем фоне
    { TAG_NH, COLOR_PAIR(74), 0 },         // Белый текст на синем фоне
    { TAG_NI, COLOR_PAIR(75), 0 },         // Серый текст на синем фоне
    { TAG_OA, COLOR_PAIR(76), 0 },         // Черный текст на пурпурном фоне
    { TAG_OB, COLOR_PAIR(77), 0 },         // Красный текст на пурпурном фоне
    { TAG_OC, COLOR_PAIR(78), 0 },         // Зелёный текст на пурпурном фоне
    { TAG_OD, COLOR_PAIR(79), 0 },         // Жёлтый текст на пурпурном фоне
    { TAG_OE, COLOR_PAIR(80), 0 },         // Синий текст на пурпурном фоне
    { TAG_OF, COLOR_PAIR(81), 0 },         // Пурпурный текст на пурпурном фоне
    { TAG_OG, COLOR_PAIR(82), 0 },         // Голубой текст на пурпурном фоне
    { TAG_OH, COLOR_PAIR(83), 0 },         // Белый текст на пурпурном фоне
    { TAG_OI, COLOR_PAIR(84), 0 },         // Серый текст на пурпурном фоне
    { TAG_PA, COLOR_PAIR(94), 0 },         // Черный текст на голубом фоне
    { TAG_PB, COLOR_PAIR(95), 0 },         // Красный текст на голубом фоне
    { TAG_PC, COLOR_PAIR(96), 0 },         // Зелёный текст на голубом фоне
    { TAG_PD, COLOR_PAIR(97), 0 },         // Жёлтый текст на голубом фоне
    { TAG_PE, COLOR_PAIR(98), 0 },         // Синий текст на голубом фоне
    { TAG_PF, COLOR_PAIR(99), 0 },         // Пурпурный текст на голубом фоне
    { TAG_PG, COLOR_PAIR(100), 0 },        // Голубой текст на голуб phức0
    { TAG_PH, COLOR_PAIR(101), 0 },        // Белый текст на голубом фоне
    { TAG_PI, COLOR_PAIR(102), 0 },        // Серый текст на голубом фоне
    { TAG_RESET_BLACK, COLOR_PAIR(6), 1 },  // Сброс черного текста
    { TAG_RESET_RED, COLOR_PAIR(3), 1 },    // Сброс красного текста
    { TAG_RESET_GREEN, COLOR_PAIR(4), 1 },  // Сброс зеленого текста
    { TAG_RESET_YELLOW, COLOR_PAIR(5), 1 }, // Сброс желтого текста
    { TAG_RESET_BLUE, COLOR_PAIR(7), 1 },   // Сброс синего текста
    { TAG_RESET_MAGENTA, COLOR_PAIR(8), 1 },// Сброс пурпурного текста
    { TAG_RESET_CYAN, COLOR_PAIR(9), 1 },   // Сброс голубого текста
    { TAG_RESET_WHITE, COLOR_PAIR(10), 1 }, // Сброс белого текста
    { TAG_RESET_GRAY, COLOR_PAIR(11), 1 },  // Сброс серого текста
    { TAG_RESET_BG_BLACK, COLOR_PAIR(12), 1 }, // Сброс черного фона
    { TAG_RESET_BG_RED, COLOR_PAIR(13), 1 },   // Сброс красного фона
    { TAG_RESET_BG_GREEN, COLOR_PAIR(14), 1 }, // Сброс зеленого фона
    { TAG_RESET_BG_YELLOW, COLOR_PAIR(15), 1 },// Сброс желтого фона
    { TAG_RESET_BG_BLUE, COLOR_PAIR(16), 1 },  // Сброс синего фона
    { TAG_RESET_BG_MAGENTA, COLOR_PAIR(17), 1 },// Сброс пурпурного фона
    { TAG_RESET_BG_CYAN, COLOR_PAIR(18), 1 },  // Сброс голубого фона
    { TAG_RESET_BG_WHITE, COLOR_PAIR(19), 1 }, // Сброс белого фона
    { TAG_RESET_BOLD, A_BOLD, 1 },            // Сброс жирного текста
    { NULL, 0, 0 }                            // Конец массива
};

// Инициализация цветовых пар для всех тегов
void init_tag_colors(void)
{
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // Желтая рамка
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);  // Желтая рамка (для меню)
    init_pair(3, COLOR_RED, -1); // Красный текст (<b>)
    init_pair(4, COLOR_GREEN, -1);   // Зеленый текст (<c>)
    init_pair(5, COLOR_YELLOW, -1);  // Желтый текст (<d>)
    init_pair(6, COLOR_BLACK, -1);   // Черный текст (<a>)
    init_pair(7, COLOR_BLUE, -1);    // Синий текст (<e>)
    init_pair(8, COLOR_MAGENTA, -1); // Пурпурный текст (<f>)
    init_pair(9, COLOR_CYAN, -1);    // Голубой текст (<g>)
    init_pair(10, COLOR_WHITE, -1);  // Белый текст (<h>)
    init_pair(11, COLOR_GRAY, -1);   // Серый текст (<i>, используем COLOR_GRAY с A_DIM)
    init_pair(12, COLOR_WHITE, COLOR_BLACK);  // Черный фон (<j>)
    init_pair(13, COLOR_WHITE, COLOR_RED);    // Красный фон (<k>)
    init_pair(14, COLOR_WHITE, COLOR_GREEN);  // Зеленый фон (<l>)
    init_pair(15, COLOR_WHITE, COLOR_YELLOW); // Желтый фон (<m>)
    init_pair(16, COLOR_WHITE, COLOR_BLUE);   // Синий фон (<n>)
    init_pair(17, COLOR_WHITE, COLOR_MAGENTA);// Пурпурный фон (<o>)
    init_pair(18, COLOR_WHITE, COLOR_CYAN);   // Голубой фон (<p>)
    init_pair(19, COLOR_WHITE, COLOR_WHITE);  // Белый фон (<q>)
    init_pair(21, COLOR_BLACK, COLOR_WHITE);  // Черный текст, белый фон (<qa>)
    init_pair(22, COLOR_RED, COLOR_WHITE);    // Белый фон, красный текст (<qb>)
    init_pair(23, COLOR_GREEN, COLOR_WHITE);  // Белый фон, зеленый текст (<qc>)
    init_pair(24, COLOR_YELLOW, COLOR_WHITE); // Белый фон, желтый текст (<qd>)
    init_pair(25, COLOR_BLUE, COLOR_WHITE);   // Белый фон, синий текст (<qe>)
    init_pair(26, COLOR_MAGENTA, COLOR_WHITE);// Белый фон, пурпурный текст (<qf>)
    init_pair(27, COLOR_CYAN, COLOR_WHITE);   // Белый фон, голубой текст (<qg>)
    init_pair(28, COLOR_WHITE, COLOR_WHITE);  // Белый фон, белый текст (<qh>)
    init_pair(29, COLOR_GRAY, COLOR_WHITE);   // Белый фон, серый текст (<qi>)
    init_pair(30, COLOR_BLACK, COLOR_BLACK);   // Черный текст на черном фоне (<ja>)
    init_pair(31, COLOR_RED, COLOR_BLACK);     // Красный текст на черном фоне (<jb>)
    init_pair(32, COLOR_GREEN, COLOR_BLACK);   // Зеленый текст на черном фоне (<jc>)
    init_pair(33, COLOR_YELLOW, COLOR_BLACK);  // Желтый текст на черном фоне (<jd>)
    init_pair(34, COLOR_BLUE, COLOR_BLACK);    // Синий текст на черном фоне (<je>)
    init_pair(35, COLOR_MAGENTA, COLOR_BLACK); // Пурпурный текст на черном фоне (<jf>)
    init_pair(36, COLOR_CYAN, COLOR_BLACK);    // Голубой текст на черном фоне (<jg>)
    init_pair(37, COLOR_WHITE, COLOR_BLACK);   // Белый текст на черном фоне (<jh>)
    init_pair(38, COLOR_GRAY, COLOR_BLACK);    // Серый текст на черном фоне (<ji>)
    init_pair(39, COLOR_BLACK, COLOR_RED);    // Черный текст на красном фоне (<ka>)
    init_pair(40, COLOR_RED, COLOR_RED);      // Красный текст на красном фоне (<kb>)
    init_pair(41, COLOR_GREEN, COLOR_RED);    // Зеленый текст на красном фоне (<kc>)
    init_pair(42, COLOR_YELLOW, COLOR_RED);   // Желтый текст на красном фоне (<kd>)
    init_pair(43, COLOR_BLUE, COLOR_RED);     // Синий текст на красном фоне (<ke>)
    init_pair(44, COLOR_MAGENTA, COLOR_RED);  // Пурпурный текст на красном фоне (<kf>)
    init_pair(45, COLOR_CYAN, COLOR_RED);     // Голубой текст на красном фоне (<kg>)
    init_pair(46, COLOR_WHITE, COLOR_RED);    // Белый текст на красном фоне (<kh>)
    init_pair(47, COLOR_GRAY, COLOR_RED);     // Серый текст на красном фоне (<ki>)
    init_pair(48, COLOR_BLACK, COLOR_GREEN);    // Черный текст на зелёном фоне (<la>)
    init_pair(49, COLOR_RED, COLOR_GREEN);      // Красный текст на зелёном фоне (<lb>)
    init_pair(50, COLOR_GREEN, COLOR_GREEN);    // Зелёный текст на зелёном фоне (<lc>)
    init_pair(51, COLOR_YELLOW, COLOR_GREEN);   // Жёлтый текст на зелёном фоне (<ld>)
    init_pair(52, COLOR_BLUE, COLOR_GREEN);     // Синий текст на зелёном фоне (<le>)
    init_pair(53, COLOR_MAGENTA, COLOR_GREEN);  // Пурпурный текст на зелёном фоне (<lf>)
    init_pair(54, COLOR_CYAN, COLOR_GREEN);     // Голубой текст на зелёном фоне (<lg>)
    init_pair(55, COLOR_WHITE, COLOR_GREEN);    // Белый текст на зелёном фоне (<lh>)
    init_pair(56, COLOR_GRAY, COLOR_GREEN);     // Серый текст на зелёном фоне (<li>)
    init_pair(57, COLOR_BLACK, COLOR_YELLOW);    // Черный текст на жёлтом фоне (<ma>)
    init_pair(58, COLOR_RED, COLOR_YELLOW);      // Красный текст на жёлтом фоне (<mb>)
    init_pair(59, COLOR_GREEN, COLOR_YELLOW);    // Зелёный текст на жёлтом фоне (<mc>)
    init_pair(60, COLOR_YELLOW, COLOR_YELLOW);   // Жёлтый текст на жёлтом фоне (<md>)
    init_pair(61, COLOR_BLUE, COLOR_YELLOW);     // Синий текст на жёлтом фоне (<me>)
    init_pair(62, COLOR_MAGENTA, COLOR_YELLOW);  // Пурпурный текст на жёлтом фоне (<mf>)
    init_pair(63, COLOR_CYAN, COLOR_YELLOW);     // Голубой текст на жёлтом фоне (<mg>)
    init_pair(64, COLOR_WHITE, COLOR_YELLOW);    // Белый текст на жёлтом фоне (<mh>)
    init_pair(65, COLOR_GRAY, COLOR_YELLOW);     // Серый текст на жёлтом фоне (<mi>)
    init_pair(67, COLOR_BLACK, COLOR_BLUE);      // Черный текст на синем фоне (<na>)
    init_pair(68, COLOR_RED, COLOR_BLUE);        // Красный текст на синем фоне (<nb>)
    init_pair(69, COLOR_GREEN, COLOR_BLUE);      // Зелёный текст на синем фоне (<nc>)
    init_pair(70, COLOR_YELLOW, COLOR_BLUE);     // Жёлтый текст на синем фоне (<nd>)
    init_pair(71, COLOR_BLUE, COLOR_BLUE);       // Синий текст на синем фоне (<ne>)
    init_pair(72, COLOR_MAGENTA, COLOR_BLUE);    // Пурпурный текст на синем фоне (<nf>)
    init_pair(73, COLOR_CYAN, COLOR_BLUE);       // Голубой текст на синем фоне (<ng>)
    init_pair(74, COLOR_WHITE, COLOR_BLUE);      // Белый текст на синем фоне (<nh>)
    init_pair(75, COLOR_GRAY, COLOR_BLUE);       // Серый текст на синем фоне (<ni>)
    init_pair(76, COLOR_BLACK, COLOR_MAGENTA);    // Черный текст на пурпурном фоне (<oa>)
    init_pair(77, COLOR_RED, COLOR_MAGENTA);      // Красный текст на пурпурном фоне (<ob>)
    init_pair(78, COLOR_GREEN, COLOR_MAGENTA);    // Зелёный текст на пурпурном фоне (<oc>)
    init_pair(79, COLOR_YELLOW, COLOR_MAGENTA);   // Жёлтый текст на пурпурном фоне (<od>)
    init_pair(80, COLOR_BLUE, COLOR_MAGENTA);     // Синий текст на пурпурном фоне (<oe>)
    init_pair(81, COLOR_MAGENTA, COLOR_MAGENTA);  // Пурпурный текст на пурпурном фоне (<of>)
    init_pair(82, COLOR_CYAN, COLOR_MAGENTA);     // Голубой текст на пурпурном фоне (<og>)
    init_pair(83, COLOR_WHITE, COLOR_MAGENTA);    // Белый текст на пурпурном фоне (<oh>)
    init_pair(84, COLOR_GRAY, COLOR_MAGENTA);     // Серый текст на пурпурном фоне (<oi>)
    init_pair(94, COLOR_BLACK, COLOR_CYAN);       // Черный текст на голубом фоне (<pa>)
    init_pair(95, COLOR_RED, COLOR_CYAN);         // Красный текст на голубом фоне (<pb>)
    init_pair(96, COLOR_GREEN, COLOR_CYAN);       // Зелёный текст на голубом фоне (<pc>)
    init_pair(97, COLOR_YELLOW, COLOR_CYAN);      // Жёлтый текст на голубом фоне (<pd>)
    init_pair(98, COLOR_BLUE, COLOR_CYAN);        // Синий текст на голубом фоне (<pe>)
    init_pair(99, COLOR_MAGENTA, COLOR_CYAN);     // Пурпурный текст на голубом фоне (<pf>)
    init_pair(100, COLOR_CYAN, COLOR_CYAN);       // Голубой текст на голубом фоне (<pg>)
    init_pair(101, COLOR_WHITE, COLOR_CYAN);      // Белый текст на голубом фоне (<ph>)
    init_pair(102, COLOR_GRAY, COLOR_CYAN);       // Серый текст на голубом фоне (<pi>)
}

// Объединённая функция разбора тегов и обработки символов
void parse_tags(const wchar_t *input, wchar_t *output, int max_output, attr_t *attributes, int *attr_count)
{
    const wchar_t *forbidden_substrings[] = {
        L"<s>",
        L"<t>",
        L"<u>",
        L"<v>",
        L"<w>",
        L"<x>",
        L"<y>",
        L"<z>",
        L"</s>",
        L"</t>",
        L"</u>",
        L"</v>",
        L"</w>",
        L"</x>",
        L"</y>",
        L"</z>",
        L"</ka>",
        L"</ki>",
        L"</la>",
        L"</li>",
        L"</ma>",
        L"</mi>",
        L"</na>",
        L"</ni>",
        L"</oa>",
        L"</oi>",
        L"</pa>",
        L"</pi>",
        L"</qa>",
        L"</qi>",
        NULL
    };
    const wchar_t *replacement = L"<error>";
    const wchar_t *error_msg = L"<error>";
    const wchar_t *ptr = input;
    int out_idx = 0;
    int attr_idx = 0;
    attr_t current_attr = 0;
    output[0] = L'\0';
    int in_black = 0;
    int in_gray = 0;
    int in_hash_tag = 0;
    int in_a_tag = 0;
    int in_restricted_tag = 0; // Добавлено

    while (*ptr && out_idx < max_output - 1 && attr_idx < max_output - 1) {
        // Проверка на хэш-тег <#>
        if (wcsncmp(ptr, L"<#>", 3) == 0 && !in_a_tag) {
            in_hash_tag = 1;
            ptr += 3;
            // Пропускаем содержимое до </#>
            while (*ptr && wcsncmp(ptr, L"</#>", 4) != 0) {
                ptr++;
            }
            if (wcsncmp(ptr, L"</#>", 4) == 0) {
                ptr += 4;
            }
            in_hash_tag = 0;
            continue;
        }

        // Проверка на запрещённые подстроки
        int found_forbidden = 0;
        for (int i = 0; forbidden_substrings[i]; i++) {
            size_t substr_len = wcslen(forbidden_substrings[i]);
            if (wcsncmp(ptr, forbidden_substrings[i], substr_len) == 0) {
                // Добавляем замену (@)
                for (size_t j = 0; replacement[j] && out_idx < max_output - 1; j++) {
                    output[out_idx] = replacement[j];
                    attributes[attr_idx] = COLOR_PAIR(3); // Красный цвет для замены
                    out_idx++;
                    attr_idx++;
                }
                ptr += substr_len;
                found_forbidden = 1;
                break;
            }
        }
        if (found_forbidden) continue;

// Проверка на невалидные теги для обработки вложенных <r> </r> -----------------------------------------------------------------------------
const wchar_t *open_tags[]  = {
 L"<a>",  L"<i>",  L"<ka>", L"<ki>", L"<la>", L"<li>", L"<ma>", L"<mi>", L"<na>", L"<ni>", L"<oa>", L"<oi>", L"<pa>", L"<pi>", L"<qa>", L"<qi>", NULL
 };
const wchar_t *close_tags[] = {
 L"</a>", L"</i>", L"</>",  L"</>",  L"</>",  L"</>",  L"</>",  L"</>",  L"</>",  L"</>",  L"</>",  L"</>",  L"</>",  L"</>",  L"</>",  L"</>",  NULL
 };

for (int i = 0; open_tags[i]; i++) {
    size_t tag_len = wcslen(open_tags[i]);
    if (wcsncmp(ptr, open_tags[i], tag_len) == 0) {
        in_restricted_tag = 1;
        const wchar_t *start_i = ptr;
        ptr += tag_len;
        const wchar_t *content_start = ptr;
        while (*ptr && wcsncmp(ptr, close_tags[i], wcslen(close_tags[i])) != 0) {
            if (wcsncmp(ptr, L"<r>", 3) == 0 || wcsncmp(ptr, L"</r>", 4) == 0) {
                for (size_t j = 0; error_msg[j] && out_idx < max_output - 1; j++) {
                    output[out_idx] = error_msg[j];
                    attributes[attr_idx] = COLOR_PAIR(3);
                    out_idx++;
                    attr_idx++;
                }
                ptr = content_start;
                while (*ptr && wcsncmp(ptr, close_tags[i], wcslen(close_tags[i])) != 0) ptr++;
                if (*ptr) ptr += wcslen(close_tags[i]);
                in_restricted_tag = 0;
                goto continue_loop;
            }
            ptr++;
        }
        if (wcsncmp(ptr, L"</r>", 4) == 0) {
            ptr += 4;
            in_restricted_tag = 0;
        } else {
            ptr = start_i;
        }
        break;
    }
}
//---
        // Проверка на теги из tag_mappings
        int found_tag = 0;
        for (int j = 0; tag_mappings[j].tag != NULL; j++) {
            size_t tag_len = wcslen(tag_mappings[j].tag);
            if (wcsncmp(ptr, tag_mappings[j].tag, tag_len) == 0) {
                if (wcscmp(tag_mappings[j].tag, TAG_BLACK) == 0) {
                    in_black = 1;
                    current_attr = (current_attr & A_BOLD) | tag_mappings[j].attribute;
                } else if (wcscmp(tag_mappings[j].tag, TAG_GRAY) == 0) {
                    in_gray = 1;
                    current_attr = (current_attr & A_BOLD) | tag_mappings[j].attribute | A_DIM;
                } else if (wcscmp(tag_mappings[j].tag, TAG_RESET_BLACK) == 0) {
                    in_black = 0;
                    current_attr &= ~tag_mappings[j].attribute;
                } else if (wcscmp(tag_mappings[j].tag, TAG_RESET_GRAY) == 0) {
                    in_gray = 0;
                    current_attr &= ~(A_DIM | COLOR_PAIR(11));
// Делает текст серым на фоне.
} else if (wcscmp(tag_mappings[j].tag, TAG_JI) == 0 ||
           wcscmp(tag_mappings[j].tag, TAG_KI) == 0 ||
           wcscmp(tag_mappings[j].tag, TAG_LI) == 0 ||
           wcscmp(tag_mappings[j].tag, TAG_MI) == 0 ||
           wcscmp(tag_mappings[j].tag, TAG_NI) == 0 ||
           wcscmp(tag_mappings[j].tag, TAG_OI) == 0 ||
           wcscmp(tag_mappings[j].tag, TAG_PI) == 0 ||
           wcscmp(tag_mappings[j].tag, TAG_QI) == 0) {
    current_attr = (current_attr & A_BOLD) | tag_mappings[j].attribute | A_DIM;
//
                } else if (wcscmp(tag_mappings[j].tag, TAG_BOLD) == 0) {
                    if (!in_black && !in_gray && !in_restricted_tag) {
                        current_attr |= A_BOLD;
                    }
                } else if (wcscmp(tag_mappings[j].tag, TAG_NEWLINE) == 0) {
                    output[out_idx] = L'\n';
                    attributes[attr_idx] = current_attr;
                    out_idx++;
                    attr_idx++;
                } else if (wcscmp(tag_mappings[j].tag, TAG_RESET) == 0) {
                    current_attr = 0;
                    in_black = 0;
                    in_gray = 0;
                    in_a_tag = 0;
                } else if (tag_mappings[j].reset) {
                    if (wcscmp(tag_mappings[j].tag, TAG_RESET_BOLD) == 0) {
                        current_attr &= ~A_BOLD;
                    } else {
                        current_attr &= ~tag_mappings[j].attribute;
                    }
                } else {
                    current_attr = (current_attr & A_BOLD) | tag_mappings[j].attribute;
                }
                ptr += tag_len;
                found_tag = 1;
                break;
            }
        }

        if (!found_tag && !in_hash_tag && !in_a_tag) {
            output[out_idx] = *ptr;
            attributes[attr_idx] = current_attr;
            out_idx++;
            ptr++;
            attr_idx++;
        } else if (in_hash_tag || in_a_tag) {
            ptr++; // Пропускаем символы внутри хэш-тега или недопустимого тега <a>
        }
continue_loop:;
    }
    output[out_idx] = L'\0';
    *attr_count = attr_idx;
}

// Функция для вывода текста с применением атрибутов
void print_tagged_text(const wchar_t *buf, int x, int y, attr_t *attributes, int attr_count, int max_x)
{
    const wchar_t *ptr = buf;
    int attr_idx = 0;
    int cur_x = x;
    int cur_y = y;
    while (*ptr && cur_x < max_x && cur_y < LINES - 1) {
        if (*ptr == L'\n') {
            cur_y++;
            cur_x = x;
            ptr++;
            attr_idx++;
            continue;
        }
        attr_t current_attr = (attr_idx < attr_count) ? attributes[attr_idx] : 0;
        if (current_attr) attron(current_attr);
        cchar_t cch;
        setcchar(&cch, ptr, current_attr, 0, NULL);
        mvadd_wch(cur_y, cur_x, &cch);
        if (current_attr) attroff(current_attr);
        ptr++;
        cur_x++;
        attr_idx++;
    }
}
