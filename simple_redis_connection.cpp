//
// simple_redis_connection.cpp
// Created by TheFatNinja 
// 08-10-2019
//

#include "simple_redis_connection.h"

SimpleRedisConnection::SimpleRedisConnection(const std::string &host, const int &port) {
    this->host = host;
    this->port = port;
}

// TODO: Here down

void SimpleRedisConnection::Connect() {
    if (this->context == nullptr) {

    }
}

void SimpleRedisConnection::Disconnect() {

}

bool SimpleRedisConnection::IsConnected() {
    return false;
}

RedisConnectionType SimpleRedisConnection::Type() {
    return StandardRedisConnection;
}

std::vector<std::string> SimpleRedisConnection::KEYS(const std::string &search_string) {
    return {};
}

std::vector<std::string> SimpleRedisConnection::SCAN(const std::string &search_string, const KeyType &type) {
    return {};
}

KeyType SimpleRedisConnection::TYPE(const std::string &key) {
    return KeyTypeAny;
}

std::string SimpleRedisConnection::GET(const std::string &key) {
    return "";
}

bool SimpleRedisConnection::SET(const std::string &key, const std::string &value) {
    return false;
}

int SimpleRedisConnection::DEL(const std::string &key) {
    return 0;
}

int SimpleRedisConnection::DEL(const std::vector<std::string> &keys) {
    int count = 0;
    for (auto &key: keys) {
        count += DEL(key);
    }
    return count;
}

std::vector<std::string> SimpleRedisConnection::LRANGE(const std::string &key, const int &first, const int &last) {
    return {};
}

int SimpleRedisConnection::LPUSH(const std::string &key, const std::string &value) {
    return 0;
}

int SimpleRedisConnection::LPUSHX(const std::string &key, const std::string &value) {
    return 0;
}

int SimpleRedisConnection::LSET(const std::string &key, const int &index, const std::string &value) {
    return 0;
}

int SimpleRedisConnection::LINSERT(const std::string &key, bool before, const std::string &reference_value, const std::string &value) {
    return 0;
}

int SimpleRedisConnection::RPUSH(const std::string &key, const std::string &value) {
    return 0;
}

int SimpleRedisConnection::RPUSHX(const std::string &key, const std::string &value) {
    return 0;
}

int SimpleRedisConnection::LREM(const std::string &key, const std::string &value) {
    return 0;
}

std::string SimpleRedisConnection::LPOP(const std::string &key) {
    return "";
}

std::string SimpleRedisConnection::RPOP(const std::string &key) {
    return "";
}

std::vector<std::string> SimpleRedisConnection::SMEMBERS(const std::string &key) {
    return {};
}

std::vector<std::string> SimpleRedisConnection::SSCAN(const std::string &key, const std::string &search_string) {
    return {};
}

int SimpleRedisConnection::SADD(const std::string &key, const std::string &value) {
    return 0;
}

int SimpleRedisConnection::SREM(const std::string &key, const std::string &value) {
    return 0;
}

int SimpleRedisConnection::SREM(const std::string &key, const std::vector<std::string> &values) {
    int count = 0;
    for (auto &value: values) {
        count += SREM(key, value);
    }
    return count;
}

std::vector<std::string> SimpleRedisConnection::ZRANGE(const std::string &key, const int &start, const int &end) {
    return {};
}

std::map<std::string, double> SimpleRedisConnection::ZSCAN(const std::string &key, const std::string &search_string) {
    return {};
}

int SimpleRedisConnection::ZADD(const std::string &key, const double &score, const std::string &value) {
    return 0;
}

int SimpleRedisConnection::ZREM(const std::string &key, const std::string &value) {
    return 0;
}

int SimpleRedisConnection::ZREM(const std::string &key, const std::vector<std::string> &values) {
    int count = 0;
    for (auto& value: values) {
        count += ZREM(key, value);
    }
    return count;
}

std::vector<std::string> SimpleRedisConnection::HKEYS(const std::string &key) {
    return {};
}

std::map<std::string, std::string> SimpleRedisConnection::HSCAN(const std::string &key, const std::string &search_string) {
    return {};
}

std::string SimpleRedisConnection::HGET(const std::string &key, const std::string &field) {
    return "";
}

bool SimpleRedisConnection::HSET(const std::string &key, const std::string &field, const std::string &value) {
    return false;
}

int SimpleRedisConnection::HDEL(const std::string &key, const std::string &field) {
    return 0;
}

int SimpleRedisConnection::HDEL(const std::string &key, const std::vector<std::string> &fields) {
    int count = 0;
    for (auto& field: fields) {
        count += HDEL(key, field);
    }
    return count;
}
