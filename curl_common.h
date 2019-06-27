#ifndef __CURL_COMMON_H
#define __CURL_COMMON_H
#include <string>
#include <sstream>

#include <curl/curl.h>

#include <stddef.h>


size_t
write_cb(void *contents, size_t size, size_t nmemb, void *userp);


size_t
discard_cb(void *contents, size_t size, size_t nmemb, void *userp);


#endif

