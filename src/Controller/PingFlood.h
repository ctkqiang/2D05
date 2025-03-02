#pragma once

#include <string>

class PingFlood {
   public:
    static bool SendPing(const std::string& TARGET_IP);
    static uint16_t CalculateChecksum(uint16_t* BUFFER, int SIZE);

   private:
    static const int PING_PKT_S = 64;
    static const int PING_SLEEP_RATE = 1000000;
};