/**
 * 网络流量测试工具头文件
 */
#include "TrafficFlood.h"

/**
 * 网络请求库
 */
#include <curl/curl.h>

/**
 * 标准库依赖
 */
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

/**
 * 测试模式标志
 */
bool isTest = false;

/**
 * cURL响应数据处理回调函数
 * @param contents 接收到的数据指针
 * @param size 单个数据块大小
 * @param nmemb 数据块数量
 * @param output 输出字符串指针
 * @return 处理的总字节数
 */
size_t WriteCallback(void* contents, size_t size, size_t nmemb,
                     std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

/**
 * 显示加载动画
 * @param message 显示的消息文本
 */
void showSpinner(const std::string& message) {
    static const char spinner[] = "⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏";
    static int i = 0;
    std::cout << "\r" << spinner[i++ % 10] << " " << message << std::flush;
}

/**
 * 清除当前行输出
 */
void clearLine() { std::cout << "\r\033[K" << std::flush; }

/**
 * 流量发送主函数
 * @param url 目标网址
 */
void TrafficFlood::Flood(std::string url) {
    std::string ip = URL_TO_IP(url);

    if (url.find("://") == std::string::npos) {
        url = "http://" + url;
    }

    std::cout << "\033[1m|> 正在发送流量到目标: " << url + " = " + ip
              << "\033[0m" << std::endl;

    int counter = 0;
    if (isTest) {
        while (counter < 3) {
            for (int i = 0; i < 10; i++) {
                showSpinner("第 " + std::to_string(counter + 1) +
                            " 次请求中...");
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            clearLine();
            std::cout << "\033[32m|> 第 " << (counter + 1)
                      << " 次请求开始\033[0m" << std::endl;
            if (HttpRequest(ip) != 0) {
                std::cout << "\033[31m|> 请求失败，停止测试\033[0m"
                          << std::endl;
                break;
            }
            counter++;
        }
    } else {
        while (true) {
            counter++;
            for (int i = 0; i < 10; i++) {
                showSpinner("第 " + std::to_string(counter) + " 次请求中...");
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            clearLine();
            std::cout << "\033[32m|> 第 " << counter << " 次请求开始\033[0m"
                      << std::endl;
            if (HttpRequest(ip) != 0) {
                std::cout << "\033[31m|> 请求失败，停止测试\033[0m"
                          << std::endl;
                break;
            }
        }
    }
}

/**
 * 发送HTTP请求
 * @param url 目标网址
 * @return 0表示成功，1表示失败
 */
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
        std::cerr << "\033[31m|> cURL 请求失败: " << curl_easy_strerror(res)
                  << "\033[0m" << std::endl;

        return 1;
    } else {
        struct curl_slist* headers = NULL;
        res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &headers);

        std::cout << "\n\033[1m|> 响应信息表:\033[0m\n";
        std::cout
            << "\033[36m+-----------------+------------------------+\033[0m\n";
        std::cout
            << "\033[36m| 类型           | 值                     |\033[0m\n";
        std::cout
            << "\033[36m+-----------------+------------------------+\033[0m\n";

        long response_code;

        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        std::cout << "| 状态码         | " << response_code
                  << std::setw(22 - std::to_string(response_code).length())
                  << "|\n";

        char* content_type;

        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
        if (content_type) {
            std::cout << "| 内容类型       | " << content_type
                      << std::setw(22 - strlen(content_type)) << "|\n";
        }

        if (headers) {
            std::cout << "| Cookie         | ";
            struct curl_slist* curr = headers;
            bool first = true;
            while (curr) {
                if (!first) std::cout << "                | ";
                std::cout << curr->data << std::setw(22 - strlen(curr->data))
                          << "|\n";
                first = false;
                curr = curr->next;
            }
            curl_slist_free_all(headers);
        }

        std::cout
            << "\033[36m+-----------------+------------------------+\033[0m\n";

        return 0;
    }

    curl_easy_cleanup(curl);

    return 0;
}

/**
 * 将URL转换为IP地址
 * @param url 输入的URL
 * @return 解析后的IP地址，如果解析失败则返回原始URL
 */
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

bool TrafficFlood::IsURLExist(std::string url) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);           
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);          
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);   // Follow redirects

    CURLcode res = curl_easy_perform(curl);
    
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return false;
    }

    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_cleanup(curl);

    return (response_code >= 200 && response_code < 400);  
}