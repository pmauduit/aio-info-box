#include "curl_common.h"

/**
 * Callback used when calling CURL to get the actual
 * data from the remote server.
 */

size_t
write_cb(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  std::stringbuf *mem = (std::stringbuf *) userp;

  mem->sputn((char *) contents, realsize);

  return realsize;
}

/**
 * Callback used with cURL to just ignore the arriving content.
 */
size_t
discard_cb(void *contents, size_t size, size_t nmemb, void *userp)
{
  return size * nmemb;
}


