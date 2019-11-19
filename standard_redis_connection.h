//
// simple_redis_connection.h
// Created by TheFatNinja 
// 08-10-2019
//

#ifndef REDIS_VIEWER_STANDARD_REDIS_CONNECTION_H
#define REDIS_VIEWER_STANDARD_REDIS_CONNECTION_H

#include <hiredis/hiredis.h>
#include <memory>
#include "redis_connection.h"

class StandardRedisConnection : public RedisConnection {

private:
    std::string host;
    int port;
    redisContext* context = nullptr;

public:
    explicit StandardRedisConnection(const std::string &host, const int &port);

    void Connect() override;
    void Disconnect() override;
    bool IsConnected() override;

    RedisConnectionType Type() override;

    std::vector<std::string> KEYS(const std::string &search_string) override;
    std::vector<std::string> SCAN(const std::string &search_string, const KeyType &type) override;
    KeyType TYPE(const std::string &key) override;

    // String type methods
    std::string GET(const std::string &key) override;
    bool SET(const std::string &key, const std::string &value) override;
    long long int DEL(const std::string &key) override;
    long long int DEL(const std::vector<std::string> &keys) override;

    // List type methods
    std::vector<std::string> LRANGE(const std::string &key, const int &first, const int &last) override;
    long long int LPUSH(const std::string &key, const std::string &value) override;
    long long int LPUSHX(const std::string &key, const std::string &value) override;
    long long int LSET(const std::string &key, const int &index, const std::string &value) override;
    long long int LINSERT(const std::string &key, bool before, const std::string &reference_value, const std::string &value) override;
    long long int RPUSH(const std::string &key, const std::string &value) override;
    long long int RPUSHX(const std::string &key, const std::string &value) override;
    long long int LREM(const std::string &key, const std::string &value) override;
    std::string LPOP(const std::string &key) override;
    std::string RPOP(const std::string &key) override;

    // Set type methods
    std::vector<std::string> SMEMBERS(const std::string &key) override;
    std::vector<std::string> SSCAN(const std::string &key, const std::string &search_string) override;
    long long int SADD(const std::string &key, const std::string &value) override;
    long long int SREM(const std::string &key, const std::string &value) override;
    long long int SREM(const std::string &key, const std::vector<std::string> &values) override;

    // Set type methods
    std::vector<std::string> ZRANGE(const std::string &key, const int &start, const int &end) override;
    std::map<std::string, double> ZSCAN(const std::string &key, const std::string &search_string) override;
    long long int ZADD(const std::string &key, const double &score, const std::string &value) override;
    long long int ZREM(const std::string &key, const std::string &value) override;
    long long int ZREM(const std::string &key, const std::vector<std::string> &values) override;

    // Hash type methods
    std::vector<std::string> HKEYS(const std::string &key) override;
    std::map<std::string, std::string> HSCAN(const std::string &key, const std::string &search_string) override;
    std::string HGET(const std::string &key, const std::string &field) override;
    bool HSET(const std::string &key, const std::string &field, const std::string &value) override;
    long long int HDEL(const std::string &key, const std::string &field) override;
    long long int HDEL(const std::string &key, const std::vector<std::string> &fields) override;
};

typedef std::shared_ptr<StandardRedisConnection> StandardRedisConnectionPtr;

#endif //REDIS_VIEWER_STANDARD_REDIS_CONNECTION_H
