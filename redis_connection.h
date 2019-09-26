//
// redis_connection.h
// Created by TheFatNinja 
// 24-09-2019
//

#ifndef REDIS_VIEWER_REDIS_CONNECTION_H
#define REDIS_VIEWER_REDIS_CONNECTION_H

#include <QObject>
#include <functional>
#include <memory>
#include "redis_result.h"

class RedisConnection : public QObject {
    Q_OBJECT

public:
    virtual void Connect() = 0;
    virtual void Disconnect() = 0;
    virtual bool IsConnected() = 0;

    virtual void Read(const std::string& command, std::function<void(ResultPtr)> callback) = 0;
    virtual void Write(const std::string& command, std::function<void(int)> callback) = 0;
};

typedef std::shared_ptr<RedisConnection> ConnectionPtr;

#endif //REDIS_VIEWER_REDIS_CONNECTION_H
