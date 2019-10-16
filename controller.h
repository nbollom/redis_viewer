//
// controller.h
// Created by TheFatNinja 
// 24-09-2019
//

#ifndef REDIS_VIEWER_CONTROLLER_H
#define REDIS_VIEWER_CONTROLLER_H

#include <QObject>
#include "redis_connection.h"

class Controller : public QObject {
    Q_OBJECT

private:
    RedisConnectionPtr connection;

public:
    Controller();

};

#endif //REDIS_VIEWER_CONTROLLER_H
