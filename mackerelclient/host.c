#include "env.h"
#include <curl/curl.h>
#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *data;
  size_t size;
} MEMFILE;

MEMFILE *memfopen() {
  MEMFILE *mf = (MEMFILE *)malloc(sizeof(MEMFILE));
  if (mf) {
    mf->data = NULL;
    mf->size = 0;
  }
  return mf;
}

void memfclose(MEMFILE *mf) {
  if (mf->data)
    free(mf->data);
  free(mf);
}

size_t memfwrite(char *ptr, size_t size, size_t nmemb, void *stream) {
  MEMFILE *mf = (MEMFILE *)stream;
  int block = size * nmemb;
  if (!mf)
    return block;
  if (!mf->data)
    mf->data = (char *)malloc(block);
  else
    mf->data = (char *)realloc(mf->data, mf->size + block);
  if (mf->data) {
    memcpy(mf->data + mf->size, ptr, block);
    mf->size += block;
  }
  return block;
}

char *memfstrdup(MEMFILE *mf) {
  char *buf;
  if (mf->size == 0)
    return NULL;
  buf = (char *)malloc(mf->size + 1);
  memcpy(buf, mf->data, mf->size);
  buf[mf->size] = 0;
  return buf;
}

char *list_hosts() {
  CURL *curl = curl_easy_init();
  MEMFILE *mf = NULL;
  struct curl_slist *headers = NULL;
  char header[1024];
  char *js = NULL;
  int i;

  mf = memfopen();

  curl_easy_setopt(curl, CURLOPT_URL,
                   "https://api.mackerelio.com/api/v0/hosts");
  sprintf(header, "X-Api-Key: %s", get_apikey());
  headers = curl_slist_append(headers, header);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, mf);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memfwrite);

  curl_easy_perform(curl);
  curl_easy_cleanup(curl);
  curl_slist_free_all(headers);

  js = memfstrdup(mf);
  memfclose(mf);

  json_error_t error;
  json_t *result = json_loads(js, 0, &error);
  if (result == NULL) {
    fputs(error.text, stderr);
    free(js);
  }
  json_t *hosts = json_object_get(result, "hosts");
  json_t *host;
  json_array_foreach(hosts, i, host) {
    printf("%s, %s, %s, %s\n", json_string_value(json_object_get(host, "id")),
           json_string_value(json_object_get(host, "name")),
           json_string_value(json_object_get(host, "memo")),
           json_string_value(json_object_get(host, "status")));
  }

  json_decref(result);
  return 0;
}
