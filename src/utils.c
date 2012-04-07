/*
 * =====================================================================================
 *
 *       Filename:  utils.c
 *
 *    Description:  utils for test
 *
 *        Version:  1.0
 *        Created:  03/31/2012 10:26:26 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YueYang (feinno), yueyang@feinno.com
 *        Company:  FCT
 *
 * =====================================================================================
 */

#include "utils.h"

void *
xmalloc(size_t size) {
  void *rv;
  if(0 == size)
    return NULL;
  rv = malloc (size);
  if(NULL == rv) {
    die("out-of-memory allocating %u bytes", (unsigned) size);
    return rv;
  }
  memset(rv, 0, size);
  return rv;
}

void *
xrealloc(void *a, size_t size) {
  void *rv;
  if(0 == size)
    {
      free (a);
      return NULL;
    }
  if(NULL ==  a)
    return xmalloc(size);
  rv = realloc(a, size);
  if(NULL == rv)
    die("out-of-memory re-allocating %u bytes", (unsigned) size);
  return rv;
}

void
xfree(void *p) {
    if(p != NULL) {
        free(p);
        p = NULL;
    }
}

char *
xstrdup(const char *str) {
  if(NULL == str)
    return NULL;
  /*return strdup(str);*/
  return strcpy (xmalloc (strlen (str) + 1), str);
}

void
die(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fprintf(stderr, "\n");
  exit(1);
}

int
http_post_pb(char *buf, size_t size, char *post_url) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL,
                post_url);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, size);

        res = curl_easy_perform(curl);
    }

    if(res != 0) {
        die("post error!\n");
        return -1;
    }

    curl_easy_cleanup(curl);

    return 0;
}

