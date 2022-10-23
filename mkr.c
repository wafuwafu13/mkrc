#include "mackerelclient/env.h"
#include <curl/curl.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("%s\n", argv[1]);
  printf("%s\n", get_apikey());

  CURL *curl;
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, "https://example.com/");

  curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  return 0;
}
