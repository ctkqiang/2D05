#include "TrafficFlood.h"

#include <curl/curl.h>

#include <iostream>

size_t WriteCallback(void* contents, size_t size, size_t nmemb,
                     std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);

    return totalSize;
}

void TrafficFlood::Flood(std::string url) {
    std::string ip = URL_TO_IP(url);

    if (url.find("://") == std::string::npos) {
        url = "http://" + url;
    }

    std::cout << "|> 正在发送流量到目标: " << url + " = " + ip << std::endl;

    for (int i = 0; i < 3; i++) HttpRequest(ip);
}

int TrafficFlood::HttpRequest(std::string url) {
    CURL* curl = curl_easy_init();

    if (!curl) {
        std::cerr << "cURL 初始化失败！" << std::endl;
        return 1;
    }

    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "|> cURL 请求失败: " << curl_easy_strerror(res)
                  << std::endl;
    } else {
        std::cout << "|> 响应内容:\n" << response << std::endl;
    }

    curl_easy_cleanup(curl);

    return 0;
}

std::string TrafficFlood::URL_TO_IP(std::string url) {
    CURL* curl = curl_easy_init();
    struct curl_slist* dns;
    std::string ip;

    if (curl) {
        size_t pos = url.find("://");

        if (pos != std::string::npos) {
            url = url.substr(pos + 3);
        }

        pos = url.find("/");
        if (pos != std::string::npos) {
            url = url.substr(0, pos);
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_RESOLVE, &dns);

        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            char* ip_address;
            res = curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &ip_address);

            if (res == CURLE_OK && ip_address) {
                ip = std::string(ip_address);
            }
        }

        curl_easy_cleanup(curl);
    }

    return ip.empty() ? url : ip;
}