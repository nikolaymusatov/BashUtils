#ifndef SRC_GREP_GREP_FUNCTIONS_H_
#define SRC_GREP_GREP_FUNCTIONS_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct Options {
  int expression;
  int ignore_case;
  int inversion;
  int str_count;
  int matching_files;
  int number;
  int hide_filename;
  int hide_errors;
  int re_file;
  int only_match;
  int num_files;
} Options;

void define_options(Options* options, int argc, char* argv[], char* pattern);
void get_re(char* re, char* pattern);
void fget_re(char* path, char* pattern);
int compile_re(Options* options, regex_t* re, char* pattern);
void search(Options* options, regex_t* re, char* path);
void print_matches(Options* options, regex_t* re, char* data, char* f_path,
                   int lines);
void print_lines(Options* options, regex_t* re, FILE* file, char* line,
                 char* f_path, int lines, int* count);
void print_counts(Options* options, char* f_path, int count);

#endif  //  SRC_GREP_GREP_FUNCTIONS_H_
