#ifndef __GRAFANA_MODULE_H
#define __GRAFANA_MODULE_H

#include <chrono>
#include <string>


class GrafanaModule {

  public:
    GrafanaModule(const std::string & endpoint);
    ~GrafanaModule();
    double getCurrentOutdoorTemperature() const;
    double getCurrentOutdoorPm10() const;
    double getCurrentOutdoorPm25() const;
    double getYesterdayElectricConsumption() const;

  private:
    CURL * curlCtx = NULL;
    std::string grafanaUrlEndpoint; // /api/datasources/proxy/1/query?db=collectd&epoch=ms&q="
    std::string grafanaTsdbQueryEndpoint; // https://metrics.spironet.fr/api/tsdb/query


};

#endif

