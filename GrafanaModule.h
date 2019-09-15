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
    std::string grafanaUrlEndpoint;


};

#endif

