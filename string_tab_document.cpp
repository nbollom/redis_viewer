//
// string_tab_document.cpp
// Created by TheFatNinja 
// 29-10-2019
//

#include "string_tab_document.h"

#include <utility>

StringTabDocument::StringTabDocument(std::string key, RedisConnectionPtr connection, TaskQueuePtr task_queue) :
            key_name(std::move(key)), redis(std::move(connection)), queue(std::move(task_queue)) {
    has_changes = false;
    setLayout(&layout);

    layout.addWidget(&key_label);
    layout.addWidget(&value_edit);

    QString key_text = QString::fromStdString(key_name);
    key_label.setText(key_text);

    queue->RunTask<std::string>([this](){
        return redis->GET(key_name);
    },[this](std::string &value_text, const std::string &error) {
        QString data = QString::fromStdString(value_text);
        value_edit.setText(data);
    });
}

bool StringTabDocument::CanClose() {
    return !has_changes;
}

bool StringTabDocument::IsRedisDoc() {
    return true;
}

std::string StringTabDocument::Name() {
    return key_name;
}
