//
// simple_redis_connection.cpp
// Created by TheFatNinja 
// 08-10-2019
//

#include "standard_redis_connection.h"

StandardRedisConnection::StandardRedisConnection(const std::string &host, const int &port) {
    this->host = host;
    this->port = port;
}

void StandardRedisConnection::Connect() {
    if (this->context == nullptr) {
        this->context = redisConnect(this->host.c_str(), this->port);
        redisEnableKeepAlive(this->context);
    }
}

void StandardRedisConnection::Disconnect() {
    if (this->context) {
        redisFree(this->context);
        this->context = nullptr;
    }
}

bool StandardRedisConnection::IsConnected() {
    return this->context != nullptr;
}

RedisConnectionType StandardRedisConnection::Type() {
    return RedisStandardConnection;
}

std::vector<std::string> StandardRedisConnection::KEYS(const std::string &search_string) {
    std::string command = "KEYS " + search_string;
    auto *reply = static_cast<redisReply*>(redisCommand(this->context, command.c_str()));
    std::vector<std::string> keys;
    if (!reply) {
        // TODO: throw not connected exception
        return {};
    }
    for (size_t i = 0; i < reply->elements; ++i) {
        redisReply *element_ptr = reply->element[i];
        std::string key = element_ptr->str;
        keys.emplace_back(key);
    }
    return keys;
}

std::vector<std::string> StandardRedisConnection::SCAN(const std::string &search_string, const KeyType &type) {
    int cursor = 0;
    std::string command_end = "MATCH " + search_string;
    if (type != KeyTypeAny) {
        std::string key_type = KeyTypeName[type];
        command_end = command_end + " TYPE " + key_type;
    }
    std::vector<std::string> keys;
    while(true) {
        std::string command = "SCAN " + std::to_string(cursor) + " " + command_end;
        auto *reply = static_cast<redisReply *>(redisCommand(this->context, command.c_str()));
        if (!reply) {
            // TODO: throw not connected exception
            break;
        }
        if (reply->elements == 2) {
            redisReply *cursor_ptr = reply->element[0];
            redisReply *data_ptr = reply->element[1];
            for (size_t i = 0; i < data_ptr->elements; ++i) {
                redisReply *element_ptr = data_ptr->element[i];
                std::string key = element_ptr->str;
                keys.emplace_back(key);
            }
            cursor = std::stoi(cursor_ptr->str);
            if (cursor == 0) {
                break;
            }
        }
    }
    return keys;
}

// TODO: Here down

KeyType StandardRedisConnection::TYPE(const std::string &key) {
    std::string command = "TYPE " + key;
    auto *reply = static_cast<redisReply *>(redisCommand(this->context, command.c_str()));
    return KeyTypeAny;
}

std::string StandardRedisConnection::GET(const std::string &key) {
    return "";
}

bool StandardRedisConnection::SET(const std::string &key, const std::string &value) {
    return false;
}

int StandardRedisConnection::DEL(const std::string &key) {
    return 0;
}

int StandardRedisConnection::DEL(const std::vector<std::string> &keys) {
    int count = 0;
    for (auto &key: keys) {
        count += DEL(key);
    }
    return count;
}

std::vector<std::string> StandardRedisConnection::LRANGE(const std::string &key, const int &first, const int &last) {
    return {};
}

int StandardRedisConnection::LPUSH(const std::string &key, const std::string &value) {
    return 0;
}

int StandardRedisConnection::LPUSHX(const std::string &key, const std::string &value) {
    return 0;
}

int StandardRedisConnection::LSET(const std::string &key, const int &index, const std::string &value) {
    return 0;
}

int StandardRedisConnection::LINSERT(const std::string &key, bool before, const std::string &reference_value, const std::string &value) {
    return 0;
}

int StandardRedisConnection::RPUSH(const std::string &key, const std::string &value) {
    return 0;
}

int StandardRedisConnection::RPUSHX(const std::string &key, const std::string &value) {
    return 0;
}

int StandardRedisConnection::LREM(const std::string &key, const std::string &value) {
    return 0;
}

std::string StandardRedisConnection::LPOP(const std::string &key) {
    return "";
}

std::string StandardRedisConnection::RPOP(const std::string &key) {
    return "";
}

std::vector<std::string> StandardRedisConnection::SMEMBERS(const std::string &key) {
    return {};
}

std::vector<std::string> StandardRedisConnection::SSCAN(const std::string &key, const std::string &search_string) {
    return {};
}

int StandardRedisConnection::SADD(const std::string &key, const std::string &value) {
    return 0;
}

int StandardRedisConnection::SREM(const std::string &key, const std::string &value) {
    return 0;
}

int StandardRedisConnection::SREM(const std::string &key, const std::vector<std::string> &values) {
    int count = 0;
    for (auto &value: values) {
        count += SREM(key, value);
    }
    return count;
}

std::vector<std::string> StandardRedisConnection::ZRANGE(const std::string &key, const int &start, const int &end) {
    return {};
}

std::map<std::string, double> StandardRedisConnection::ZSCAN(const std::string &key, const std::string &search_string) {
    return {};
}

int StandardRedisConnection::ZADD(const std::string &key, const double &score, const std::string &value) {
    return 0;
}

int StandardRedisConnection::ZREM(const std::string &key, const std::string &value) {
    return 0;
}

int StandardRedisConnection::ZREM(const std::string &key, const std::vector<std::string> &values) {
    int count = 0;
    for (auto& value: values) {
        count += ZREM(key, value);
    }
    return count;
}

std::vector<std::string> StandardRedisConnection::HKEYS(const std::string &key) {
    return {};
}

std::map<std::string, std::string> StandardRedisConnection::HSCAN(const std::string &key, const std::string &search_string) {
    return {};
}

std::string StandardRedisConnection::HGET(const std::string &key, const std::string &field) {
    return "";
}

bool StandardRedisConnection::HSET(const std::string &key, const std::string &field, const std::string &value) {
    return false;
}

int StandardRedisConnection::HDEL(const std::string &key, const std::string &field) {
    return 0;
}

int StandardRedisConnection::HDEL(const std::string &key, const std::vector<std::string> &fields) {
    int count = 0;
    for (auto& field: fields) {
        count += HDEL(key, field);
    }
    return count;
}
