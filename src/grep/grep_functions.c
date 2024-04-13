#include "grep_functions.h"

void define_options(Options* options, int argc, char* argv[], char* pattern) {
  int option = 0;
  while ((option = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (option) {
      case 'e':
        options->expression = 1;
        get_re(optarg, pattern);
        break;
      case 'i':
        options->ignore_case = 1;
        break;
      case 'v':
        options->inversion = 1;
        break;
      case 'c':
        options->str_count = 1;
        break;
      case 'l':
        options->matching_files = 1;
        break;
      case 'n':
        options->number = 1;
        break;
      case 'h':
        options->hide_filename = 1;
        break;
      case 's':
        options->hide_errors = 1;
        break;
      case 'f':
        options->re_file = 1;
        fget_re(optarg, pattern);
        break;
      case 'o':
        options->only_match = 1;
        break;
    }
  }
  if (!(options->expression || options->re_file)) {
    if (argv[optind]) {
      strcat(pattern, argv[optind]);
      optind++;
    }
  }
  if (argc - optind > 1) options->num_files = 1;
}

void get_re(char* re, char* pattern) {
  if (!strcmp(pattern, "")) {
    strcat(pattern, re);
  } else {
    strcat(pattern, "|");
    strcat(pattern, re);
  }
}

void fget_re(char* path, char* pattern) {
  FILE* file;
  int end = 0;
  char regexp[256] = {0};
  if ((file = fopen(path, "r")) == NULL) {
    fprintf(stderr, "%s%s: %s", "grep: ", path, "No such file or directory\n");
  } else {
    while (!feof(file)) {
      if (fgets(regexp, sizeof(regexp), file)) {
        end = strlen(regexp) - 1;
        if (regexp[end] == '\n') regexp[end] = '\0';
        if (!strcmp(regexp, "")) strcat(regexp, "\n");
        if (!strcmp(pattern, "")) {
          strcat(pattern, regexp);
        } else {
          strcat(pattern, "|");
          strcat(pattern, regexp);
        }
      }
    }
    fclose(file);
  }
}

int compile_re(Options* options, regex_t* re, char* pattern) {
  int error = 0;
  char err_msg[80] = {0};
  if (options->ignore_case)
    error = regcomp(re, pattern, REG_ICASE | REG_EXTENDED | REG_NEWLINE);
  else
    error = regcomp(re, pattern, REG_EXTENDED | REG_NEWLINE);
  if (error) {
    regerror(error, re, err_msg, 80);
    fprintf(stderr, "%s%s\n", "grep: ", err_msg);
  }
  return error;
}

void search(Options* options, regex_t* re, char* path) {
  FILE* file;
  char line[8192];
  int str_count = 0;
  int str_number = 0;
  if ((file = fopen(path, "r")) == NULL) {
    if (!options->hide_errors)
      fprintf(stderr, "%s%s: %s", "grep: ", path,
              "No such file or directory\n");
  } else {
    while (!feof(file)) {
      if (fgets(line, sizeof(line), file)) {
        str_number++;
        if (options->only_match &&
            (!options->str_count && !options->matching_files &&
             !options->inversion))
          print_matches(options, re, line, path, str_number);
        else
          print_lines(options, re, file, line, path, str_number, &str_count);
      }
    }
    if (options->str_count) print_counts(options, path, str_count);
    if (options->matching_files && str_count > 0) printf("%s\n", path);
    fclose(file);
  }
}

void print_matches(Options* options, regex_t* re, char* data, char* f_path,
                   int lines) {
  char* text = data;
  regmatch_t pmatch[1];
  while (regexec(re, text, 1, pmatch, 0) == 0) {
    if (options->num_files && !options->hide_filename) printf("%s:", f_path);
    if (options->number) printf("%d:", lines);
    for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++) {
      printf("%c", text[i]);
    }
    printf("\n");
    text += pmatch[0].rm_eo;
  }
}

void print_lines(Options* options, regex_t* re, FILE* file, char* line,
                 char* f_path, int lines, int* count) {
  int match = 0;
  match = !regexec(re, line, 0, NULL, 0);
  if (options->inversion) match = !match;
  if (match) {
    (*count)++;
    if (!options->str_count && !options->matching_files) {
      if (options->num_files && !options->hide_filename) printf("%s:", f_path);
      if (options->number) printf("%d:", lines);
      printf("%s", line);
      if (feof(file)) printf("%s", "\n");
    }
  }
}

void print_counts(Options* options, char* f_path, int count) {
  if (options->num_files && !options->hide_filename) printf("%s:", f_path);
  if (options->matching_files && options->str_count) {
    if (count == 0)
      printf("%d\n", 0);
    else
      printf("%d\n", 1);
  } else {
    printf("%d\n", count);
  }
}
