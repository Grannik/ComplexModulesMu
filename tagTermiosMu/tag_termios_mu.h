#ifndef ANSI_TAGS_H
#define ANSI_TAGS_H

#include <wchar.h>

// Определение тегов как строк в алфавитном порядке
#define TAG_NEWLINE L"<>"     // Перенос строки
#define TAG_RESET L"</>"      // Сброс всех атрибутов
#define TAG_BLACK L"<a>"      // Цвет текста: черный
#define TAG_RED L"<b>"        // Цвет текста: красный
#define TAG_GREEN L"<c>"      // Цвет текста: зеленый
#define TAG_YELLOW L"<d>"     // Цвет текста: желтый
#define TAG_BLUE L"<e>"       // Цвет текста: синий
#define TAG_MAGENTA L"<f>"    // Цвет текста: пурпурный
#define TAG_CYAN L"<g>"       // Цвет текста: голубой
#define TAG_WHITE L"<h>"      // Цвет текста: белый
#define TAG_GRAY L"<i>"       // Цвет текста: серый
#define TAG_BG_BLACK L"<j>"   // Цвет фона: черный
#define TAG_BG_RED L"<k>"     // Цвет фона: красный
#define TAG_BG_GREEN L"<l>"   // Цвет фона: зеленый
#define TAG_BG_YELLOW L"<m>"  // Цвет фона: желтый
#define TAG_BG_BLUE L"<n>"    // Цвет фона: синий
#define TAG_BG_MAGENTA L"<o>" // Цвет фона: пурпурный
#define TAG_BG_CYAN L"<p>"    // Цвет фона: голубой
#define TAG_BG_WHITE L"<q>"   // Цвет фона: белый
#define TAG_BOLD L"<r>"       // Выделение жирным
#define TAG_QA L"<qa>"        // Белый фон, черный текст
#define TAG_QB L"<qb>"        // Белый фон, красный текст
#define TAG_QC L"<qc>"        // Белый фон, зеленый текст
#define TAG_QD L"<qd>"        // Белый фон, желтый текст
#define TAG_QE L"<qe>"        // Белый фон, синий текст
#define TAG_QF L"<qf>"        // Белый фон, пурпурный текст
#define TAG_QG L"<qg>"        // Белый фон, голубой текст
#define TAG_QH L"<qh>"        // Белый фон, белый текст
#define TAG_QI L"<qi>"        // Белый фон, серый текст
#define TAG_JA L"<ja>"        // Черный текст на черном фоне
#define TAG_JB L"<jb>"        // Красный текст на черном фоне
#define TAG_JC L"<jc>"        // Зеленый текст на черном фоне
#define TAG_JD L"<jd>"        // Желтый текст на черном фоне
#define TAG_JE L"<je>"        // Синий текст на черном фоне
#define TAG_JF L"<jf>"        // Пурпурный текст на черном фоне
#define TAG_JG L"<jg>"        // Голубой текст на черном фоне
#define TAG_JH L"<jh>"        // Белый текст на черном фоне
#define TAG_JI L"<ji>"        // Серый текст на черном фоне
#define TAG_KA L"<ka>"        // Черный текст на красном фоне
#define TAG_KB L"<kb>"        // Красный текст на красном фоне
#define TAG_KC L"<kc>"        // Зеленый текст на красном фоне
#define TAG_KD L"<kd>"        // Желтый текст на красном фоне
#define TAG_KE L"<ke>"        // Синий текст на красном фоне
#define TAG_KF L"<kf>"        // Пурпурный текст на красном фоне
#define TAG_KG L"<kg>"        // Голубой текст на красном фоне
#define TAG_KH L"<kh>"        // Белый текст на красном фоне
#define TAG_KI L"<ki>"        // Серый текст на красном фоне
#define TAG_LA L"<la>"        // Черный текст на зелёном фоне
#define TAG_LB L"<lb>"        // Красный текст на зелёном фоне
#define TAG_LC L"<lc>"        // Зелёный текст на зелёном фоне
#define TAG_LD L"<ld>"        // Жёлтый текст на зелёном фоне
#define TAG_LE L"<le>"        // Синий текст на зелёном фоне
#define TAG_LF L"<lf>"        // Пурпурный текст на зелёном фоне
#define TAG_LG L"<lg>"        // Голубой текст на зелёном фоне
#define TAG_LH L"<lh>"        // Белый текст на зелёном фоне
#define TAG_LI L"<li>"        // Серый текст на зелёном фоне
#define TAG_MA L"<ma>"        // Черный текст на жёлтом фоне
#define TAG_MB L"<mb>"        // Красный текст на жёлтом фоне
#define TAG_MC L"<mc>"        // Зелёный текст на жёлтом фоне
#define TAG_MD L"<md>"        // Жёлтый текст на жёлтом фоне
#define TAG_ME L"<me>"        // Синий текст на жёлтом фоне
#define TAG_MF L"<mf>"        // Пурпурный текст на жёлтом фоне
#define TAG_MG L"<mg>"        // Голубой текст на жёлтом фоне
#define TAG_MH L"<mh>"        // Белый текст на жёлтом фоне
#define TAG_MI L"<mi>"        // Серый текст на жёлтом фоне
#define TAG_NA L"<na>"        // Черный текст на синем фоне
#define TAG_NB L"<nb>"        // Красный текст на синем фоне
#define TAG_NC L"<nc>"        // Зелёный текст на синем фоне
#define TAG_ND L"<nd>"        // Жёлтый текст на синем фоне
#define TAG_NE L"<ne>"        // Синий текст на синем фоне
#define TAG_NF L"<nf>"        // Пурпурный текст на синем фоне
#define TAG_NG L"<ng>"        // Голубой текст на синем фоне
#define TAG_NH L"<nh>"        // Белый текст на синем фоне
#define TAG_NI L"<ni>"        // Серый текст на синем фоне
#define TAG_OA L"<oa>"        // Черный текст на пурпурном фоне
#define TAG_OB L"<ob>"        // Красный текст на пурпурном фоне
#define TAG_OC L"<oc>"        // Зелёный текст на пурпурном фоне
#define TAG_OD L"<od>"        // Жёлтый текст на пурпурном фоне
#define TAG_OE L"<oe>"        // Синий текст на пурпурном фоне
#define TAG_OF L"<of>"        // Пурпурный текст на пурпурном фоне
#define TAG_OG L"<og>"        // Голубой текст на пурпурном фоне
#define TAG_OH L"<oh>"        // Белый текст на пурпурном фоне
#define TAG_OI L"<oi>"        // Серый текст на пурпурном фоне
#define TAG_PA L"<pa>"        // Черный текст на голубом фоне
#define TAG_PB L"<pb>"        // Красный текст на голубом фоне
#define TAG_PC L"<pc>"        // Зелёный текст на голубом фоне
#define TAG_PD L"<pd>"        // Жёлтый текст на голубом фоне
#define TAG_PE L"<pe>"        // Синий текст на голубом фоне
#define TAG_PF L"<pf>"        // Пурпурный текст на голубом фоне
#define TAG_PG L"<pg>"        // Голубой текст на голубом фоне
#define TAG_PH L"<ph>"        // Белый текст на голубом фоне
#define TAG_PI L"<pi>"        // Серый текст на голубом фоне
#define TAG_RESET_BLACK L"</a>"    // Сброс цвета текста: черный
#define TAG_RESET_RED L"</b>"      // Сброс цвета текста: красный
#define TAG_RESET_GREEN L"</c>"    // Сброс цвета текста: зеленый
#define TAG_RESET_YELLOW L"</d>"   // Сброс цвета текста: желтый
#define TAG_RESET_BLUE L"</e>"     // Сброс цвета текста: синий
#define TAG_RESET_MAGENTA L"</f>"  // Сброс цвета текста: пурпурный
#define TAG_RESET_CYAN L"</g>"     // Сброс цвета текста: голубой
#define TAG_RESET_WHITE L"</h>"    // Сброс цвета текста: белый
#define TAG_RESET_GRAY L"</i>"     // Сброс цвета текста: серый
#define TAG_RESET_BG_BLACK L"</j>" // Сброс цвета фона: черный
#define TAG_RESET_BG_RED L"</k>"   // Сброс цвета фона: красный
#define TAG_RESET_BG_GREEN L"</l>" // Сброс цвета фона: зеленый
#define TAG_RESET_BG_YELLOW L"</m>"// Сброс цвета фона: желтый
#define TAG_RESET_BG_BLUE L"</n>"  // Сброс цвета фона: синий
#define TAG_RESET_BG_MAGENTA L"</o>" // Сброс цвета фона: пурпурный
#define TAG_RESET_BG_CYAN L"</p>"  // Сброс цвета фона: голубой
#define TAG_RESET_BG_WHITE L"</q>" // Сброс цвета фона: белый
#define TAG_RESET_BOLD L"</r>"     // Сброс выделения жирным

// Структура для соответствия тега и ANSI-кода
struct TagMapping {
    const wchar_t *tag;
    const wchar_t *ansi_code;
    int reset; // 0 для установки атрибута, 1 для сброса
};

// Прототипы функций
void parse_tags(const wchar_t *input, wchar_t *output, size_t max_output);
void print_tagged_text(const wchar_t *buf, int x, int y, int max_x);

#endif // ANSI_TAGS_H
