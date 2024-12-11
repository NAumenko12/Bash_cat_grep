#include "s21_cat.h"

int main(int argc, char *argv[]) {
  int error_flag = 0;
  int return_code = 0;
  Flags flags = parse_cat_flags(argc, argv, &error_flag);
  if (error_flag || optind >= argc) {
    printf("Usage: 21_cat [-bEnsTv] [file..]\n");
    return_code = 1;
  } else {
    for (int i = optind; i < argc; i++) {
      output(&flags, argv);
    }
  }
  return return_code;
}
