#include "mackerelclient/env.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("%s\n", argv[1]);
  printf("%s\n", get_apikey());

  return 0;
}
