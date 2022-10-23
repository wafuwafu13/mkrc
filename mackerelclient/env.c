#include <stdio.h>
#include <stdlib.h>

char* get_apikey()
{
  char * api_key = getenv("MACKEREL_API_KEY");
  if (api_key != NULL)
  {
    return api_key;
  }
  else
  {
    return "";
  }
}
