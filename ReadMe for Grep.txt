ReadMe for Grep

Файл s21_grep.h

#ifndef S21_GREP_H
#define S21_GREP_H
Это предохранитель от повторного включения
 заголовочного файла. Если файл s21_grep.h уже включен,
  то повторное включение игнорируется.


#include <getopt.h>  // Для обработки аргументов командной строки
#include <regex.h>   // Для работы с регулярными выражениями
#include <stdio.h>   // Для ввода-вывода
#include <stdlib.h>  // Для функций malloc, free и т.д.
#include <string.h>  // Для работы со строками
Подключаем необходимые библиотеки для работы программы.


#define MAX_PATTERN_SIZE 4096  // Максимальный размер буфера для паттернов
#define MAX_FILE_PATH_SIZE 4096  // Максимальный размер пути к файлу
#define MAX_FILES 4096  // Максимальное количество обрабатываемых файлов
Определяем константы для ограничения размеров буферов и количества файлов.


// Структура для хранения флагов и паттернов
typedef struct Flags {
  int e;  // Флаг -e (указывает паттерн)
  int i;  // Флаг -i (игнорирование регистра)
  int v;  // Флаг -v (инверсия совпадений)
  int c;  // Флаг -c (вывод количества совпадений)
  int l;  // Флаг -l (вывод только имен файлов с совпадениями)
  int n;  // Флаг -n (вывод номеров строк)
  int h;  // Флаг -h (не выводить имя файла)
  int s;  // Флаг -s (игнорировать ошибки открытия файлов)
  int f;  // Флаг -f (загрузка паттернов из файла)
  int o;  // Флаг -o (вывод только совпадений)
  char *pattern;  // Буфер для хранения паттернов
  int len_pattern;  // Длина буфера паттернов
} Flags;
Определяем структуру Flags для хранения флагов и паттернов.


void add_pattern(Flags *flags, char *pattern);  // Добавление паттерна в буфер
void load_patterns_from_file(Flags *flags, char *filepath);  // Загрузка паттернов из файла
Flags parse_flags(int argc, char *argv[]);  // Парсинг флагов и аргументов
void print_line(char *line, int n);  // Вывод строки
void print_regex_match(regex_t *re, char *line);  // Вывод совпадений регулярного выражения
void processFile(Flags flags, char *path, regex_t *reg);  // Обработка файла
void output(Flags flags, int argc, char **argv);  // Вывод результатов
Объявляем прототипы функций, которые будут реализованы в основном коде.


#endif
Закрываем предохранитель от повторного включения.

Файл s21_grep.c

#include "s21_grep.h"
Подключаем заголовочный файл, 
чтобы использовать объявленные в нем функции и структуры.

Функция add_pattern

void add_pattern(Flags *flags, char *pattern) {
  int n = strlen(pattern);  // Получаем длину паттерна
  if (flags->len_pattern + n + 2 >= MAX_PATTERN_SIZE) {  // Проверяем, не переполнится ли буфер
    fprintf(stderr, "Pattern buffer overflow. Increase MAX_PATTERN_SIZE.\n");  // Выводим ошибку
  }
  if (flags->len_pattern != 0) {  // Если буфер не пустой, добавляем разделитель "|"
    strcat(flags->pattern + flags->len_pattern, "|");
    flags->len_pattern++;
  }
  flags->len_pattern +=  // Добавляем новый паттерн в буфер
      sprintf(flags->pattern + flags->len_pattern, "(%s)", pattern);
}
Функция добавляет новый паттерн в общий буфер паттернов.

Функция load_patterns_from_file

void load_patterns_from_file(Flags *flags, char *filepath) {
  FILE *f = fopen(filepath, "r");  // Открываем файл для чтения
  if (f == NULL) {  // Если файл не открылся
    if (!flags->s) {  // Если флаг -s не установлен
      perror(filepath);  // Выводим ошибку
    }
    return;
  }
  char line[MAX_PATTERN_SIZE];  // Буфер для строки
  while (fgets(line, sizeof(line), f)) {  // Читаем файл построчно
    if (line[strlen(line) - 1] == '\n') {  // Убираем символ новой строки
      line[strlen(line) - 1] = '\0';
    }
    add_pattern(flags, line);  // Добавляем строку как паттерн
  }
  fclose(f);  // Закрываем файл
}
Функция загружает паттерны из файла и добавляет их в общий буфер.

Функция parse_flags

Flags parse_flags(int argc, char *argv[]) {
  Flags flags = {0};  // Инициализируем структуру флагов нулями
  flags.pattern = malloc(MAX_PATTERN_SIZE * sizeof(char));  // Выделяем память для буфера паттернов
  if (flags.pattern == NULL) {  // Проверяем, удалось ли выделить память
    perror("malloc");  // Выводим ошибку
  }
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {  // Обрабатываем флаги с помощью getopt
    switch (opt) {
      case 'e':  // Флаг -e
        flags.e = 1;
        add_pattern(&flags, optarg);  // Добавляем паттерн
        break;
      case 'i':  // Флаг -i
        flags.i = REG_ICASE;  // Устанавливаем флаг игнорирования регистра
        break;
      case 'v':  // Флаг -v
        flags.v = 1;
        break;
      case 'c':  // Флаг -c
        flags.c = 1;
        break;
      case 'l':  // Флаг -l
        flags.l = 1;
        break;
      case 'n':  // Флаг -n
        flags.n = 1;
        break;
      case 'h':  // Флаг -h
        flags.h = 1;
        break;
      case 's':  // Флаг -s
        flags.s = 1;
        break;
      case 'f':  // Флаг -f
        flags.f = 1;
        load_patterns_from_file(&flags, optarg);  // Загружаем паттерны из файла
        break;
      case 'o':  // Флаг -o
        flags.o = 1;
        break;
      default:  // Неизвестный флаг
        printf("Usage: 21_grep [-eivclnhsfo] [file..]\n");  // Выводим сообщение об использовании
        break;
    }
  }
  if (flags.len_pattern == 0) {  // Если паттерн не указан явно
    add_pattern(&flags, argv[optind]);  // Добавляем первый аргумент как паттерн
    optind++;
  }
  if (argc - optind == 1) {  // Если указан только один файл
    flags.h = 1;  // Устанавливаем флаг -h
  }
  return flags;  // Возвращаем структуру флагов
}
Функция парсит флаги и аргументы командной строки.

Функция print_line

void print_line(char *line, int n) {
  for (int i = 0; i < n; i++) {  // Выводим строку посимвольно
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') {  // Если строка не заканчивается на \n
    putchar('\n');  // Добавляем символ новой строки
  }
}
Функция выводит строку.

Функция print_regex_match

void print_regex_match(regex_t *re, char *line) {
  regmatch_t match;
  int offset = 0;
  while (!regexec(re, line + offset, 1, &match, 0)) {  // Поиск всех совпадений в строке
    for (int i = match.rm_so; i < match.rm_eo; i++) {  // Выводим совпадение
      putchar(line[i]);
    }
    putchar('\n');
    offset += match.rm_eo;  // Переходим к следующему совпадению
  }
}
Функция выводит все совпадения регулярного выражения в строке.

Функция processFile

void processFile(Flags flags, char *path, regex_t *reg) {
  FILE *f = fopen(path, "r");  // Открываем файл для чтения
  if (f == NULL) {  // Если файл не открылся
    if (!flags.s) {  // Если флаг -s не установлен
      perror(path);  // Выводим ошибку
    }
    return;
  }
  char line[MAX_PATTERN_SIZE];  // Буфер для строки
  int line_count = 1;  // Счетчик строк
  int counter = 0;  // Счетчик совпадений
  while (fgets(line, sizeof(line), f)) {  // Читаем файл построчно
    int result = regexec(reg, line, 0, NULL, 0);  // Проверяем совпадение с паттерном
    if ((result == 0 && !flags.v) || (flags.v && result != 0)) {  // Если совпадение найдено
      if (!flags.c && !flags.l) {  // Если не флаги -c и -l
        if (!flags.h) {  // Если не флаг -h
          printf("%s:", path);  // Выводим имя файла
        }
        if (flags.n) {  // Если флаг -n
          printf("%d:", line_count);  // Выводим номер строки
        }
        if (flags.o) {  // Если флаг -o
          print_regex_match(reg, line);  // Выводим совпадения
        } else {
          print_line(line, strlen(line));  // Выводим строку
        }
      }
      counter++;  // Увеличиваем счетчик совпадений
    }
    line_count++;  // Увеличиваем счетчик строк
  }
  if (flags.c && !flags.l) {  // Если флаг -c и не флаг -l
    if (!flags.h) printf("%s:", path);  // Выводим имя файла
    printf("%d\n", counter);  // Выводим количество совпадений
  }
  if (flags.l && counter > 0 && !flags.c) {  // Если флаг -l и найдено хотя бы одно совпадение
    printf("%s\n", path);  // Выводим имя файла
  }
  if (flags.l && flags.c) {  // Если флаги -l и -c
    if (!flags.h) printf("%s:", path);  // Выводим имя файла
    if (counter > 0) {  // Если найдено хотя бы одно совпадение
      printf("1\n%s\n", path);  // Выводим количество совпадений и имя файла
    } else {
      printf("0\n");  // Выводим 0
    }
  }
  fclose(f);  // Закрываем файл
}
Функция обрабатывает файл, ищет совпадения с паттерном и выводит результаты.

Функция output

void output(Flags flags, int argc, char **argv) {
  regex_t re;
  int error = regcomp(&re, flags.pattern, REG_EXTENDED | flags.i);  // Компилируем регулярное выражение
  if (error) {  // Если произошла ошибка
    perror("regcomp");  // Выводим ошибку
  }
  for (int i = optind; i < argc; i++) {  // Обрабатываем все файлы
    processFile(flags, argv[i], &re);
  }
  regfree(&re);  // Освобождаем память, занятую регулярным выражением
}
Функция выводит результаты обработки файлов.

Функция main

int main(int argc, char **argv) {
  Flags flags = parse_flags(argc, argv);  // Парсим флаги и аргументы
  output(flags, argc, argv);  // Выводим результаты
  free(flags.pattern);  // Освобождаем память, занятую буфером паттернов
  return 0;  // Возвращаем 0 (успешное завершение)
}