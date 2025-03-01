#ifndef TRAFFICFLOOD_H
#define TRAFFICFLOOD_H

#include <string>

class TrafficFlood {
   public:
    void Flood(std::string url);
    int HttpRequest(std::string url);

    std::string URL_TO_IP(std::string url);
};

#endif