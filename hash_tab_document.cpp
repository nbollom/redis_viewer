//
// hash_tab_document.cpp
// Created by TheFatNinja 
// 22-11-2019
//

#include <map>
#include <string>
#include "hash_tab_document.h"
#include "task_queue.h"

HashTabDocument::HashTabDocument(std::string key, RedisConnectionPtr connection) : key_name(std::move(key)), redis(std::move(connection)) {
    has_changes = false;
    setLayout(&layout);

    layout.addWidget(&spliter);

    spliter.addWidget(&hash_list);
    spliter.addWidget(&value_edit);
    spliter.setChildrenCollapsible(false);
    spliter.setOrientation(Qt::Orientation::Vertical);

    hash_list.setColumnCount(2);
    hash_list.setHeaderLabels({"Key", "Value"});
    connect(&hash_list, &QTreeWidget::itemActivated, this, &HashTabDocument::ItemSelected);

    TaskQueuePtr queue = GetTaskQueue();
    queue->RunTask<std::map<std::string, std::string>>([this](){
        return redis->HSCAN(key_name, "*");
    },[this](std::map<std::string, std::string> &values, const std::string &error) {
        if (!error.empty()) {

        }
        else {
            for (std::pair<std::string, std::string> value: values) {
                auto *item = new QTreeWidgetItem(&hash_list, {QString::fromStdString(value.first), QString::fromStdString(value.second)});
                hash_list.addTopLevelItem(item);
            }
        }
    });
}

void HashTabDocument::ItemSelected(QTreeWidgetItem *item, int column) {
    if (value_edit.HasChanges()) {
        // TODO: confirm save old key first
    }
    else {
        last_field = item->text(0).toStdString();
        std::string value = item->text(1).toStdString();
        value_edit.SetValue(value);
    }
}

void HashTabDocument::ValueChanged() {
    has_changes = true;
}

void HashTabDocument::ChangesUndone() {
    has_changes = false;
}

bool HashTabDocument::CanClose() {
    return !has_changes;
}

bool HashTabDocument::IsRedisDoc() {
    return true;
}

std::string HashTabDocument::Name() {
    return key_name;
}

void HashTabDocument::Save(const std::function<void(bool)> &callback) {

}
