#ifndef SRC_CAT_FLAGS_H_
#define SRC_CAT_FLAGS_H_

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int b;
  int E;
  int n;
  int s;
  int T;
  int v;
} Flags;

Flags parse_cat_flags(int argc, char **argv, int *error_flag);
char v_output(char ch);
void output_line(Flags *flags, char *line, int len);
void output(Flags *flags, char **argv);

#endif