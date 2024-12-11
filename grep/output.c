#include "s21_cat.h"

char v_output(char ch) {
  if (ch <= 31 && ch != '\n' && ch != '\r' && ch != '\t') {
    putchar('^');
    ch += 64;
  } else if (ch == 127) {
    putchar('^');
    ch = '?';
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