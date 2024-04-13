#ifndef SRC_CAT_CAT_FUNCTIONS_H_
#define SRC_CAT_CAT_FUNCTIONS_H_

struct Options {
  int number_nonblank;
  int number_all;
  int show_signs;
  int show_endl;
  int squeeze;
  int show_tabs;
};

void cat_no_args();
void cat(int arg_c, char *arg_v[]);
int flags_filler(struct Options *flags, char arg[]);
void files_processer(struct Options *flags, char path[]);

#endif  //  SRC_CAT_CAT_FUNCTIONS_H_
