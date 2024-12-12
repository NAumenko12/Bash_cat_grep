#include "s21_grep.h"
#include <stdio.h>
#include <string.h>
#include <regex.h>

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
  while (!regexec(re, line + offset, 1, &match, 0)) {
    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[i]);
    }
    putchar('\n');
    offset += match.rm_eo;
  }
}

void processFile(Flags flags, char *path, regex_t *reg) {
  FILE *f = fopen(path, "r");
  if (!f) {
    if (!flags.s) perror(path);
    return;
  }

  char line[MAX_PATTERN_SIZE];
  int line_count = 1, counter = 0;

  while (fgets(line, sizeof(line), f)) {
    int result = regexec(reg, line, 0, NULL, 0);
    if ((!result && !flags.v) || (flags.v && result)) {
      if (!flags.c && !flags.l) {
        if (!flags.h) printf("%s:", path);
        if (flags.n) printf("%d:", line_count);
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

  if (flags.l && counter > 0) {
    printf("%s\n", path);
    if (flags.c) printf("%d\n", counter > 0);
  }

  fclose(f);
}

void output(Flags flags, int argc, char **argv) {
  regex_t re;
  if (regcomp(&re, flags.pattern, REG_EXTENDED | (flags.i ? REG_ICASE : 0))) {
    perror("regcomp");
    return;
  }

  for (int i = optind; i < argc; i++) {
    processFile(flags, argv[i], &re);
  }

  regfree(&re);
}
