//
// redis_result.h
// Created by TheFatNinja 
// 24-09-2019
//

#ifndef REDIS_VIEWER_REDIS_RESULT_H
#define REDIS_VIEWER_REDIS_RESULT_H

#include <QObject>
#include <memory>

class RedisResult : public QObject {
    Q_OBJECT
    // TODO: implementation
};

typedef std::shared_ptr<RedisResult> ResultPtr;

#endif //REDIS_VIEWER_REDIS_RESULT_H
