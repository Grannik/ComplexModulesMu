
<e> *** Инструкция по добавлению нового тега <b> в программу *** </e>

 Эта инструкция поможет добавить тег <b> (красный текст) в программу. Выполняйте шаги по порядку. Каждый шаг объясняет, что и где писать.

1. Откройте файл `ansi_tags.h`. Найдите строки, где написаны теги, например:
   #define TAG_NEWLINE L"\n"

2. Добавьте новую строку после `#define TAG_NEWLINE L"\n"`:
   #define TAG_RED L"<b>"        // Цвет текста: красный

3. Откройте файл `ansi_tags.c`. Найдите массив `tag_mappings`. Он выглядит так:
   const struct TagMapping tag_mappings[] = {
       { TAG_NEWLINE, 0, 0 },                  // Перенос строки
       ...

4. Добавьте новую строку после `{ TAG_NEWLINE, 0, 0 },`:
   { TAG_RED, COLOR_PAIR(3), 0 },          // Красный текст

5. В том же файле `ansi_tags.c` найдите функцию `init_tag_colors`. Она начинается так:
   void init_tag_colors(void) {
       start_color();

6. Добавьте новую строку после `init_pair(2, COLOR_YELLOW, COLOR_BLACK);`:
   init_pair(3, COLOR_RED, COLOR_BLACK);    // Красный текст (<b>)

7. Откройте файл `minor_menu.c`. Найдите функцию `init_menu_items`. Она содержит блоки вроде:
   items[0] = (MenuItem){
       ...

8. Найдите последний пункт меню, например:
   items[8] = (MenuItem){
       .description = L" Gray text",
       .details = {L"<i>| \\033[37m | Gray text | Текст: серый |"},
       .detail_lines = 1
   };

9. Добавьте новый блок для тега <b> после последнего пункта (например, после `items[8]`):
   items[9] = (MenuItem){
       .description = L" Red text",
       .details = {L"<b>| \\033[31m | Red text | Текст: красный |"},
       .detail_lines = 1
   };

10. Откройте файл `minor_menu.h`. Найдите строку:
    #define MAX_ITEMS 9

11. Измените её на:
    #define MAX_ITEMS 10
    Это добавляет место для нового пункта меню.

12. В том же файле `minor_menu.h` проверьте, есть ли строка:
    const wchar_t *details[30];
    Если она есть, ничего не меняйте.

13. В `minor_menu.c` найдите функцию `display_details`. Проверьте, есть ли строка:
    for (int i = 0; i < item->detail_lines && i < 30; i++) {
    Если она есть, ничего не меняйте.

14. Сохраните все файлы. Скомпилируйте программу командой:
    gcc -o minor_menu minor_menu.c ansi_tags.c hotkey.c -lncursesw

15. Запустите программу. Проверьте, что тег <b> появился в меню и делает текст красным.

*** Готовый блок для копирования в init_menu_items ***
items[9] = (MenuItem){
    .description = L" Red text",
    .details = {L"<b>| \\033[31m | Red text | Текст: красный |"},
    .detail_lines = 1
};
