#include <iostream>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <string>

#include <curl/curl.h>
#include <json/reader.h>
#include <glib.h>
#include <glib/gprintf.h>

#include "ourhome.h"
#include "curl_common.h"

/**
 * OurHome class related code
 */

const std::string OurHome::loginUrl = OURHOME_LOGIN_URL;
const std::string OurHome::choresUrl = OURHOME_CHORES_URL;
const std::string OurHome::shoppingUrl = OURHOME_SHOPPING_URL;
const std::string OurHome::logoutUrl = OURHOME_LOGOUT_URL;

OurHome::OurHome() {
  this->curlCtx = curl_easy_init();
}

OurHome::~OurHome() {
  curl_easy_cleanup(this->curlCtx);
}

/**
 * Extract the sessionid token after a successful authentication
 * onto OurHome.
 */
void OurHome::extractSessionId(std::stringbuf * headers) {
  std::stringstream hh(headers->str());
  for (std::string line ; std::getline(hh, line); ) {
    if (line.find("Set-Cookie: sessionid=") != std::string::npos) {
      this->sessionid = line.substr(12, 42);
      return;
    }
  }
}

/**
 * Performs a login onto the OurHome API
 */
void OurHome::login(std::string username, std::string password) {
  struct curl_slist *customHeaders = NULL;
  CURLcode res;
  curl_easy_reset(this->curlCtx);
  customHeaders = curl_slist_append(customHeaders, "Content-Type: application/json");

  std::stringbuf * headerChunk = new std::stringbuf();

  std::string jsonLoginData;
  jsonLoginData.append("{ ");
  jsonLoginData.append("\"email\": \"");
  jsonLoginData.append(username);
  jsonLoginData.append("\", \"password\": \"");
  jsonLoginData.append(password);
  jsonLoginData.append("\"}");

  curl_easy_setopt(this->curlCtx, CURLOPT_URL, OurHome::loginUrl.c_str());
  curl_easy_setopt(this->curlCtx, CURLOPT_HEADERFUNCTION, write_cb);
  curl_easy_setopt(this->curlCtx, CURLOPT_WRITEFUNCTION, discard_cb);
  curl_easy_setopt(this->curlCtx, CURLOPT_HEADERDATA, (void *) headerChunk);
  curl_easy_setopt(this->curlCtx, CURLOPT_POSTFIELDS, jsonLoginData.c_str());
  curl_easy_setopt(this->curlCtx, CURLOPT_HTTPHEADER, customHeaders);

  res = curl_easy_perform(this->curlCtx);

  if(res != CURLE_OK) {
    std::cerr <<  "curl_easy_perform() failed: " <<
              curl_easy_strerror(res) << std::endl;

    curl_slist_free_all(customHeaders);
    delete headerChunk;
    return;
  }

  OurHome::extractSessionId(headerChunk);

  curl_slist_free_all(customHeaders);
  delete headerChunk;
}

/**
 * Logs out from OurHome.
 */
void OurHome::logout() {
  if (this->sessionid.empty()) {
    return;
  }

  // resets the curl context
  curl_easy_reset(this->curlCtx);

  struct curl_slist *customHeaders = NULL;
  std::string cookie;
  cookie.append("Cookie: ");
  cookie.append(this->sessionid);
  customHeaders = curl_slist_append(customHeaders, cookie.c_str());

  CURLcode res;
  curl_easy_reset(this->curlCtx);
  curl_easy_setopt(this->curlCtx, CURLOPT_URL, OurHome::logoutUrl.c_str());
  curl_easy_setopt(this->curlCtx, CURLOPT_HEADERFUNCTION, discard_cb);
  curl_easy_setopt(this->curlCtx, CURLOPT_WRITEFUNCTION, discard_cb);
  curl_easy_setopt(this->curlCtx, CURLOPT_HTTPHEADER, customHeaders);

  res = curl_easy_perform(this->curlCtx);

  if(res != CURLE_OK) {
    std::cerr <<  "curl_easy_perform() failed: " <<
              curl_easy_strerror(res) << std::endl;
    curl_slist_free_all(customHeaders);
    return;
  }

  curl_slist_free_all(customHeaders);
  this->sessionid = "";
  return;
}



/**
 * Resets the CURL context, and performs
 * an authenticated request.
 */
std::string OurHome::doAuthenticatedRequest(const std::string &url) {
  std::string ret;
  // Returns an empty string if not connected
  if (this->sessionid.empty()) {
    return ret;
  }
  // resets the curl context
  curl_easy_reset(this->curlCtx);
  CURLcode res;
  struct curl_slist *customHeaders = NULL;

  std::stringbuf * contentChunk = new std::stringbuf();

  std::string cookie;
  cookie.append("Cookie: ");
  cookie.append(this->sessionid);
  customHeaders = curl_slist_append(customHeaders, cookie.c_str());

  curl_easy_setopt(this->curlCtx, CURLOPT_URL, url.c_str());
  curl_easy_setopt(this->curlCtx, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(this->curlCtx, CURLOPT_WRITEDATA, (void *) contentChunk);
  curl_easy_setopt(this->curlCtx, CURLOPT_HTTPHEADER, customHeaders);

  res = curl_easy_perform(this->curlCtx);

  if(res != CURLE_OK) {
    std::cerr <<  "curl_easy_perform() failed: " <<
              curl_easy_strerror(res) << std::endl;

    curl_slist_free_all(customHeaders);
    delete contentChunk;
    return ret;
  }
  ret = contentChunk->str();

  curl_slist_free_all(customHeaders);
  delete contentChunk;
  return ret;
}

/**
 * Retrieves the planned chores from the OurHome App API.
 *
 * Note: this will discard the upcoming & the done ones.
 *
 * Note2: entries will be transliterated (i.e. 'é' -> 'e'),
 *   as the goal is to display them onto a LCD screen.
 */
std::list<std::string> OurHome::getChores() {
  std::list<std::string> ret;

  std::string retFromApi = doAuthenticatedRequest(OurHome::choresUrl);
  Json::Reader r;
  Json::Value v;
  r.parse(retFromApi, v);
  for (int i = 0 ; i  < v["objects"].size(); i++) {
    std::string desc = v["objects"][i]["description"].asString();
    // List "T": "TO-DO"
    // List "U": "upcoming"
    // List "S": "Done"
    std::string affectedList = v["objects"][i]["list"].asString();
    if (affectedList.compare("T") == 0) {
      char * transliterated = g_str_to_ascii(desc.c_str(), NULL);
      std::string downgraded(transliterated);
      ret.push_back(downgraded);
      free(transliterated);
    }
  }

  return ret;

}

/**
 * Retrieves the shopping list from the OurHome API.
 *
 * Note: only items with "added_to_current_house_list" set to true
 * are listed.
 * Note2: As the previous method, items are transliterated.
 */
std::list<std::string> OurHome::getShoppingList() {
  std::list<std::string> ret;
  std::string retFromApi = doAuthenticatedRequest(OurHome::shoppingUrl);
  Json::Reader r;
  Json::Value v;
  r.parse(retFromApi, v);
  for (int i = 0 ; i  < v["objects"].size(); i++) {
    std::string name = v["objects"][i]["name"].asString();
    if (v["objects"][i]["added_to_current_house_list"].asBool()) {
      char * transliterated = g_str_to_ascii(name.c_str(), NULL);
      std::string downgraded(transliterated);
      ret.push_back(downgraded);
      free(transliterated);
    }
  }
  return ret;
}
