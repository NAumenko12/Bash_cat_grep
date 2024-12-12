

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


