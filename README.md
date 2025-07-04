# ComplexModulesMu

MuParser — это консольное приложение на языке C для обработки и отображения текстовых файлов с расширением `.mu`. Оно поддерживает разметку текста с использованием тегов, которые преобразуются в ANSI-коды для форматирования (цвет текста, фона, жирный шрифт). Проект включает несколько утилит с различной функциональностью: от простого вывода текста до интерактивной навигации по разделам, обозначенным тегами.

## Возможности

- **Парсинг тегов**: Поддержка тегов для форматирования текста (например, `<a>` для черного текста, `<r>` для жирного шрифта, `<qa>` для комбинации цветов).
- **Чтение файлов `.mu`**: Валидация и обработка файлов с поддержкой UTF-8 (включая BOM).
- **Интерактивная навигация**:
  - `parser_exit0_mu`: Простой вывод содержимого файла с форматированием.
  - `parser_scroll_mu`: Прокрутка текста с помощью стрелок (вверх/вниз).
  - `parser_flipp_mu`: Навигация по разделам, обозначенным тегами `<XX>`, с помощью стрелок (влево/вправо).
  - `parser_navigator_mu`: Интерактивный поиск и переход к тегам через ввод в командной строке.
- **Эффективное обновление экрана**: Использование буфера для минимизации перерисовки терминала.
- **Кроссплатформенность**: Работает в Unix-подобных системах (Linux, macOS) с поддержкой терминалов, совместимых с ANSI.

## Зависимости

- Компилятор GCC
- Стандартная библиотека C
- Библиотека `termios` (доступна в Unix-подобных системах)
- Библиотека `libtag_termios_mu.a` (включена в проект, собирается автоматически)

## Сборка

1. Клонируйте репозиторий:
   ```bash
   git clone https://github.com/your-username/muparser.git
   cd muparser
   ```

2. Перейдите в директорию `tagTermiosMu` и соберите статическую библиотеку `libtag_termios_mu.a`:
   ```bash
   cd tagTermiosMu
   make
   cd ..
   ```

3. Скомпилируйте утилиты, используя команды из `compilation.mu`:

## Использование

Каждая утилита принимает в качестве аргумента путь к файлу `.mu`:

```bash
./parser_exit0_mu example.mu
./parser_scroll_mu example.mu
./parser_flipp_mu example.mu
./parser_navigator_mu example.mu
```

### Управление

- **parser_exit0_mu**: Выводит содержимое файла и завершает работу.
- **parser_scroll_mu**:
  - Стрелка вверх/вниз: прокрутка текста.
  - `q`: выход.
- **parser_flipp_mu**:
  - Стрелка влево/вправо: переход между разделами (тегами `<XX>`).
  - Стрелка вверх/вниз: прокрутка внутри раздела.
  - `q`: выход.
- **parser_navigator_mu**:
  - Ввод тега (например, `<00>`): переход к разделу.
  - Стрелка влево/вправо: переход между тегами.
  - Стрелка вверх/вниз: прокрутка внутри раздела.
  - Backspace: удаление символа в запросе.
  - `q`: выход.

### Формат файла `.mu`

Файл `.mu` — это текстовый файл в кодировке UTF-8, содержащий теги форматирования. Пример:

```mu
<00>
<r>Hello, <b>World!</b></r>
<01>
<c>This is green text.</c>
```

Поддерживаемые теги описаны в `tags.mu`

## Структура проекта

```
muparser/
├── compilation.mu         # Команды для компиляции
├── parser_exit0_mu.c      # Утилита для простого вывода
├── parser_scroll_mu.c     # Утилита с прокруткой
├── parser_flipp_mu.c      # Утилита с навигацией по тегам
├── parser_navigator_mu.c  # Утилита с поиском тегов
├── utils.c                # Общие функции (чтение файлов, парсинг)
├── utils.h                # Заголовочный файл для utils.c
├── tagTermiosMu/
│   ├── Makefile           # Сборка библиотеки libtag_termios_mu.a
│   ├── tag_termios_mu.c   # Парсинг тегов и ANSI-коды
│   ├── tag_termios_mu.h   # Определение тегов и прототипы
```

## Ограничения

- Максимальный размер входного файла: 32,768 символов.
- Максимальное количество строк: 2,048.
- Поддержка только UTF-8 кодировки.
- Работает только в терминалах с поддержкой ANSI-кодов.
- Ограниченная обработка ошибок при некорректных тегах.

## Лицензия

Проект распространяется под лицензией MIT.
