//
// simple_redis_connection.cpp
// Created by TheFatNinja 
// 08-10-2019
//

#include "standard_redis_connection.h"
#include "redis_exceptions.h"

StandardRedisConnection::StandardRedisConnection(const std::string &host, const int &port) {
    this->host = host;
    this->port = port;
}

void StandardRedisConnection::Connect() {
    if (this->context == nullptr) {
        this->context = redisConnect(this->host.c_str(), this->port);
        auto reply = static_cast<redisReply*>(redisCommand(this->context, "PING"));
        if (reply) {
            redisEnableKeepAlive(this->context);
            freeReplyObject(reply);
        }
        else {
            Disconnect();
            throw RedisConnectionException();
        }
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
    auto *reply = static_cast<redisReply*>(redisCommand(this->context, "KEYS %b", search_string.data(), search_string.size()));
    std::vector<std::string> keys;
    if (!reply) {
        Disconnect();
        throw RedisDisconnectedException();
    }
    for (size_t i = 0; i < reply->elements; ++i) {
        redisReply *element_ptr = reply->element[i];
        std::string key = element_ptr->str;
        keys.emplace_back(key);
    }
    freeReplyObject(reply);
    return keys;
}

std::vector<std::string> StandardRedisConnection::SCAN(const std::string &search_string, const KeyType &type) {
    int cursor = 0;
    std::vector<std::string> keys;
    while(true) {
        redisReply *reply = nullptr;
        if (type != KeyTypeAny) {
            std::string key_type = KeyTypeName[type];
            reply = static_cast<redisReply*>(redisCommand(this->context, "SCAN %d MATCH %b TYPE %b", cursor, search_string.data(), search_string.size(), key_type.data(), key_type.size()));
        }
        else {
            reply = static_cast<redisReply*>(redisCommand(this->context, "SCAN %d MATCH %b", cursor, search_string.data(), search_string.size()));
        }
        if (!reply) {
            Disconnect();
            throw RedisDisconnectedException();
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
        freeReplyObject(reply);
    }
    return keys;
}

KeyType StandardRedisConnection::TYPE(const std::string &key) {
    auto *reply = static_cast<redisReply*>(redisCommand(this->context, "TYPE %b", key.data(), key.size()));
    KeyType type;
    if (reply == nullptr) {
        Disconnect();
        throw RedisDisconnectedException();
    }
    else if (strcmp(reply->str, "string") == 0) {
        type = KeyTypeString;
    }
    else if (strcmp(reply->str, "list") == 0) {
        type = KeyTypeList;
    }
    else if (strcmp(reply->str, "set") == 0) {
        type = KeyTypeSet;
    }
    else if (strcmp(reply->str, "zset") == 0) {
        type = KeyTypeZSet;
    }
    else if (strcmp(reply->str, "hash") == 0) {
        type = KeyTypeHash;
    }
    else {
        throw RedisUnknownTypeException();
    }
    freeReplyObject(reply);
    return type;
}

std::string StandardRedisConnection::GET(const std::string &key) {
    auto *reply = static_cast<redisReply*>(redisCommand(this->context, "GET %b", key.data(), key.size()));
    if (reply == nullptr) {
        Disconnect();
        throw RedisDisconnectedException();
    }
    else {
        std::string val = reply->str;
        freeReplyObject(reply);
        return val;
    }
}

bool StandardRedisConnection::SET(const std::string &key, const std::string &value) {
    auto *reply = static_cast<redisReply*>(redisCommand(this->context, "SET %b %b", key.data(), key.size(), value.data(), value.size()));
    if (reply == nullptr) {
        Disconnect();
        throw RedisDisconnectedException();
    }
    else {
        bool success = strcmp(reply->str, "OK") == 0;
        freeReplyObject(reply);
        return success;
    }
}

long long int StandardRedisConnection::DEL(const std::string &key) {
    auto *reply = static_cast<redisReply*>(redisCommand(this->context, "DEL %b", key.data(), key.size()));
    if (reply == nullptr) {
        Disconnect();
        throw RedisDisconnectedException();
    }
    else {
        long long int val = reply->integer;
        freeReplyObject(reply);
        return val;
    }
}

long long int StandardRedisConnection::DEL(const std::vector<std::string> &keys) {
    long long int count = 0;
    long long int commands = 0;
    for (auto &key: keys) {
        redisAppendCommand(this->context, "DEL %b", key.data(), key.size());
        commands++;
    }
    redisReply *reply;
    for (long long int i = 0; i < commands; ++i) {
        redisGetReply(this->context, (void**)&reply);
        count += reply->integer;
        freeReplyObject(reply);
    }
    return count;
}

// TODO: Here down

std::vector<std::string> StandardRedisConnection::LRANGE(const std::string &key, const int &first, const int &last) {
    return {};
}

long long int StandardRedisConnection::LPUSH(const std::string &key, const std::string &value) {
    return 0;
}

long long int StandardRedisConnection::LPUSHX(const std::string &key, const std::string &value) {
    return 0;
}

long long int StandardRedisConnection::LSET(const std::string &key, const int &index, const std::string &value) {
    return 0;
}

long long int StandardRedisConnection::LINSERT(const std::string &key, bool before, const std::string &reference_value, const std::string &value) {
    return 0;
}

long long int StandardRedisConnection::RPUSH(const std::string &key, const std::string &value) {
    return 0;
}

long long int StandardRedisConnection::RPUSHX(const std::string &key, const std::string &value) {
    return 0;
}

long long int StandardRedisConnection::LREM(const std::string &key, const std::string &value) {
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

long long int StandardRedisConnection::SADD(const std::string &key, const std::string &value) {
    return 0;
}

long long int StandardRedisConnection::SREM(const std::string &key, const std::string &value) {
    return 0;
}

long long int StandardRedisConnection::SREM(const std::string &key, const std::vector<std::string> &values) {
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

long long int StandardRedisConnection::ZADD(const std::string &key, const double &score, const std::string &value) {
    return 0;
}

long long int StandardRedisConnection::ZREM(const std::string &key, const std::string &value) {
    return 0;
}

long long int StandardRedisConnection::ZREM(const std::string &key, const std::vector<std::string> &values) {
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

long long int StandardRedisConnection::HDEL(const std::string &key, const std::string &field) {
    return 0;
}

long long int StandardRedisConnection::HDEL(const std::string &key, const std::vector<std::string> &fields) {
    int count = 0;
    for (auto& field: fields) {
        count += HDEL(key, field);
    }
    return count;
}
