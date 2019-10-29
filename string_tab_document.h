//
// string_tab_document.h
// Created by TheFatNinja 
// 29-10-2019
//

#ifndef REDIS_VIEWER_STRING_TAB_DOCUMENT_H
#define REDIS_VIEWER_STRING_TAB_DOCUMENT_H

#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include "tab_document.h"
#include "redis_connection.h"
#include "task_queue.h"

class StringTabDocument : public TabDocument {
    Q_OBJECT

private:
    const std::string key_name;
    std::string value;
    RedisConnectionPtr redis;
    TaskQueuePtr queue;

    QVBoxLayout layout;
    QLabel key_label;
    QTextEdit value_edit;

public:
    explicit StringTabDocument(std::string key, RedisConnectionPtr connection, TaskQueuePtr task_queue);
    bool CanClose() override;
    bool IsRedisDoc() override;
    std::string Name() override;

};

#endif //REDIS_VIEWER_STRING_TAB_DOCUMENT_H
