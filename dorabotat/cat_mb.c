//посмотреть v_output
/* Флаг -n и -b
Если одновременно указаны флаги -n и -b, 
 код игнорирует флаг -n, так как -b имеет приоритет. Это корректное поведение, 
но стоит добавить проверку, чтобы избежать неожиданных результатов.

if (flags->b && line[0] != '\n') {
  printf("%6d\t", line_count++);
} else if (flags->n && !flags->b) {
  printf("%6d\t", line_count++);
}

*/
#include "s21_cat.h"

Flags parse_cat_flags(int argc, char **argv, int *error_flag) {
  Flags flags = {0};
  struct option long_options[] = {{"number", no_argument, NULL, 'n'},
                                  {"number-nonblank", no_argument, NULL, 'b'},
                                  {"squeeze-blank", no_argument, NULL, 's'},
                                  {"show-ends", no_argument, NULL, 'E'},
                                  {"show-tabs", no_argument, NULL, 'T'},
                                  {"show-nonprinting", no_argument, NULL, 'v'},
                                  {0, 0, 0, 0}};
  int opt;
  while ((opt = getopt_long(argc, argv, "+bEnsetTv", long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'b':
        flags.b = 1;
        break;
      case 'E':
        flags.E = 1;
        break;
      case 'n':
        flags.n = 1;
        break;
      case 's':
        flags.s = 1;
        break;
      case 't':
        flags.T = 1;
        flags.v = 1;
        break;
      case 'e':
        flags.E = 1;
        flags.v = 1;
        break;
      case 'T':
        flags.T = 1;
        break;
      case 'v':
        flags.v = 1;
        break;
      default:
        printf("Usage: 21_cat [-bEnsTv] [file..]\n");
        *error_flag = 1;
        break;
    }
  }
  return flags;
}

char v_output(char ch) {
  if (ch <= 31 && ch != '\n' && ch != '\r' && ch != '\t') {
    putchar('^');
    ch += 64;
  } else if (ch == 127) {
    putchar('^');
    ch = '?';
  } else if (ch > 127) {
    putchar('M');
    putchar('-');
    if (ch <= 159) {
      putchar('^');
      ch -= 64;
    } else {
      ch -= 128;
    }
  }
  return ch;
}

void output_line(Flags *flags, char *line, int len) {
  for (int i = 0; i < len; i++) {
    if (flags->T && line[i] == '\t') {
      printf("^I");
    } else {
      if (flags->E && (line[i] == '\n' || line[i] == '\r')) {
        putchar('$');
      }
      if (flags->v) {
        line[i] = v_output(line[i]);
      }
      putchar(line[i]);
    }
  }
}

void output(Flags *flags, char **argv) {
  FILE *f = fopen(argv[optind], "r");
  if (!f) {
    perror("fopen");
    return;
  }
  char line[4096];
  int line_count = 1;
  int empty_count = 0;
  int current_length = 0;
  int ch;

  while ((ch = fgetc(f)) != EOF) {
    line[current_length++] = ch;

    if (ch == '\n') {
      line[current_length] = '\0';

      if (line[0] == '\n') {
        empty_count++;
      } else {
        empty_count = 0;
      }

      if (flags->s && empty_count > 1) {
        current_length = 0;
        continue;
      }

      if (flags->b && line[0] != '\n') {
        printf("%6d\t", line_count++);
      } else if (flags->n && !flags->b) {
        printf("%6d\t", line_count++);
      }

      output_line(flags, line, current_length);
      current_length = 0;
    }
  }
  if (current_length > 0) {
    line[current_length] = '\0';
    if (flags->b && line[0] != '\n') {
      printf("%6d\t", line_count++);
    } else if (flags->n && !flags->b) {
      printf("%6d\t", line_count++);
    }
    output_line(flags, line, current_length);
  }

  fclose(f);
}

int main(int argc, char *argv[]) {
  int error_flag = 0;
  int return_code = 0;
  Flags flags = parse_cat_flags(argc, argv, &error_flag);
  if (error_flag || optind >= argc) {
    printf("Usage: 21_cat [-bEnsTv] [file..]\n");
    return_code = 1;
  } else {
    for (int i = optind; i < argc; i++) {
      FILE *f = fopen(argv[i], "r");
      if (!f) {
        perror("fopen");
        return_code = 1;
        continue;
      }
      if (argc > optind + 1) {
        printf("==> %s <==\n", argv[i]);
      }
      output(&flags, argv);
      fclose(f);
    }
  }
  return return_code;
}