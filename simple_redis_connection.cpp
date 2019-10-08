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

void SimpleRedisConnection::Connect() {
    if (this->context == nullptr) {

    }
}
