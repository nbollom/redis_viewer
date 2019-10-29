//
// redis_exceptions.h
// Created by TheFatNinja 
// 25-10-2019
//

#ifndef REDIS_VIEWER_REDIS_EXCEPTIONS_H
#define REDIS_VIEWER_REDIS_EXCEPTIONS_H

#include <exception>

class RedisException : public std::exception {

public:
    const char* what() const noexcept override {
        return "Unknown redis exception";
    }

};

class RedisConnectionException : RedisException {

public:
    const char* what() const noexcept override {
        return "Failed to connect to redis";
    }
};

class RedisDisconnectedException : public RedisException {

public:
    const char* what() const noexcept override {
        return "Disconnected from redis";
    }

};

class RedisUnknownTypeException : public RedisException {

public:
    const char* what() const noexcept override {
        return "Unknown Key type";
    }
};

#endif //REDIS_VIEWER_REDIS_EXCEPTIONS_H
