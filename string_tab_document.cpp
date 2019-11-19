//
// string_tab_document.cpp
// Created by TheFatNinja 
// 29-10-2019
//

#include "string_tab_document.h"

#include <utility>
#include <QtGui/QFontDatabase>
#include <QtWidgets/QMessageBox>

StringTabDocument::StringTabDocument(std::string key, RedisConnectionPtr connection) :
            key_name(std::move(key)), redis(std::move(connection)) {
    has_changes = false;
    setLayout(&layout);

    layout.addWidget(&value_edit, 1);
    connect(&value_edit, &ValueEditor::value_changed, this, &StringTabDocument::ValueChanged);
    connect(&value_edit, &ValueEditor::changes_undone, this, &StringTabDocument::ChangesUndone);

    TaskQueuePtr queue = GetTaskQueue();
    queue->RunTask<std::string>([this](){
        return redis->GET(key_name);
    },[this](std::string &value_text, const std::string &error) {
        value_edit.SetValue(value_text);
    });
}

void StringTabDocument::ValueChanged() {
    has_changes = true;
}

void StringTabDocument::ChangesUndone() {
    has_changes = false;
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

void StringTabDocument::Save(const std::function<void(bool)> &callback) {
    TaskQueuePtr queue = GetTaskQueue();
    if (has_changes) {
        try {
            std::string value = value_edit.GetValue();
            // TODO: process value for save
            queue->RunTask<bool>([this, value]() {
                return redis->SET(key_name, value);
            }, [this, callback](bool &value, const std::string &error) {
                bool success = value && error.empty();
                if (!success) {
                    if (!error.empty()) {
                        QString error_str = QString::fromStdString(error);
                        QMessageBox::critical(this, "Error", error_str);
                    }
                    else {
                        QMessageBox::critical(this, "Error", "Failed to save value");
                    }
                }
                callback(success);
            });
        }
        catch (std::runtime_error &error) {
            // TODO: Handle error
            queue->RunLater([callback](){
                callback(false);
            });
        }
    }
    else {
        // Nothing to save, call the callback immediately
        queue->RunLater([callback](){
            callback(true);
        });
    }
}
