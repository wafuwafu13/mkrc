#include "mackerelclient/env.h"
#include "mackerelclient/host.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("%s\n", argv[1]);
  printf("%s\n", get_apikey());
  printf("%s\n", list_hosts());

  return 0;
}
