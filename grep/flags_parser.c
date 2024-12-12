#include "s21_grep.h"

void pattern_add(Flags *flags, char *pattern) {
  int n = strlen(pattern);
  if (flags->len_pattern + n + 2 >= MAX_PATTERN_SIZE) {
    fprintf(stderr, "Pattern buffer overflow. Increase MAX_PATTERN_SIZE.\n");
  }
  if (flags->len_pattern != 0) {
    strcat(flags->pattern + flags->len_pattern, "|");
    flags->len_pattern++;
  }
  flags->len_pattern +=
      sprintf(flags->pattern + flags->len_pattern, "(%s)", pattern);
}

void add_reg_from_file(Flags *flags, char *filepath) {
  FILE *f = fopen(filepath, "r");
  if (f == NULL) {
    if (!flags->s) {
      perror(filepath);
    }
    return;
  }
  char line[MAX_PATTERN_SIZE];
  while (fgets(line, sizeof(line), f)) {
    if (line[strlen(line) - 1] == '\n') {
      line[strlen(line) - 1] = '\0';
    }
    pattern_add(flags, line);
  }
  fclose(f);
}

Flags Flags_parser(int argc, char *argv[]) {
  Flags flags = {0};
  flags.pattern = malloc(MAX_PATTERN_SIZE * sizeof(char));
  if (flags.pattern == NULL) {
    perror("malloc");
  }
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        flags.e = 1;
        pattern_add(&flags, optarg);
        break;
      case 'i':
        flags.i = REG_ICASE;
        break;
      case 'v':
        flags.v = 1;
        break;
      case 'c':
        flags.c = 1;
        break;
      case 'l':
        flags.l = 1;
        break;
      case 'n':
        flags.n = 1;
        break;
      case 'h':
        flags.h = 1;
        break;
      case 's':
        flags.s = 1;
        break;
      case 'f':
        flags.f = 1;
        add_reg_from_file(&flags, optarg);
        break;
      case 'o':
        flags.o = 1;
        break;
      default:
        printf("Usage: 21_grep [-eivclnhsfo] [file..]\n");
        break;
    }
  }
  if (flags.len_pattern == 0) {
    pattern_add(&flags, argv[optind]);
    optind++;
  }
  if (argc - optind == 1) {
    flags.h = 1;
  }
  return flags;
}
