/**
 * 用户协议管理模块
 *
 * 功能说明：
 * 1. 管理用户协议的接受状态
 * 2. 使用SQLite数据库存储协议状态
 * 3. 提供协议展示和确认功能
 *
 * 数据库结构：
 * - 表名：agreement
 * - 字段：agreed (INTEGER)
 * - 默认值：0（未同意）
 */
#include "Agreement.h"

#include <sqlite3.h>

#include <iostream>

const char* Agreement::DATABASE_PATH = "database/2d05.db";

bool Agreement::InitDatabase() {
    sqlite3* database;

    int rc = sqlite3_open(DATABASE_PATH, &database);

    if (rc) return false;

    const char* query =
        "CREATE TABLE IF NOT EXISTS agreement ("
        "agreed INTEGER DEFAULT 0"
        ");";

    char* errMsg = 0;
    rc = sqlite3_exec(database, query, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        sqlite3_free(errMsg);
        sqlite3_close(database);

        return false;
    }

    query = "INSERT OR IGNORE INTO agreement (agreed) VALUES (0);";
    rc = sqlite3_exec(database, query, 0, 0, &errMsg);

    sqlite3_close(database);

    return rc == SQLITE_OK;
}

bool Agreement::HasUserAgreed() {
    sqlite3* db;
    sqlite3_stmt* stmt;

    bool agreed = false;

    sqlite3_open(DATABASE_PATH, &db);

    const char* sql = "SELECT agreed FROM agreement LIMIT 1;";

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            agreed = sqlite3_column_int(stmt, 0) == 1;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return agreed;
}

bool Agreement::SetUserAgreed() {
    sqlite3* db;
    char* errMsg = 0;

    sqlite3_open(DATABASE_PATH, &db);

    const char* sql = "UPDATE agreement SET agreed = 1;";

    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);

    return rc == SQLITE_OK;
}

void Agreement::ShowAgreement() {
    std::cout << "\033[1;31m重要声明\033[0m\n";
    std::cout << "═══════════════════════════════════════════\n\n";
    std::cout << "本软件仅供合法用途，任何滥用均由用户自行承担责任，作者概不负"
                 "责。\n\n";
    std::cout << "根据《中华人民共和国网络安全法》《中华人民共和国刑法》\n";
    std::cout
        << "《计算机信息系统安全保护条例》等法律，未经授权入侵计算机系统、\n";
    std::cout
        << "传播恶意软件、窃取数据、进行网络诈骗、攻击关键信息基础设施等\n";
    std::cout
        << "行为均属违法，违法者可被判处最高七年以上有期徒刑，并处以罚金\n";
    std::cout << "或财产没收。\n\n";
    std::cout << "本软件不提供任何担保，使用风险由用户自行承担。\n\n";
    std::cout
        << "下载、安装、使用本软件即表示您已阅读、理解并同意本免责声明，\n";
    std::cout << "如不同意，请立即删除本软件并停止使用。\n\n";
    std::cout << "═══════════════════════════════════════════\n";
}