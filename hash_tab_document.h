//
// hash_tab_document.h
// Created by TheFatNinja 
// 22-11-2019
//

#ifndef REDIS_VIEWER_HASH_TAB_DOCUMENT_H
#define REDIS_VIEWER_HASH_TAB_DOCUMENT_H

#include <QHBoxLayout>
#include <QSplitter>
#include <QTreeWidget>
#include "tab_document.h"
#include "redis_connection.h"
#include "value_editor.h"

class HashTabDocument: public TabDocument {
Q_OBJECT

private:
    const std::string key_name;
    RedisConnectionPtr redis;

    QVBoxLayout layout;
    QSplitter spliter;
    QTreeWidget hash_list;
    ValueEditor value_edit;

    std::string last_field;

    void ItemSelected(QTreeWidgetItem *item, int column);
    void ValueChanged();
    void ChangesUndone();

public:
    explicit HashTabDocument(std::string key, RedisConnectionPtr connection);
    bool CanClose() override;
    bool IsRedisDoc() override;
    std::string Name() override;
    void Save(const std::function<void(bool)> &callback) override;
};

#endif //REDIS_VIEWER_HASH_TAB_DOCUMENT_H
