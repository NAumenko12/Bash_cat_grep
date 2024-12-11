#include "s21_grep.h"

int main(int argc, char **argv) {
  Flags flags = Flags_parser(argc, argv);
  output(flags, argc, argv);
  free(flags.pattern);
  return 0;
}
