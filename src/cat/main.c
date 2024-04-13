#include "cat_functions.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cat_no_args();
  } else {
    cat(argc, argv);
  }
  return 0;
}
