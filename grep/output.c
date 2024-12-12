#include "s21_grep.h"

void output_line(char *line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') {
    putchar('\n');
  }
}

void print_match(regex_t *re, char *line) {
  regmatch_t match;
  int offset = 0;
  while (1) {
    int result = regexec(re, line + offset, 1, &match, 0);
    if (result != 0) {
      break;
    }
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[i]);
    }
    putchar('\n');
    offset += match.rm_eo;
  }
}

void processFile(Flags flags, char *path, regex_t *reg) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    if (!flags.s) {
      perror(path);
    }
    return;
  }
  char line[MAX_PATTERN_SIZE];
  int line_count = 1;
  int counter = 0;
  while (fgets(line, sizeof(line), f)) {
    int result = regexec(reg, line, 0, NULL, 0);
    if ((result == 0 && !flags.v) || (flags.v && result != 0)) {
      if (!flags.c && !flags.l) {
        if (!flags.h) {
          printf("%s:", path);
        }
        if (flags.n) {
          printf("%d:", line_count);
        }
        if (flags.o) {
          print_match(reg, line);
        } else {
          output_line(line, strlen(line));
        }
      }
      counter++;
    }
    line_count++;
  }
  if (flags.c && !flags.l) {
    if (!flags.h) printf("%s:", path);
    printf("%d\n", counter);
  }
  if (flags.l && counter > 0 && !flags.c) {
    printf("%s\n", path);
  }
  if (flags.l && flags.c) {
    if (!flags.h) printf("%s:", path);
    if (counter > 0) {
      printf("1\n%s\n", path);
    } else {
      printf("0\n");
    }
  }
  fclose(f);
}

void output(Flags flags, int argc, char **argv) {
  regex_t re;
  int error = regcomp(&re, flags.pattern, REG_EXTENDED | flags.i);
  if (error) {
    perror("regcomp");
  }
  for (int i = optind; i < argc; i++) {
    processFile(flags, argv[i], &re);
  }
  regfree(&re);
}
