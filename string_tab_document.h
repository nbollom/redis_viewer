//
// string_tab_document.h
// Created by TheFatNinja 
// 29-10-2019
//

#ifndef REDIS_VIEWER_STRING_TAB_DOCUMENT_H
#define REDIS_VIEWER_STRING_TAB_DOCUMENT_H

#include <QVBoxLayout>
#include <QLabel>
#include "tab_document.h"
#include "redis_connection.h"
#include "task_queue.h"
#include "value_editor.h"

class StringTabDocument : public TabDocument {
    Q_OBJECT

private:
    const std::string key_name;
    RedisConnectionPtr redis;

    QVBoxLayout layout;
    ValueEditor value_edit;

    void ValueChanged();
    void ChangesUndone();

public:
    explicit StringTabDocument(std::string key, RedisConnectionPtr connection);
    bool CanClose() override;
    bool IsRedisDoc() override;
    std::string Name() override;
    void Save(const std::function<void(bool)> &callback) override;

};

#endif //REDIS_VIEWER_STRING_TAB_DOCUMENT_H
