ReadMe for Cat

#ifndef SRC_CAT_FLAGS_H_
#define SRC_CAT_FLAGS_H_

#include <getopt.h> //библиотека ждя обработки аргументов командной строки
#include <stdio.h> // стандартная библиотека ввода-вывода
#include <stdlib.h>
#include <string.h> // для работы со строками можно удалить

//Определяет структуру Flags, 
//которая содержит флаги для настройки поведения программы.

typedef struct {
  int b;
  int E;
  int n;
  int s;
  int T;
  int v;
} Flags;

Flags parse_options(int argc, char **argv, int *error_flag);
// обработка аргументов командной строки и возвращает структуру Flags
char char_process_nonprinting(char ch);
// обрабатыват непечатные символы аски код 
void print_line(Flags *flags, char *line, int len);
//печатает строку с учетом флагов
void process_file(Flags *flags, char **argv);
// обработка файла с учетом флагов

#endif

#include "s21_cat.h"

// Функция для парсинга опций командной строки
Flags parse_options(int argc, char **argv, int *error_flag) {
  Flags flags = {0};  // Инициализация структуры флагов нулями

  // Определение длинных опций
  struct option long_options[] = {
      {"number", no_argument, NULL, 'n'},           // --number → -n
      {"number-nonblank", no_argument, NULL, 'b'},  // --number-nonblank → -b
      {"squeeze-blank", no_argument, NULL, 's'},    // --squeeze-blank → -s
      {"show-ends", no_argument, NULL, 'E'},        // --show-ends → -E
      {"show-tabs", no_argument, NULL, 'T'},        // --show-tabs → -T
      {"show-nonprinting", no_argument, NULL, 'v'}, // --show-nonprinting → -v
      {0, 0, 0, 0}                                  // Конец массива
  };

  int opt;  // Переменная для хранения текущей опции

  // Цикл для обработки опций с помощью getopt_long
  while ((opt = getopt_long(argc, argv, "+bEnsetTv", long_options, NULL)) != -1) {
    switch (opt) {
      case 'b':  // Обработка опции -b (--number-nonblank)
        flags.b = 1;  // Установка флага b
        break;
      case 'E':  // Обработка опции -E (--show-ends)
        flags.E = 1;  // Установка флага E
        break;
      case 'n':  // Обработка опции -n (--number)
        flags.n = 1;  // Установка флага n
        break;
      case 's':  // Обработка опции -s (--squeeze-blank)
        flags.s = 1;  // Установка флага s
        break;
      case 't':  // Обработка опции -t (эквивалент -T и -v)
        flags.T = 1;  // Установка флага T
        flags.v = 1;  // Установка флага v
        break;
      case 'e':  // Обработка опции -e (эквивалент -E и -v)
        flags.E = 1;  // Установка флага E
        flags.v = 1;  // Установка флага v
        break;
      case 'T':  // Обработка опции -T (--show-tabs)
        flags.T = 1;  // Установка флага T
        break;
      case 'v':  // Обработка опции -v (--show-nonprinting)
        flags.v = 1;  // Установка флага v
        break;
      default:  // Обработка неизвестной опции
        printf("Usage: 21_cat [-bEnsTv] [file..]\n");  // Вывод сообщения об использовании
        *error_flag = 1;  // Установка флага ошибки
        break;
    }
  }

  return flags;  // Возврат структуры с установленными флагами
}

// Функция для обработки непечатаемых символов
char char_process_nonprinting(char ch) {
  // Если символ непечатаемый (ASCII код <= 31, но не перевод строки, возврат каретки или табуляция)
  if (ch <= 31 && ch != '\n' && ch != '\r' && ch != '\t') {
    putchar('^');  // Вывод символа '^'
    ch += 64;      // Преобразование символа в его эквивалент в диапазоне печатаемых символов
  } else if (ch == 127) {  // Если символ DEL (ASCII код 127)
    putchar('^');  // Вывод символа '^'
    ch = '?';      // Замена символа DEL на '?'
  }
  return ch;  // Возврат обработанного символа
}

// Функция для печати строки с учетом флагов
void print_line(Flags *flags, char *line, int len) {
  for (int i = 0; i < len; i++) {  // Цикл по символам строки
    if (flags->T && line[i] == '\t') {  // Если установлен флаг T и текущий символ — табуляция
      printf("^I");  // Вывод '^I' вместо табуляции
    } else {
      if (flags->E && (line[i] == '\n' || line[i] == '\r')) {  // Если установлен флаг E и текущий символ — перевод строки
        putchar('$');  // Вывод '$' в конце строки
      }
      if (flags->v) {  // Если установлен флаг v
        line[i] = char_process_nonprinting(line[i]);  // Обработка непечатаемых символов
      }
      putchar(line[i]);  // Вывод текущего символа
    }
  }
}

// Функция для обработки файла
void process_file(Flags *flags, char **argv) {
  FILE *f = fopen(argv[optind], "r");  // Открытие файла для чтения
  if (!f) {  // Если файл не удалось открыть
    perror("fopen");  // Вывод сообщения об ошибке
    return;  // Завершение функции
  }

  char line[4096];  // Буфер для хранения строки
  int line_count = 1;  // Счетчик строк
  int empty_count = 0;  // Счетчик пустых строк
  int current_length = 0;  // Текущая длина строки
  int ch;  // Переменная для хранения текущего символа

  // Чтение файла посимвольно
  while ((ch = fgetc(f)) != EOF) {
    line[current_length++] = ch;  // Добавление символа в буфер

    if (ch == '\n') {  // Если текущий символ — перевод строки
      line[current_length] = '\0';  // Завершение строки нулевым символом

      if (line[0] == '\n') {  // Если строка пустая
        empty_count++;  // Увеличение счетчика пустых строк
      } else {
        empty_count = 0;  // Сброс счетчика пустых строк
      }

      if (flags->s && empty_count > 1) {  // Если установлен флаг s и пустых строк больше одной
        current_length = 0;  // Пропуск строки
        continue;
      }

      if (flags->b && line[0] != '\n') {  // Если установлен флаг b и строка не пустая
        printf("%6d\t", line_count++);  // Вывод номера строки
      } else if (flags->n && !flags->b) {  // Если установлен флаг n и не установлен флаг b
        printf("%6d\t", line_count++);  // Вывод номера строки
      }

      print_line(flags, line, current_length);  // Печать строки
      current_length = 0;  // Сброс текущей длины строки
    }
  }

  // Обработка последней строки, если она не завершена переводом строки
  if (current_length > 0) {
    line[current_length] = '\0';  // Завершение строки нулевым символом
    if (flags->b && line[0] != '\n') {  // Если установлен флаг b и строка не пустая
      printf("%6d\t", line_count++);  // Вывод номера строки
    } else if (flags->n && !flags->b) {  // Если установлен флаг n и не установлен флаг b
      printf("%6d\t", line_count++);  // Вывод номера строки
    }
    print_line(flags, line, current_length);  // Печать строки
  }

  fclose(f);  // Закрытие файла
}

#include "s21_cat.h"

int main(int argc, char *argv[]) {
  int error_flag = 0;  // Флаг ошибки
  int return_code = 0;  // Код возврата

  // Парсинг опций командной строки
  Flags flags = parse_options(argc, argv, &error_flag);

  // Проверка на ошибки или отсутствие файлов
  if (error_flag || optind >= argc) {
    printf("Usage: 21_cat [-bEnsTv] [file..]\n");  // Вывод сообщения об использовании
    return_code = 1;  // Установка кода возврата на 1 (ошибка)
  } else {
    // Обработка всех файлов, переданных в командной строке
    for (int i = optind; i < argc; i++) {
      process_file(&flags, argv);  // Обработка текущего файла
    }
  }

  return return_code;  // Возврат кода возврата
}