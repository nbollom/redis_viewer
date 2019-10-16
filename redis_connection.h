//
// redis_connection.h
// Created by TheFatNinja 
// 24-09-2019
//

#ifndef REDIS_VIEWER_REDIS_CONNECTION_H
#define REDIS_VIEWER_REDIS_CONNECTION_H

#include <QObject>
#include <memory>
#include <vector>
#include <map>

typedef enum {
    StandardRedisConnection = 0,
    SentinelRedisConnection = 1
} RedisConnectionType;

typedef enum {
    KeyTypeString,
    KeyTypeList,
    KeyTypeSet,
    KeyTypeZSet,
    KeyTypeHash,
    KeyTypeStream,
    KeyTypeAny
} KeyType;

class RedisConnection {

protected:
    RedisConnection() = default;

public:
    virtual void Connect() = 0;
    virtual void Disconnect() = 0;
    virtual bool IsConnected() = 0;

    virtual RedisConnectionType Type() = 0;

    virtual std::vector<std::string> KEYS(const std::string &search_string) = 0;
    virtual std::vector<std::string> SCAN(const std::string &search_string, const KeyType &type) = 0;
    virtual KeyType TYPE(const std::string &key) = 0;

    // String type methods
    virtual std::string GET(const std::string &key) = 0;
    virtual bool SET(const std::string &key, const std::string &value) = 0;
    virtual int DEL(const std::string &key) = 0;
    virtual int DEL(const std::vector<std::string> &keys) = 0;

    // List type methods
    virtual std::vector<std::string> LRANGE(const std::string &key, const int &first, const int &last) = 0;
    virtual int LPUSH(const std::string &key, const std::string &value) = 0;
    virtual int LPUSHX(const std::string &key, const std::string &value) = 0;
    virtual int LSET(const std::string &key, const int &index, const std::string &value) = 0;
    virtual int LINSERT(const std::string &key, bool before, const std::string &reference_value, const std::string &value) = 0;
    virtual int RPUSH(const std::string &key, const std::string &value) = 0;
    virtual int RPUSHX(const std::string &key, const std::string &value) = 0;
    virtual int LREM(const std::string &key, const std::string &value) = 0;
    virtual std::string LPOP(const std::string &key) = 0;
    virtual std::string RPOP(const std::string &key) = 0;

    // Set type methods
    virtual std::vector<std::string> SMEMBERS(const std::string &key) = 0;
    virtual std::vector<std::string> SSCAN(const std::string &key, const std::string &search_string) = 0;
    virtual int SADD(const std::string &key, const std::string &value) = 0;
    virtual int SREM(const std::string &key, const std::string &value) = 0;
    virtual int SREM(const std::string &key, const std::vector<std::string> &values) = 0;

    // Set type methods
    virtual std::vector<std::string> ZRANGE(const std::string &key, const int &start, const int &end) = 0;
    virtual std::map<std::string, double> ZSCAN(const std::string &key, const std::string &search_string) = 0;
    virtual int ZADD(const std::string &key, const double &score, const std::string &value) = 0;
    virtual int ZREM(const std::string &key, const std::string &value) = 0;
    virtual int ZREM(const std::string &key, const std::vector<std::string> &values) = 0;

    // Hash type methods
    virtual std::vector<std::string> HKEYS(const std::string &key) = 0;
    virtual std::map<std::string, std::string> HSCAN(const std::string &key, const std::string &search_string) = 0;
    virtual std::string HGET(const std::string &key, const std::string &field) = 0;
    virtual bool HSET(const std::string &key, const std::string &field, const std::string &value) = 0;
    virtual int HDEL(const std::string &key, const std::string &field) = 0;
    virtual int HDEL(const std::string &key, const std::vector<std::string> &fields) = 0;
};

typedef std::shared_ptr<RedisConnection> RedisConnectionPtr;

#endif //REDIS_VIEWER_REDIS_CONNECTION_H
