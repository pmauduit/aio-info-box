#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

#include <curl/curl.h>
#include <json/reader.h>
#include <json/writer.h>

#include "GrafanaModule.h"
#include "curl_common.h"
/**
 * A module which queries a remote Grafana instance
 * to collect metrics to be printed afterwards.
 *
 */

GrafanaModule::GrafanaModule(const std::string &grafanaEndpoint):
  grafanaUrlEndpoint(grafanaEndpoint + "api/datasources/proxy/1/query?db=collectd&epoch=ms&q="),
  grafanaTsdbQueryEndpoint(grafanaEndpoint + "api/tsdb/query") {
  this->curlCtx = curl_easy_init();
}

GrafanaModule::~GrafanaModule() {
  curl_easy_cleanup(this->curlCtx);
}

/**
 * Sample query:
 * https://metrics.spironet.fr/api/datasources/proxy/1/query?db=collectd
 * &q=SELECT%20mean(%22value%22)%20FROM%20%22exec_value%22%20WHERE%20(%22type%22%20%3D%20%27temperature%27)
 *    %20AND%20time%20%3E%3D%201561637455000ms%20and%20time%20%3C%3D%201561637465000ms%20GROUP%20BY%20time(1m)%20fill(linear)&epoch=ms
 *
 * urldecoded query:
 * SELECT mean("value") FROM "exec_value" WHERE ("type" = 'temperature') AND time >= 1561637455000ms and time <= 1561637465000ms GROUP BY time(1m) fill(linear)
 *
 */
double GrafanaModule::getCurrentOutdoorTemperature() const {
  double ret = 0;

  curl_easy_reset(this->curlCtx);
  CURLcode result;

  std::stringbuf * contentChunk = new std::stringbuf();

  double currentTimestamp = std::chrono::duration_cast< std::chrono::milliseconds >(
      std::chrono::system_clock::now().time_since_epoch()
  ).count();

  std::stringstream query, fullUrl ;
  query << "SELECT mean(value) FROM \"exec_value\" WHERE (\"type\" = 'temperature') AND time >= "
     << (long long) (currentTimestamp - 60000) << "ms and time <= " << (long long) currentTimestamp <<
      "ms GROUP BY time(1m) fill(linear)";
  char * escapedQuery = curl_easy_escape(this->curlCtx, query.str().c_str(), query.str().length());
  fullUrl << this->grafanaUrlEndpoint << escapedQuery;
  free(escapedQuery);

  curl_easy_setopt(this->curlCtx, CURLOPT_URL, fullUrl.str().c_str());
  curl_easy_setopt(this->curlCtx, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(this->curlCtx, CURLOPT_WRITEDATA, (void *) contentChunk);

  result = curl_easy_perform(this->curlCtx);

  if(result != CURLE_OK) {
    std::cerr <<  "curl_easy_perform() failed: " <<
              curl_easy_strerror(result) << std::endl;

    delete contentChunk;
    return ret;
  }

  // Parses the received JSON payload
  Json::Reader r;
  Json::Value v;
  r.parse(contentChunk->str(), v);

  int i = 0;
  for (auto const & value : v["results"][0]["series"][0]["values"]) {
    if (value[1].type() == Json::nullValue)
      continue;
    double dbl = value[1].asDouble();
    ret += value[1].asDouble();
    i++;
  }
  ret /= i;

  delete contentChunk;
  return ret;
}

double GrafanaModule::getCurrentOutdoorPm10() const {
  double ret = 0;

  curl_easy_reset(this->curlCtx);
  CURLcode result;

  std::stringbuf * contentChunk = new std::stringbuf();

  double currentTimestamp = std::chrono::duration_cast< std::chrono::milliseconds >(
      std::chrono::system_clock::now().time_since_epoch()
  ).count();

  std::stringstream query, fullUrl ;
  query << "SELECT mean(value) FROM \"exec_value\" WHERE (\"type_instance\" = 'sds011_pm10') AND time >= "
     << (long long) (currentTimestamp - 60000) << "ms and time <= " << (long long) currentTimestamp <<
      "ms GROUP BY time(1m) fill(linear)";
  char * escapedQuery = curl_easy_escape(this->curlCtx, query.str().c_str(), query.str().length());
  fullUrl << this->grafanaUrlEndpoint << escapedQuery;
  free(escapedQuery);

  curl_easy_setopt(this->curlCtx, CURLOPT_URL, fullUrl.str().c_str());
  curl_easy_setopt(this->curlCtx, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(this->curlCtx, CURLOPT_WRITEDATA, (void *) contentChunk);

  result = curl_easy_perform(this->curlCtx);

  if(result != CURLE_OK) {
    std::cerr <<  "curl_easy_perform() failed: " <<
              curl_easy_strerror(result) << std::endl;

    delete contentChunk;
    return ret;
  }

  // Parses the received JSON payload
  Json::Reader r;
  Json::Value v;
  r.parse(contentChunk->str(), v);

  int i = 0;
  for (auto const & value : v["results"][0]["series"][0]["values"]) {
    if (value[1].type() == Json::nullValue)
      continue;
    double dbl = value[1].asDouble();
    ret += value[1].asDouble();
    i++;
  }
  ret /= i;

  delete contentChunk;
  return ret;
}

double GrafanaModule::getCurrentOutdoorPm25() const {
  double ret = 0;

  curl_easy_reset(this->curlCtx);
  CURLcode result;

  std::stringbuf * contentChunk = new std::stringbuf();

  double currentTimestamp = std::chrono::duration_cast< std::chrono::milliseconds >(
      std::chrono::system_clock::now().time_since_epoch()
  ).count();

  std::stringstream query, fullUrl ;
  query << "SELECT mean(value) FROM \"exec_value\" WHERE (\"type_instance\" = 'sds011_pm25') AND time >= "
     << (long long) (currentTimestamp - 60000) << "ms and time <= " << (long long) currentTimestamp <<
      "ms GROUP BY time(1m) fill(linear)";
  char * escapedQuery = curl_easy_escape(this->curlCtx, query.str().c_str(), query.str().length());
  fullUrl << this->grafanaUrlEndpoint << escapedQuery;
  free(escapedQuery);

  curl_easy_setopt(this->curlCtx, CURLOPT_URL, fullUrl.str().c_str());
  curl_easy_setopt(this->curlCtx, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(this->curlCtx, CURLOPT_WRITEDATA, (void *) contentChunk);

  result = curl_easy_perform(this->curlCtx);

  if(result != CURLE_OK) {
    std::cerr <<  "curl_easy_perform() failed: " <<
              curl_easy_strerror(result) << std::endl;

    delete contentChunk;
    return ret;
  }

  // Parses the received JSON payload
  Json::Reader r;
  Json::Value v;
  r.parse(contentChunk->str(), v);

  int i = 0;
  for (auto const & value : v["results"][0]["series"][0]["values"]) {
    if (value[1].type() == Json::nullValue)
      continue;
    double dbl = value[1].asDouble();
    ret += value[1].asDouble();
    i++;
  }
  ret /= i;

  delete contentChunk;
  return ret;
}

double GrafanaModule::getYesterdayElectricConsumption() const {
  double ret = 0;
  struct curl_slist *customHeaders = NULL;

  curl_easy_reset(this->curlCtx);
  CURLcode result;
  customHeaders = curl_slist_append(customHeaders, "Content-Type: application/json");

  std::stringbuf * contentChunk = new std::stringbuf();

  double currentTimestamp = std::chrono::duration_cast< std::chrono::milliseconds >(
      std::chrono::system_clock::now().time_since_epoch()
  ).count();

  Json::Value root;
  root["from"] = std::to_string((long long) (currentTimestamp - (24 * 3600 * 1000)));
  root["to"] = std::to_string((long long) currentTimestamp);
  Json::Value query;
  query["rawSql"] = "SELECT val_date AS \"time\", name AS metric, kwh FROM electric_consumption WHERE $__timeFilter(val_date) ORDER BY 1,2";
  query["refId"] = "A";
  query["intervalMs"] = 900000;
  query["maxDataPoints"] = 100;
  query["datasourceId"] = 2;
  query["format"] = "time_series";
  root["queries"].append(query);

  Json::FastWriter fastwriter;
  std::string message = fastwriter.write(root);

  curl_easy_setopt(this->curlCtx, CURLOPT_URL, this->grafanaTsdbQueryEndpoint.c_str());
  curl_easy_setopt(this->curlCtx, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(this->curlCtx, CURLOPT_WRITEDATA, (void *) contentChunk);
  curl_easy_setopt(this->curlCtx, CURLOPT_POSTFIELDS, message.c_str());
  curl_easy_setopt(this->curlCtx, CURLOPT_HTTPHEADER, customHeaders);

  result = curl_easy_perform(this->curlCtx);

  if(result != CURLE_OK) {
    std::cerr <<  "curl_easy_perform() failed: " <<
              curl_easy_strerror(result) << std::endl;

    delete contentChunk;
    curl_slist_free_all(customHeaders);
    return ret;
  }

  // Parses the received JSON payload
  Json::Reader r;
  Json::Value v;
  r.parse(contentChunk->str(), v);

  int i = 0;
  auto const & value = v["results"]["A"]["series"][0]["points"][0];
  if (value.type() == Json::nullValue) {
      ret = 0;
  } else {
    ret = value[0].asDouble();
  }

  delete contentChunk;
  curl_slist_free_all(customHeaders);
  return ret;
}

