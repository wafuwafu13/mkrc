#include "mackerelclient/env.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Buffer {
  char *data;
  int data_size;
};

size_t buffer_writer(char *ptr, size_t size, size_t nmemb, void *stream) {
  struct Buffer *buf = (struct Buffer *)stream;
  int block = size * nmemb;
  if (!buf) {
    return block;
  }

  if (!buf->data) {
    buf->data = (char *)malloc(block);
  } else {
    buf->data = (char *)realloc(buf->data, buf->data_size + block);
  }

  if (buf->data) {
    memcpy(buf->data + buf->data_size, ptr, block);
    buf->data_size += block;
  }

  return block;
}

int main(int argc, char *argv[]) {
  printf("%s\n", argv[1]);
  printf("%s\n", get_apikey());

  CURL *curl = curl_easy_init();
  struct Buffer *buf;
  struct curl_slist *headers = NULL;
  char header[1024];

  buf = (struct Buffer *)malloc(sizeof(struct Buffer));
  buf->data = NULL;
  buf->data_size = 0;

  curl_easy_setopt(curl, CURLOPT_URL,
                   "https://api.mackerelio.com/api/v0/hosts");

  sprintf(header, "X-Api-Key: %s", get_apikey());
  headers = curl_slist_append(headers, header);

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, buffer_writer);

  curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  printf("%s\n", buf->data);

  free(buf->data);
  free(buf);
  curl_slist_free_all(headers);

  return 0;
}
