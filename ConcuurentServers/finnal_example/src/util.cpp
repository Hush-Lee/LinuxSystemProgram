#include "lib/util.hpp"
#include <cstdio>
#include <cstdlib>

void ErrorIf(bool condition, const char *message) {
  if (condition) {
    perror(message);
    exit(EXIT_FAILURE);
  }
}