#include "grep_functions.h"

int main(int argc, char* argv[]) {
  Options flags = {0};
  regex_t regex;
  char expression[4096] = {0};
  define_options(&flags, argc, argv, expression);
  if (!compile_re(&flags, &regex, expression)) {
    for (int i = optind; i < argc; i++) {
      search(&flags, &regex, argv[i]);
    }
  }
  regfree(&regex);
  return 0;
}
