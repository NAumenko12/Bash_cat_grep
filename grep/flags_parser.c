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