#ifndef __GRAFANA_MODULE_H
#define __GRAFANA_MODULE_H

#include <chrono>
#include <string>


class GrafanaModule {

  public:
    GrafanaModule(const std::string & endpoint);
    ~GrafanaModule();
    double getCurrentOutdoorTemperature() const;
    double getOutdoorPm10() const;
    double getOutdoorPm25() const;

  private:
    CURL * curlCtx = NULL;
    std::string grafanaUrlEndpoint;


};

#endif

