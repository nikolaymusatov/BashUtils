#include "cat_functions.h"

#include <stdio.h>
#include <string.h>

void cat_no_args() {
  char data[4096] = {0};
  while (1) {
    scanf("%s", data);
    printf("%s\n", data);
  }
}

void cat(int arg_c, char *arg_v[]) {
  struct Options options = {0};
  int flag_check = 0;
  int file_check = 0;
  for (int i = 1; i < arg_c; i++) {
    if (*arg_v[i] == '-') {
      if (file_check == 1) {
        fprintf(stderr, "%s%s: %s", "cat: ", arg_v[i],
                "No such file or directory\n");
      } else {
        flag_check = flags_filler(&options, arg_v[i]);
      }
    } else {
      if (flag_check == 0) {
        file_check = 1;
        files_processer(&options, arg_v[i]);
      }
    }
  }
}

int flags_filler(struct Options *flags, char arg[]) {
  int wrong_flag = 0;
  ++arg;
  if (*arg == '-') {
    if (strcmp(arg, "-number-nonblank") == 0) {
      flags->number_nonblank = 1;
    } else if (strcmp(arg, "-number") == 0) {
      flags->number_all = 1;
    } else if (strcmp(arg, "-squeeze-blank") == 0) {
      flags->squeeze = 1;
    } else {
      wrong_flag = 1;
      fprintf(stderr, "%s'-%s'%s", "cat: Wrong option ", arg, "\n");
    }
  } else {
    for (char *f = arg; *f; f++) {
      switch (*f) {
        case 'b':
          flags->number_nonblank = 1;
          break;
        case 'e':
          flags->show_endl = 1;
          flags->show_signs = 1;
          break;
        case 'E':
          flags->show_endl = 1;
          break;
        case 'n':
          flags->number_all = 1;
          break;
        case 's':
          flags->squeeze = 1;
          break;
        case 't':
          flags->show_tabs = 1;
          flags->show_signs = 1;
          break;
        case 'T':
          flags->show_tabs = 1;
          break;
        case 'v':
          flags->show_signs = 1;
          break;
        default:
          wrong_flag = 1;
          fprintf(stderr, "%s'-%c'%s", "cat: Wrong option ", *f, "\n");
      }
    }
  }
  return wrong_flag;
}

void files_processer(struct Options *flags, char path[]) {
  if (flags->number_all && flags->number_nonblank) flags->number_all = 0;
  FILE *file;
  if ((file = fopen(path, "r")) == NULL) {
    fprintf(stderr, "%s%s: %s", "cat: ", path, "No such file or directory\n");
  } else {
    int line_number = 1;
    int empty_line = 0;
    char sign = '0';
    char temp_sign = '\n';
    while ((sign = fgetc(file)) != EOF) {
      if (flags->squeeze) {
        if (sign == '\n' && temp_sign == '\n' && !empty_line) {
          empty_line = 1;
        } else if (sign == '\n' && temp_sign == '\n' && empty_line) {
          continue;
        } else if (sign != '\n' && temp_sign == '\n') {
          empty_line = 0;
        }
      }
      if (flags->number_all && temp_sign == '\n') {
        printf("%6d\t", line_number);
        line_number++;
      }
      if (flags->number_nonblank && sign != '\n' && temp_sign == '\n') {
        printf("%6d\t", line_number);
        line_number++;
      }
      if (flags->show_endl && sign == '\n') {
        if (flags->number_nonblank && temp_sign == '\n') printf("%6c\t", ' ');
        printf("%c\n", '$');
        temp_sign = sign;
        continue;
      }
      if (flags->show_tabs && sign == '\t') {
        printf("%c%c", '^', 'I');
        temp_sign = sign;
        continue;
      }
      if (flags->show_signs &&
          ((sign >= 0 && sign <= 31 && sign != 10 && sign != 9) ||
           sign == 127)) {
        if (sign == 127)
          printf("^?");
        else
          printf("^%c", sign + 64);
        temp_sign = sign;
        continue;
      }
      printf("%c", sign);
      temp_sign = sign;
    }
    fclose(file);
  }
}
